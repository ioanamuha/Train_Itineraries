#include "User.h"

bool fileVerificationUsers()
{
    XMLDocument XMLUsers;
    if (XMLUsers.LoadFile("users.xml") != XML_SUCCESS)
    {
        std::cout << "Nu s-a putut incarca fisierul XML." << std::endl;
        exit(0);
        return false;
    }
    return true;
}
void User::setData(char comanda[1000])
{
    char *p = strtok(comanda, " ");
    p = strtok(NULL, " ");
    username = p;
    p = strtok(NULL, " ");
    password = p;

}
void User::setLogged(bool type, std::string name, std::string pass)
{
    XMLDocument XMLUsers;
    if (fileVerificationUsers() == true)
        XMLUsers.LoadFile("users.xml");

    XMLNode *users = XMLUsers.FirstChildElement("Users");

    XMLElement *user = users->FirstChildElement("User");
    while (user != nullptr)
    {
        XMLElement *user_name = user->FirstChildElement("Username");
        XMLElement *user_password = user->FirstChildElement("Password");
        XMLElement *user_logged = user->FirstChildElement("Logged");

        if (name == std::string(user_name->GetText()) && pass == std::string(user_password->GetText()))
        {
            user_logged->SetText(type ? "true" : "false");
            XMLUsers.SaveFile("users.xml");
            break;
        }
        user = user->NextSiblingElement("User");
    }
}
std::string User::getUsername()
{
    return username;
}
std::string User::getPassword()
{
    return password;
}
bool User::alreadyLogged(std::string name, std::string pass)
{
    User user;
    if (user.getLogged(name, pass) == true)
        return true;
    else
        return false;
}
std::string User::createUser(char data[1000])
{
    int type = 0;
    std::string name, pass;

    char *p = strtok(data, " ");
    p = strtok(NULL, " ");
    type = *p - '0';
    if (type == 1 || type == 2)
    {
        p = strtok(NULL, " ");
        name = p;
        p = strtok(NULL, " ");
        pass = p;

        XMLDocument XMLUsers;
        if (fileVerificationUsers() == true)
            XMLUsers.LoadFile("users.xml");

        XMLNode *users = XMLUsers.FirstChildElement("Users");
        XMLElement *user = users->FirstChildElement("User");
        while (user)
        {
            XMLElement *user_name = user->FirstChildElement("Username");
            if (user_name)
                if (name == std::string(user_name->GetText()))
                    return "Un user cu acelasi nume exista deja.";
            user = user->NextSiblingElement("User");
        }

        XMLElement *newUser = XMLUsers.NewElement("User");

        XMLElement *newName = XMLUsers.NewElement("Username");
        newName->SetText(name.c_str());
        newUser->InsertEndChild(newName);

        XMLElement *newPassword = XMLUsers.NewElement("Password");
        newPassword->SetText(pass.c_str());
        newUser->InsertEndChild(newPassword);

        XMLElement *newRole = XMLUsers.NewElement("Role");
        if (type == 1)
            newRole->SetText("calator");
        else if (type == 2)
            newRole->SetText("operator");
        newUser->InsertEndChild(newRole);

        XMLElement *newLogged = XMLUsers.NewElement("Logged");
        newLogged->SetText("false");
        newUser->InsertEndChild(newLogged);

        users->InsertEndChild(newUser);
        XMLUsers.SaveFile("users.xml");
        return "Am introdus userul: " + name + " cu parola: " + pass + ", de tipul: " + newRole->GetText();
    }
    else
        return "Introducere incompatibila.";
}
bool User::getLogged(std::string name, std::string pass)
{
    XMLDocument XMLUsers;
    if (fileVerificationUsers() == true)
        XMLUsers.LoadFile("users.xml");

    XMLNode *users = XMLUsers.FirstChildElement("Users");
    XMLElement *user = users->FirstChildElement("User");
    while (user != nullptr)
    {
        XMLElement *user_name = user->FirstChildElement("Username");
        XMLElement *user_password = user->FirstChildElement("Password");
        XMLElement *user_logged = user->FirstChildElement("Logged");

        if (name == std::string(user_name->GetText()) && pass == std::string(user_password->GetText()))
        {
            isLogged = std::string(user_logged->GetText()) == "true";
            return isLogged;
        }
        user = user->NextSiblingElement("User");
    }
    return false;
}
bool User::userExists(std::string username, std::string password)
{
    XMLDocument XMLUsers;
    if (fileVerificationUsers() == true)
        XMLUsers.LoadFile("users.xml");

    XMLNode *users = XMLUsers.FirstChildElement("Users");

    XMLElement *user = users->FirstChildElement("User");
    while (user != nullptr)
    {
        XMLElement *user_name = user->FirstChildElement("Username");
        XMLElement *user_password = user->FirstChildElement("Password");

        if (user_name != nullptr && user_password != nullptr)
        {
            if (username == std::string(user_name->GetText()) && password == std::string(user_password->GetText()))
                return true;
        }
        user = user->NextSiblingElement("User");
    }
    return false;
}
std::string User::getRole(std::string name, std::string pas)
{
    XMLDocument XMLUsers;
    if (fileVerificationUsers() == true)
        XMLUsers.LoadFile("users.xml");

    XMLNode *users = XMLUsers.FirstChildElement("Users");
    XMLElement *user = users->FirstChildElement("User");
    while (user != nullptr)
    {
        XMLElement *user_name = user->FirstChildElement("Username");
        XMLElement *user_password = user->FirstChildElement("Password");
        XMLElement *user_role = user->FirstChildElement("Role");

        if (name == std::string(user_name->GetText()) && pas == std::string(user_password->GetText()))
        {
            role = std::string(user_role->GetText());
            return role;
        }
        user = user->NextSiblingElement("User");
    }
    return "Acest user nu are rol.";
}
