#pragma once
#include <string>
#include <iostream>
#include <cstring>
#include <tinyxml2.h>
using namespace tinyxml2;

class User
{
    int type;
    std::string username;
    std::string password;
    std::string role;
    bool isLogged;

public:
    void setData(char comanda[1000]);
    void setLogged(bool type, std::string name, std::string pass);
    std::string getUsername();
    std::string getPassword();

    bool getLogged(std::string name, std::string pass);
    std::string createUser(char data[1000]);
    std::string getRole(std::string name, std::string pass);
    bool alreadyLogged(std::string name, std::string pass);
    bool userExists(std::string name, std::string pass);
};