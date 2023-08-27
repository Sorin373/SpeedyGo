#pragma once
#ifndef AUTENTIFICARE_HPP
#define AUTENTIFICARE_HPP

class AUTENTIFICARE
{
private:
    struct NOD_AUTENTIFICARE
    {
        char *host_name = nullptr;
        char *username = nullptr;
        char *parola = nullptr;
        char *DB = nullptr;

        NOD_AUTENTIFICARE(char *host_name, char *username, char *parola, char *DB);
        ~NOD_AUTENTIFICARE();
    };

    static NOD_AUTENTIFICARE *data;

public:
    static void introducere_date(char *vHost_name, char *vUsername, char *vParola, char *vDB);
    static NOD_AUTENTIFICARE *get_nod();
    static void cleanup();
    ~AUTENTIFICARE() = default;
};

#endif