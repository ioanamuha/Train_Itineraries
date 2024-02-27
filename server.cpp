#include <sys/types.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <tinyxml2.h>
#include <queue>
#include "CommandInterface.h"

#define PORT 2909

struct commandQueueStruct
{
    int thread_id;
    Command *command;
};
std::queue<commandQueueStruct *> *coadaPisicii = new std::queue<commandQueueStruct*>();

static void *treat(void *);

typedef struct
{
    pthread_t idSoseta;
    int contorSosete;
} Thread;

Thread *colectieSosete;

int soseta;
int nrsosete;
pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER;

void threadCreate(int i)
{
    void *treat(void *);
    int *arg = (int *)malloc(sizeof(int));
    *arg = i;
    pthread_create(&colectieSosete[i].idSoseta, NULL, &treat, arg);
    return; 
}

void raspunde(int clientulSosetei, int idSoseta)
{

    bool userIsStillConnected = true;
    std::string nume, parola;
    int amLogged = 0; // 0 - neconectat , 1 - operator , 2 - calator

    while (userIsStillConnected)
    {
        std::cout << "[Thread " << idSoseta << "]Vom efectua cereri de la userul " << nume << " inregistrat cu parola " << parola << " de tipul " << amLogged << ".\n";

        char comanda[10000];
        memset(comanda, 0, sizeof(comanda));

        std::string raspuns = "";

        std::cout << "[Thread " << idSoseta << "]Astept sa citesc.\n";
        if (read(clientulSosetei, &comanda, sizeof(comanda)) <= 0)
        {
            std::cout << "[Thread " << idSoseta << "]" << '\n';
            perror("Eroare la read() de la client.\n");
        }
        comanda[strlen(comanda) - 1] = '\0';
        std::cout << "[Thread " << idSoseta << "]Am primit comanda " << comanda << '\n';

        // AICI INCEPEM CAZURILE PENTRU COMENZI
        User user;
        Train train;
        int comanda_int = comanda[0] - '0';
        commandQueueStruct *comandaMeaActuala = new commandQueueStruct();

        switch (comanda_int)
        {
        case 1: // login
        {
       
            Command *login = new Login(user, comanda, amLogged, nume, parola, raspuns);
            comandaMeaActuala->command = login;
            comandaMeaActuala->thread_id = idSoseta;
            coadaPisicii->push(comandaMeaActuala);
            break;
        }
        case 2: // quit
        {
            Command *quit = new Quit(user, amLogged, userIsStillConnected, raspuns, nume, parola);
            comandaMeaActuala->command = quit;
            comandaMeaActuala->thread_id = idSoseta;
            coadaPisicii->push(comandaMeaActuala);
            break;
        }
        case 3: // logout
        {
            Command *logout = new Logout(user, nume, parola, amLogged, raspuns);
            comandaMeaActuala->command = logout;
            comandaMeaActuala->thread_id = idSoseta;
            coadaPisicii->push(comandaMeaActuala);
            break;
        }
        case 4: // status plecari
        {
            Command *stationdepartures = new StationDepartures(user, train, nume, parola, comanda, raspuns);
            comandaMeaActuala->command = stationdepartures;
            comandaMeaActuala->thread_id = idSoseta;
            coadaPisicii->push(comandaMeaActuala);
            break;
        }
        case 5: // status sosiri
        {
            Command *arrivals = new StationArrivals(user, train, nume, parola, comanda, raspuns);
            comandaMeaActuala->command = arrivals;
            comandaMeaActuala->thread_id = idSoseta;
            coadaPisicii->push(comandaMeaActuala);
            break;
        }
        case 6: // program trenuri
        {
            Command *trainInfo = new TrainInfo(user, train, nume, parola, comanda, raspuns);
            comandaMeaActuala->command = trainInfo;
            comandaMeaActuala->thread_id = idSoseta;
            coadaPisicii->push(comandaMeaActuala);
            break;
        }
        case 7: // modificare program
        {
            Command *modifyTrainSchedule = new ModifyTrainSchedule(train, user, comanda, raspuns, nume, parola);
            comandaMeaActuala->command = modifyTrainSchedule;
            comandaMeaActuala->thread_id = idSoseta;
            coadaPisicii->push(comandaMeaActuala);
            break;
        }
        case 8: // adaugare user
        {
            Command *addUser = new AddUser(user, comanda, raspuns);
            comandaMeaActuala->command = addUser;
            comandaMeaActuala->thread_id = idSoseta;
            coadaPisicii->push(comandaMeaActuala);
            break;
        }
        default: // command unknown
            Command *unknown = new Unknown(raspuns);
            comandaMeaActuala->command = unknown;
            comandaMeaActuala->thread_id = idSoseta;
            coadaPisicii->push(comandaMeaActuala);
            break;
        }
        
        while (true)
        {
            auto elem = coadaPisicii->front();
            if (idSoseta == elem->thread_id)
            {
                comandaMeaActuala = elem;
                break;
            }
            sleep(1);
        }

        comandaMeaActuala->command->execute();         
        coadaPisicii->pop();
       
        std::cout << "[Thread " << idSoseta << "]Incercam sa trimitem inapoi comanda " << comanda << '\n';

        raspuns = raspuns + std::to_string(amLogged);
        if (write(clientulSosetei, raspuns.c_str(), raspuns.size()) <= 0)
        {
            std::cout << "[Thread " << idSoseta << "]" << '\n';
            perror("[Thread]Eroare la write() catre client.\n");
        }
        else
            std::cout << "[Thread " << idSoseta << "]Comanda a fost trasnmisa cu succes!\n";

        raspuns.clear();
    }

    std::cout << "[Thread " << idSoseta << "]Userul " << nume << " a inchis aplicatia. Asteptam reconectarea!\n";
}

