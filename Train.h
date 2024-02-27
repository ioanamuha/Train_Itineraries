#pragma once
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <tinyxml2.h>

using namespace tinyxml2;
using namespace std::chrono;

class Train
{
    int number;

public:
    std::string getTrainInfo(int nr);
    std::string getStationInfoDepartures(int nr);
    std::string getStationInfoArrivals(int nr);
    std::string modifyTrainData(int train_number, int station_number, int type, int modification);
};