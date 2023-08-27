#include "../../include/classes/autentificare.hpp"
#include <cstring>

AUTENTIFICARE::NOD_AUTENTIFICARE *AUTENTIFICARE::data = nullptr;

AUTENTIFICARE::NOD_AUTENTIFICARE::NOD_AUTENTIFICARE(char *host_name, char *username, char *parola, char *DB)
{
    this->host_name = strdup(host_name);
    this->username = strdup(username);
    this->parola = strdup(parola);
    this->DB = strdup(DB);
}

AUTENTIFICARE::NOD_AUTENTIFICARE::~NOD_AUTENTIFICARE()
{
    delete[] host_name;
    delete[] username;
    delete[] parola;
    delete[] DB;
}

void AUTENTIFICARE::introducere_date(char *vHost_name, char *vUsername, char *vParola, char *vDB)
{
    cleanup();
    data = new NOD_AUTENTIFICARE(vHost_name, vUsername, vParola, vDB);
}

AUTENTIFICARE::NOD_AUTENTIFICARE *AUTENTIFICARE::get_nod()
{
    return data;
}

void AUTENTIFICARE::cleanup()
{
    if (data)
    {
        delete data;
        data = nullptr;
    }
}