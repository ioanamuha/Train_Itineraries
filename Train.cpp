#include "Train.h"
bool fileVerificationTrains()
{
    XMLDocument XMLTrains;
    if (XMLTrains.LoadFile("trains.xml") != XML_SUCCESS)
    {
        std::cout << "Nu s-a putut incarca fisierul XML." << std::endl;
        exit(0);
        return false;
    }
    return true;
}
bool compareHours(std::string time1, std::string time2)
{
    int hour1 = std::stoi(time1.substr(0, 2));
    int minute1 = std::stoi(time1.substr(3, 2));

    int hour2 = std::stoi(time2.substr(0, 2));
    int minute2 = std::stoi(time2.substr(3, 2));

    if (hour1 > hour2)
        return false;
    if (hour1 == hour2 && minute1 > minute2)
        return false;

    return true;
}
int createMinuteValue(const std::string &timeless)
{
    int myOrar = std::stoi(timeless.substr(0, 2));
    int myMinutar = std::stoi(timeless.substr(3, 2));
    myMinutar = myOrar * 60 + myMinutar;
    return myMinutar;
}
int myActualTimeInMinutes()
{
    auto sistemTime = system_clock::now();
    time_t myTime = system_clock::to_time_t(sistemTime);
    tm localTime = *localtime(&myTime);
    return localTime.tm_hour * 60 + localTime.tm_min;
}
std::string Modif(XMLElement *early, XMLElement *late)
{
    std::string response;
    std::string earlyTime = early->GetText();
    std::string lateTime = late->GetText();
    if (earlyTime == "0" && lateTime == "0")
        response = " fara modificari ale orei";
    else if (earlyTime != "0" && lateTime == "0")
        response = " cu " + earlyTime + " minute mai repede";
    else if (earlyTime == "0" && lateTime != "0")
        response = " cu " + lateTime + " minute intarziere";
    return response;
}
std::string myActualDay()
{
    std::time_t time = std::time(0);
    std::tm *today = std::localtime(&time);
    return std::to_string(today->tm_mday) + ":" + std::to_string(today->tm_mon + 1) + ":" + std::to_string(1900 + today->tm_year);
}

