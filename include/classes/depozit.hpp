#pragma once
#ifndef DEPOZIT_HPP
#define DEPOZIT_HPP

class DEPOZIT
{
public:
    struct NOD_DEPOZIT
    {
        char *ID_Produs = nullptr;
        char *ID_Oras = nullptr;
        double Cantitate_Produs = 0.0;
        NOD_DEPOZIT *prev = nullptr;
        NOD_DEPOZIT *next = nullptr;

        NOD_DEPOZIT(char *ID_Produs, char *ID_Oras, double Cantitate_Produs);
        ~NOD_DEPOZIT();
    };

    NOD_DEPOZIT *head_depozit = nullptr;
    NOD_DEPOZIT *tail_depozit = nullptr;

public:
    NOD_DEPOZIT *getHead();
    NOD_DEPOZIT *getTail();
    void inserareDateDepozit(char *ID_Produs, char *ID_Oras, double Cantitate_Produs);
    ~DEPOZIT();
};

#endif