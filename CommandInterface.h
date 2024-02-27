#include "User.h"
#include "Train.h"

class Command
{
public:
    virtual ~Command() {}
    virtual void execute() = 0;
};

class Login : public Command
{
private:
    User &user;
    char *commandData;
    int &amLogged;
    std::string &raspunsServer;
    std::string &nume;
    std::string &parola;

public:
    Login(User &user, char *commandData, int &amLogged, std::string &nume, std::string &parola, std::string &raspunsServer)
        : user(user), commandData(commandData), amLogged(amLogged), nume(nume), parola(parola), raspunsServer(raspunsServer) {}

    void execute() override
    {
        if (amLogged == 0)
        {
            user.setData(commandData);
            nume = user.getUsername();
            parola = user.getPassword();
            if (user.userExists(nume, parola) == true)
            {
                if (user.getLogged(nume, parola) == false)
                {
                    user.setLogged(true, nume, parola);
                    raspunsServer = "Buna, " + nume + "! Te-ai conectat cu succes.";

                    if (user.getRole(user.getUsername(), user.getPassword()) == "calator")
                        amLogged = 2; // Calator
                    else
                        amLogged = 1; // Operator
                }
                else
                    raspunsServer = "Sunteti deja conectat.";
            }
            else
                raspunsServer = "Userul nu exista in baza de date.";
        }
        else
            raspunsServer = "Sunteti deja conectat.";
    }
};

class Quit : public Command
{
private:
    User &user;
    int &amLogged;
    bool &userIsStillConnected;
    std::string &raspunsServer;
    std::string &nume;
    std::string &parola;

public:
    Quit(User &user, int &amLogged, bool &userIsStillConnected, std::string &raspunsServer, std::string &nume, std::string &parola)
        : user(user), amLogged(amLogged), userIsStillConnected(userIsStillConnected), raspunsServer(raspunsServer), nume(nume), parola(parola) {}

    void execute() override
    {
        user.setLogged(false, nume, parola);
        raspunsServer = "Ati iesit din program.";
        amLogged = 0;
        userIsStillConnected = false;
    }
};

class Logout : public Command
{
private:
    User &user;
    std::string nume;
    std::string parola;
    int &amLogged;
    std::string &raspunsServer;

public:
    Logout(User &user, const std::string &nume, const std::string &parola, int &amLogged, std::string &raspunsServer)
        : user(user), nume(nume), parola(parola), amLogged(amLogged), raspunsServer(raspunsServer) {}

    void execute() override
    {
        if (user.getLogged(nume, parola) == true)
        {
            user.setLogged(false, nume, parola);
            raspunsServer = "Ati fost deconectat.";
            amLogged = 0;
        }
        else
            raspunsServer = "Nu sunteti conectat inca pentru a face asta.";
    }
};

class AddUser : public Command
{
private:
    User &user;
    std::string commandData;
    std::string &raspunsServer;

public:
    AddUser(User &user, const std::string &commandData, std::string &raspunsServer) : user(user), commandData(commandData), raspunsServer(raspunsServer) {}

    void execute() override
    {
        user.createUser(const_cast<char *>(commandData.c_str()));
        raspunsServer = "A fost adaugat noul user.";
    }
};

class StationDepartures : public Command
{
private:
    User &user;
    Train &train;
    std::string nume;
    std::string parola;
    std::string &raspunsServer;
    char *comanda;

public:
    StationDepartures(User &user, Train &train, const std::string &nume, const std::string &parola, char *comanda, std::string &raspunsServer)
        : user(user), train(train), nume(nume), parola(parola), comanda(comanda), raspunsServer(raspunsServer) {}

    void execute() override
    {
        if (!user.getLogged(nume, parola))
            raspunsServer = "Trebuie sa fiti conectat pentru a vizualiza statusul plecarilor.";

        std::string comandaStr(comanda);
        if (comandaStr.size() < 3)
            raspunsServer = "Comanda invalida.";

        int comanda_trenuri = comandaStr[2] - '0';
        switch (comanda_trenuri)
        {
        case 1: // Iasi
        case 2: // Galati
        case 3: // Constanta
        case 4: // Bucuresti
            raspunsServer = train.getStationInfoDepartures(comanda_trenuri);
            break;
        default:
            raspunsServer = "Introducere incompatibila.";
            break;
        }
    }
};

class StationArrivals : public Command
{
private:
    User &user;
    Train &train;
    std::string nume;
    std::string parola;
    std::string &raspunsServer;
    char *comanda;

public:
    StationArrivals(User &user, Train &train, const std::string &nume, const std::string &parola, char *comanda, std::string &raspunsServer)
        : user(user), train(train), nume(nume), parola(parola), comanda(comanda), raspunsServer(raspunsServer) {}