std::string Train::getTrainInfo(int nr)
{
    std::string raspuns = "Program Trenul " + std::to_string(nr) + '\n';
    XMLDocument XMLTrains;

    if (fileVerificationTrains() == true)
        XMLTrains.LoadFile("trains.xml");

    std::string today = myActualDay();

    XMLNode *rad_trains = XMLTrains.FirstChildElement("Trains");
    XMLElement *train = rad_trains->FirstChildElement("Train");
    while (train != nullptr)
    {
        XMLElement *train_number = train->FirstChildElement("number");
        int trainNumber = std::stoi(train_number->GetText());
        if (nr == trainNumber)
        {
            XMLElement *station = train->FirstChildElement("station");
            while (station)
            {
                XMLElement *station_number = station->FirstChildElement("number");
                int statNumber = std::stoi(station_number->GetText());
                if (statNumber > 0)
                {
                    XMLElement *station_name = station->FirstChildElement("name");
                    XMLElement *station_existing = station->FirstChildElement("existing");

                    if (std::string(station_existing->GetText()) == "true")
                    {
                        XMLElement *arrival = station->FirstChildElement("arrival");
                        XMLElement *departure = station->FirstChildElement("departure");

                        XMLElement *arrival_time = arrival->FirstChildElement("time");
                        XMLElement *arrival_early = arrival->FirstChildElement("early");
                        XMLElement *arrival_late = arrival->FirstChildElement("late");
                        XMLElement *arrival_day = arrival->FirstChildElement("day");
                        std::string arrivalDay = arrival_day->GetText();

                        XMLElement *departure_time = departure->FirstChildElement("time");
                        XMLElement *departure_early = departure->FirstChildElement("early");
                        XMLElement *departure_late = departure->FirstChildElement("late");
                        XMLElement *departure_day = departure->FirstChildElement("day");
                        std::string departureDay = departure_day->GetText();

                        if (today == arrivalDay && departureDay == today)
                            raspuns += "Va ajunge astazi la statia din " + std::string(station_name->GetText()) + " la ora: " + std::string(arrival_time->GetText()) + Modif(arrival_early, arrival_late) + " si va pleca la ora: " + std::string(departure_time->GetText()) + Modif(departure_early, departure_late) + ".\n";
                        else if (today == arrivalDay && departureDay != today)
                            raspuns += "Va ajunge astazi la statia din " + std::string(station_name->GetText()) + " la ora: " + std::string(arrival_time->GetText()) + Modif(arrival_early, arrival_late) + ".\n";
                        else if (today != arrivalDay && departureDay != today)
                            raspuns += "Nu ajunge astazi in statia din " + std::string(station_name->GetText()) + ".\n";
                    }
                }
                station = station->NextSiblingElement("station");
            }
            return raspuns;
        }
        train = train->NextSiblingElement("Train");
    }
    return "Trenul " + std::to_string(nr) + " nu a fost gasit.";
}
std::string Train::getStationInfoDepartures(int nr)
{
    bool exists = false;

    std::string raspuns = "Program Statia " + std::to_string(nr) + ":" + '\n';
    XMLDocument XMLTrains;
    if (fileVerificationTrains() == true)
        XMLTrains.LoadFile("trains.xml");

    int actualTimeInMinutes = myActualTimeInMinutes();
    int supposeddepartureTime = actualTimeInMinutes + 60; // Ora actuală plus o oră

    XMLNode *rad_trains = XMLTrains.FirstChildElement("Trains");
    XMLElement *train = rad_trains->FirstChildElement("Train");
    while (train)
    {
        XMLElement *train_number = train->FirstChildElement("number");
        XMLElement *station = train->FirstChildElement("station");
        while (station)
        {
            XMLElement *station_number = station->FirstChildElement("number");
            XMLElement *existing = station->FirstChildElement("existing");
            XMLElement *departure = station->FirstChildElement("departure");
            XMLElement *time = departure->FirstChildElement("time");

            if (station_number && existing && departure && time && existing->GetText() == std::string("true") && std::stoi(station_number->GetText()) == nr)
            {
                int departureTimeInMinutes = createMinuteValue(time->GetText());
                if (departureTimeInMinutes >= actualTimeInMinutes && departureTimeInMinutes <= supposeddepartureTime)
                {
                    exists = true;
                    raspuns += "Trenul " + std::string(train_number->GetText()) + " va pleca la ora: " + time->GetText();
                    XMLElement *early = departure->FirstChildElement("early");
                    XMLElement *late = departure->FirstChildElement("late");
                    raspuns += Modif(early, late) + ".\n";
                }
            }
            station = station->NextSiblingElement("station");
        }
        train = train->NextSiblingElement("Train");
    }
    if (exists == false)
        raspuns = "Niciun tren nu pleaca din aceasta statie in urmatoarea ora.\n";
    return raspuns;
}
std::string Train::getStationInfoArrivals(int nr)
{
    bool exists = false;

    std::string raspuns = "Program Statia " + std::to_string(nr) + ":" + '\n';
    XMLDocument XMLTrains;
    if (fileVerificationTrains() == true)
        XMLTrains.LoadFile("trains.xml");

    int actualTimeInMinutes = myActualTimeInMinutes();
    int supposedArrivalTime = actualTimeInMinutes + 60;

    XMLNode *rad_trains = XMLTrains.FirstChildElement("Trains");
    XMLElement *train = rad_trains->FirstChildElement("Train");

    while (train)
    {
        XMLElement *train_number = train->FirstChildElement("number");
        XMLElement *station = train->FirstChildElement("station");
        while (station)
        {
            XMLElement *station_number = station->FirstChildElement("number");
            XMLElement *existing = station->FirstChildElement("existing");
            XMLElement *arrival = station->FirstChildElement("arrival");
            XMLElement *time = arrival->FirstChildElement("time");

            if (station_number && existing && arrival && time &&
                existing->GetText() == std::string("true") &&
                std::stoi(station_number->GetText()) == nr)
            {
                int arrivalTimeInMinutes = createMinuteValue(time->GetText());
                if (arrivalTimeInMinutes >= actualTimeInMinutes && arrivalTimeInMinutes <= supposedArrivalTime)
                {
                    exists = true;
                    raspuns += "Trenul " + std::string(train_number->GetText()) + " va sosi la ora: " + time->GetText();
                    XMLElement *early = arrival->FirstChildElement("early");
                    XMLElement *late = arrival->FirstChildElement("late");
                    raspuns += Modif(early, late) + ".\n";
                }
            }
            station = station->NextSiblingElement("station");
        }
        train = train->NextSiblingElement("Train");
    }
    if (exists == false)
    {
        raspuns = "Niciun tren nu ajunge in aceasta statie in urmatoarea ora.\n";
    }
    return raspuns;
}
std::string Train::modifyTrainData(int trainNumber, int stationNumber, int type, int modification)
{
    XMLDocument XMLTrains;
    if (fileVerificationTrains() == true)
        XMLTrains.LoadFile("trains.xml");
    XMLNode *root = XMLTrains.FirstChildElement("Trains");
    XMLElement *train = root->FirstChildElement("Train");
    while (train)
    {
        XMLElement *train_number = train->FirstChildElement("number");
        if (train_number && std::stoi(train_number->GetText()) == trainNumber)
        {
            XMLElement *station = train->FirstChildElement("station");

            while (station)
            {
                XMLElement *exists = station->FirstChildElement("existing");
                XMLElement *station_number = station->FirstChildElement("number");

                if (station_number && std::stoi(station_number->GetText()) == stationNumber)
                {
                    std::string stationExists = exists->GetText();
                    if (stationExists == "true")
                    {
                        //[1] Sosire mai repede
                        //[2] Sosire mai tarziu
                        //[3] Plecare mai repede
                        //[4] Plecare mai tarziu
                        XMLElement *arrivalOrDeparture;
                        if (type == 1 || type == 2)
                            arrivalOrDeparture = station->FirstChildElement("arrival");
                        else
                            arrivalOrDeparture = station->FirstChildElement("departure");
                        XMLElement *time = arrivalOrDeparture->FirstChildElement("time");
                        XMLElement *early = arrivalOrDeparture->FirstChildElement("early");
                        XMLElement *late = arrivalOrDeparture->FirstChildElement("late");

                        if (time)
                        {
                            int timeInMinutes = createMinuteValue(time->GetText());
                            if (type == 1 || type == 3)
                            {
                                timeInMinutes += -modification;
                                late->SetText("0");
                            }
                            else
                            {
                                timeInMinutes += modification;
                                early->SetText("0");
                            }
                            int newHour = (timeInMinutes / 60) % 24;
                            int newMinute = timeInMinutes % 60;

                            std::string newTime;
                            if (newHour < 10)
                                newTime = "0" + std::to_string(newHour);
                            else
                                newTime = std::to_string(newHour);
                            newTime += ":";
                            if (newMinute < 10)
                                newTime = newTime + "0" + std::to_string(newMinute);
                            else
                                newTime = newTime + std::to_string(newMinute);

                            time->SetText(newTime.c_str());
                        }

                        if ((type == 1 || type == 3) && early)
                            early->SetText(std::to_string(modification).c_str());
                        if ((type == 2 || type == 4) && late)
                            late->SetText(std::to_string(modification).c_str());

                        if (type == 3 && compareHours(((station->FirstChildElement("arrival"))->FirstChildElement("time"))->GetText(), time->GetText()) == false)
                            return "Nu se poate modifica ora deoarece trenul ar pleca mai repede decat ar ajunge.";

                        XMLTrains.SaveFile("trains.xml");
                        return "Datele trenului au fost actualizate.";
                    }
                    else
                        return "Statia nu exista.";
                }
                station = station->NextSiblingElement("station");
            }
        }
        train = train->NextSiblingElement("Train");
    }
    return "Trenul specificat nu a fost găsit.";
}