void *treat(void *arg)
{
    int client;
    int idCLient = *((int *)arg);

    struct sockaddr_in from;
    bzero(&from, sizeof(from));
    std::cout << "[thread] - " << idCLient << " - pornit..." << '\n';
    fflush(stdout);

    for (;;)
    {
        socklen_t length = sizeof(from);
        pthread_mutex_lock(&mlock);
        if ((client = accept(soseta, (struct sockaddr *)&from, &length)) < 0)
        {
            perror("[thread]Eroare la accept().\n");
            continue;
        }
        pthread_mutex_unlock(&mlock);
        colectieSosete[idCLient].contorSosete++;

        std::cout << "[Thread " << idCLient << "]Am conectat clientul\n";
        raspunde(client, idCLient);
        std::cout << "Thread " << idCLient << "]Am raspuns clientului\n";

        close(client);
    }
}

int main(int argc, char *argv[])
{
    struct sockaddr_in server;

    if (argc < 2)
    {
        fprintf(stderr, "Eroare: Primul argument este numarul de fire de executie...");
        exit(1);
    }
    nrsosete = atoi(argv[1]);
    if (nrsosete <= 0)
    {
        fprintf(stderr, "Eroare: Numar de fire invalid...");
        exit(1);
    }
    colectieSosete = (Thread *)calloc(nrsosete, sizeof(Thread));

    if ((soseta = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[server]Eroare la socket().\n");
        return errno;
    }

    int on = 1;
    setsockopt(soseta, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    bzero(&server, sizeof(server));

    server.sin_family = AF_INET;

    server.sin_addr.s_addr = htonl(INADDR_ANY);

    server.sin_port = htons(PORT);

    if (bind(soseta, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[server]Eroare la bind().\n");
        return errno;
    }

    if (listen(soseta, 2) == -1)
    {
        perror("[server]Eroare la listen().\n");
        return errno;
    }

    printf("Nr threaduri %d \n", nrsosete);
    fflush(stdout);
    int i;
    for (i = 0; i < nrsosete; i++)
        threadCreate(i);

    for (;;)
    {
        std::cout << "[server]Asteptam la portul " << PORT << "..." << '\n';
        pause();
    }
};