    void execute() override
    {
        if (!user.getLogged(nume, parola))
            raspunsServer = "Trebuie sa fiti conectat pentru a vizualiza statusul sosirilor.";

        std::string comandaStr(comanda);
        if (comandaStr.size() < 3)
            raspunsServer = "Comanda invalida.";

        int comanda_trenuri = comandaStr[2] - '0';
        switch (comanda_trenuri)
        {
        case 1: // Iasi
        case 2: // Galati
        case 3: // Constanta
        case 4: // Bucuresti
            raspunsServer = train.getStationInfoArrivals(comanda_trenuri);
            break;
        default:
            raspunsServer = "Introducere incompatibila.";
            break;
        }
    }
};

class TrainInfo : public Command
{
private:
    User &user;
    Train &train;
    std::string nume;
    std::string parola;
    std::string &raspunsServer;
    char *comanda;

public:
    TrainInfo(User &user, Train &train, const std::string &nume, const std::string &parola, char *comanda, std::string &raspunsServer)
        : user(user), train(train), nume(nume), parola(parola), comanda(comanda), raspunsServer(raspunsServer) {}

    void execute() override
    {
        if (!user.getLogged(nume, parola))
            raspunsServer = "Trebuie sa fiti conectat pentru a vizualiza programul trenurilor.";

        std::string comandaStr(comanda);
        if (comandaStr.size() < 3)
            raspunsServer = "Comanda invalida.";

        int comanda_trenuri = comandaStr[2] - '0';
        switch (comanda_trenuri)
        {
        case 1: // Trenul 1
        case 2: // Trenul 2
        case 3: // Trenul 3
            raspunsServer = train.getTrainInfo(comanda_trenuri);
            break;
        case 4: // Toate trenurile
            for (int i = 1; i <= 3; i++)
                raspunsServer = raspunsServer + train.getTrainInfo(i);
            break;
        default:
            raspunsServer = "Introducere incompatibila.";
        }
    }
};

class ModifyTrainSchedule : public Command
{
private:
    Train &train;
    User &user;
    char *command;
    std::string nume;
    std::string parola;
    std::string &raspunsServer;

public:
    ModifyTrainSchedule(Train &train, User &user, char *command, std::string &raspunsServer, std::string nume, std::string parola)
        : train(train), user(user), command(command), raspunsServer(raspunsServer), nume(nume), parola(parola) {}

    void execute() override
    {
        if (!user.getLogged(nume, parola))
            raspunsServer = "Trebuie sa fiti conectat pentru a vedea daca aveti optiunea de a modifica programul.";
        else
        {
            if (user.getRole(nume, parola) != "operator")
                raspunsServer = "Ca si calator nu aveti posibilitatea de a modifica date. Doar operatorii feroviari pot face acest lucru.";
            else
            {
                int comanda_trenuri = command[2] - '0';
                    switch (comanda_trenuri)
                    {
                    case 1: // Trenul 1
                    case 2: // Trenul 2
                    case 3: // Trenul 3
                    {
                        int station_number = command[4] - '0';
                        switch (station_number)
                        {
                        case 1: // Iasi
                        case 2: // Galati
                        case 3: // Constanta
                        case 4: // Bucuresti
                        {
                            int type = command[6] - '0';
                            switch (type)
                            {                                   
                            case 1: // Sosire mai repede
                            case 2: // Sosire mai tarziu
                            {
                                int modification = 0;
                                char myModification[10] = {'\0'};
                                strcpy(myModification, command + 8);
                                for (int i = 0; i < strlen(myModification); i++)
                                    modification = modification*10 + (myModification[i] - '0') ;
                                raspunsServer = train.modifyTrainData(comanda_trenuri, station_number, type, modification);
                                raspunsServer = train.modifyTrainData(comanda_trenuri, station_number, type+2, modification);
                                break;
                            }
                            case 3: // Plecare mai repede
                            case 4: // Plecare mai tarziu
                            {
                                int modification = 0;
                                char myModification[10] = {'\0'};
                                strcpy(myModification, command + 8);
                                for (int i = 0; i < strlen(myModification); i++)
                                    modification = modification*10 + (myModification[i] - '0') ;
                                raspunsServer = train.modifyTrainData(comanda_trenuri, station_number, type, modification);
                                break;
                            }
                            default:
                            {
                                raspunsServer = "Introducere incompatibila.";
                                break;
                            }
                            }
                            break;
                        }
                        default:
                        {
                            raspunsServer = "Introducere incompatibila.";
                            break;
                        }
                        }
                        break;
                    }
                    default:
                    {
                        raspunsServer = "Introducere incompatibila.";
                        break;
                    }
                    }
            }
        }
    }
};

class Unknown : public Command
{
private:
std::string &raspunsServer;
public:
    Unknown(std::string &raspunsServer) : raspunsServer(raspunsServer){}
    void execute() override
    {
        raspunsServer = "Comanda introdusa nu este valida.";
    }
};