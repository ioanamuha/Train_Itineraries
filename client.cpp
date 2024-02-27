#include <sys/types.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

int port;

bool commandVerification(const std::string &command)
{
    const char *commandChar = command.c_str();

    for (int contor = 0; contor < strlen(commandChar); contor++)
    {
        if (std::isdigit(commandChar[contor]) == false)
            return false;
    }

    if (atoi(commandChar) > 9)
        return false;
    return true;
}

int main()
{
    int soseta;
    struct sockaddr_in server;

    port = 2909;
    server.sin_family = AF_INET;

    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    server.sin_port = htons(port);

    if ((soseta = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Eroare la socket().\n");
        return errno;
    }

    if (connect(soseta, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[aplicatie] Eroare la connect().\n");
        return errno;
    }

    bool program = true;
    int amLogged = 0; // 0-nu 1-operator 2-calator
    while (program)
    {
        // VARIABILE FOLOSITE
        int nr = 0;
        char raspuns[10000] = {'\0'}, comanda_char;
        std::string userType, comanda, username, password, station_leaving, station_arriving, train_number, station_number, station_data, time, newName, newPassword, newType;

        if (amLogged == 1)
        {
            std::cout << "Lista comenzi: " << '\n'
                      << "[1] Login" << '\n'
                      << "[2] Quit" << '\n'
                      << "[3] Logout" << '\n'
                      << "[4] Status plecari in urmatoarea ora" << '\n'
                      << "[5] Status sosiri in urmatoarea ora" << '\n'
                      << "[6] Program trenuri" << '\n'
                      << "[7] Modificare program trenuri" << '\n'
                      << "[8] Adaugare user" << '\n';
        }
        else if (amLogged == 2)
        {
            std::cout << "Lista comenzi: " << '\n'
                      << "[1] Login" << '\n'
                      << "[2] Quit" << '\n'
                      << "[3] Logout" << '\n'
                      << "[4] Status plecari in urmatoarea ora" << '\n'
                      << "[5] Status sosiri in urmatoarea ora" << '\n'
                      << "[6] Program trenuri" << '\n';
        }
        else
        {
            std::cout << "Lista comenzi: " << '\n'
                      << "[1] Login" << '\n'
                      << "[2] Quit" << '\n';
        }
        std::cout << "Introduceti o comanda: ";
        fflush(stdout);

        std::cin >> comanda;
        int comanda_int;
        if (commandVerification(comanda) == false)
            comanda_int = 0;
        else
            comanda_int = std::stoi(comanda);

        switch (comanda_int)
        {
        case 1: // login
        {
            std::cout << "Introduceti username-ul: ";
            std::cin >> username;
            comanda = "1 " + username + " ";

            std::cout << "Introduceti parola: ";
            std::cin >> password;
            comanda = comanda + password;
            break;
        }
        case 2: // quit
        {
            comanda = "2";
            program = false;
            break;
        }
        case 3: // logout
        {
            if (amLogged != 0)
                comanda = "3";
            break;
        }
        case 4: // status plecari
        {
            if (amLogged != 0)
            {
                std::cout << "Puteti vizualiza pentru: " << '\n'
                          << "[1] Iasi" << '\n'
                          << "[2] Galati" << '\n'
                          << "[3] Constanta" << '\n'
                          << "[4] Bucuresti" << '\n'
                          << "Selectati statia: ";
                std::cin >> station_leaving;
                if (commandVerification(station_leaving) == false)
                    station_leaving = "0";
                comanda = "4 " + station_leaving;
            }
            break;
        }
        case 5: // status sosiri
        {
            if (amLogged != 0)
            {
                std::cout << "Puteti vizualiza pentru: " << '\n'
                          << "[1] Iasi" << '\n'
                          << "[2] Galati" << '\n'
                          << "[3] Constanta" << '\n'
                          << "[4] Bucuresti" << '\n'
                          << "Selectati statia: ";
                std::cin >> station_arriving;
                if (commandVerification(station_arriving) == false)
                    station_arriving = "0";
                comanda = "5 " + station_arriving;
            }
            break;
        }
        case 6: // program trenuri
        {
            if (amLogged != 0)
            {
                std::cout << "Putetii vizualiza programul pentru: " << '\n'
                          << "[1] Trenul 1" << '\n'
                          << "[2] Trenul 2" << '\n'
                          << "[3] Trenul 3" << '\n'
                          << "[4] Toate trenurile" << '\n'
                          << "Selectati trenul: ";
                std::cin >> train_number;
                if (commandVerification(train_number) == false)
                    train_number = "0";
                comanda = "6 " + train_number;
            }
            break;
        }
        case 7: // modificare program
        {
            if (amLogged == 1)
            {
                std::cout << "Selectati trenul pentru care doriti sa modificati: " << '\n'
                          << "[1] Trenul 1" << '\n'
                          << "[2] Trenul 2" << '\n'
                          << "[3] Trenul 3" << '\n'
                          << "Selectati trenul: ";
                std::cin >> train_number;
                if (commandVerification(train_number) == false)
                    train_number = "0";
                comanda = "7 " + train_number;
                std::cout << "Statii disponibile: " << '\n'
                          << "[1] Iasi" << '\n'
                          << "[2] Galati" << '\n'
                          << "[3] Constanta" << '\n'
                          << "[4] Bucuresti" << '\n'
                          << "Selectati statia: ";
                std::cin >> station_number;
                if (commandVerification(station_number) == false)
                    station_number = "0";
                comanda = comanda + " " + station_number;
                std::cout << "Date pe care le puteti modifica: " << '\n'
                          << "[1] Sosire mai repede" << '\n'
                          << "[2] Sosire mai tarziu" << '\n'
                          << "[3] Plecare mai repede" << '\n'
                          << "[4] Plecare mai tarziu" << '\n'
                          << "Selectati datele: ";
                std::cin >> station_data;
                if (commandVerification(station_data) == false)
                    station_data = "0";
                std::cout << "Introduceti modificarea pe care o doriti in minute: ";
                std::cin >> time;
                comanda = comanda + " " + station_data + " " + time;
            }
            break;
        }
        case 8: // adaugare user
        {
            std::cout << "Tipuri de utilizator disponibilie pentru noul utilizator: \n[1] Calator\n[2] Operator feroviar\nIntroduceti tipul noului utilizator: ";
            std::cin >> newType;
            std::cout << "Introduceti numele noului utilizator: ";
            std::cin >> newName;
            std::cout << "Introduceti parola noului utilizator: ";
            std::cin >> newPassword;
            comanda = "8 " + newType + " " + newName + " " + newPassword;
            break;
        }
        default:
        {
            comanda = "unknown";
            break;
        }
        }

        comanda = comanda + std::to_string(amLogged);

        if (write(soseta, comanda.c_str(), comanda.size()) <= 0)
        {
            perror("[aplicatie] Eroare la write() spre server.\n");
            return errno;
        }

        std::cout << "Comanda a fost trimisa!\n";

        if (read(soseta, &raspuns, sizeof(raspuns)) < 0)
        {
            perror("[aplicatie] Eroare la read() de la server.\n");
            return errno;
        }
        
        amLogged = raspuns[strlen(raspuns) - 1] - '0';
        raspuns[strlen(raspuns) - 1] = '\0';
        std::cout << raspuns << '\n';
        memset(raspuns, 0, sizeof(raspuns));
    }
    close(soseta);
}