#include "../../include/classes/authentification.hpp"
#include <cstring>

AUTHENTICATION::AUTHENTICATION_NODE *AUTHENTICATION::data = nullptr;

AUTHENTICATION::AUTHENTICATION_NODE::AUTHENTICATION_NODE(const char *host_name, const char *username, const char *password, const char *database)
{
    this->host_name = strdup(host_name);
    this->username = strdup(username);
    this->password = strdup(password);
    this->database = strdup(database);
}

AUTHENTICATION::AUTHENTICATION_NODE::~AUTHENTICATION_NODE(void)
{
    delete[] host_name;
    delete[] username;
    delete[] password;
    delete[] database;
}

void AUTHENTICATION::getData(const char *host_name, const char *username, const char *password, const char *database)
{
    cleanup();
    data = new AUTHENTICATION_NODE(host_name, username, password, database);
}

AUTHENTICATION::AUTHENTICATION_NODE *AUTHENTICATION::getNode(void)
{
    return data;
}

void AUTHENTICATION::cleanup(void)
{
    if (data)
    {
        delete data;
        data = nullptr;
    }
}