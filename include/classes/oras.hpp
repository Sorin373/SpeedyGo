#pragma once
#ifndef ORAS_HPP
#define ORAS_HPP

class ORAS
{
public:
    struct NOD_ORAS
    {
        char *ID_Oras = nullptr;
        char *denumire_oras = nullptr;
        char *tip_depozit = nullptr;
        double latitudine = 0.0;
        double longitudine = 0.0;
        NOD_ORAS *prev = nullptr;
        NOD_ORAS *next = nullptr;

        NOD_ORAS(char *ID_Oras, char *Denumire_Oras, char *Tip_Depozit, double Latitudine, double Longitudine);
        ~NOD_ORAS();
    };

    NOD_ORAS *head_oras = nullptr;
    NOD_ORAS *tail_oras = nullptr;

public:
    NOD_ORAS *getHead();
    NOD_ORAS *getTail();
    void insearareDateOrase(char *ID_Oras, char *Denumire_Oras, char *Tip_Depozit, double Latitudine, double Longitudine);
    ~ORAS();
};

#endif