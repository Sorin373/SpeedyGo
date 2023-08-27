#pragma once
#ifndef DETALII_PRODUS_HPP
#define DETALII_PRODUS_HPP

class DETALII_PRODUS
{
public:
    struct NOD_DETALII_PRODUS
    {
        char *ID_Produs = nullptr;
        char *Denumire_Produs = nullptr;
        char *Categorie_Produs = nullptr;
        double pret_produs = 0.0;
        NOD_DETALII_PRODUS *next = nullptr;
        NOD_DETALII_PRODUS *prev = nullptr;

        NOD_DETALII_PRODUS(char *ID_Produs, char *Denumire_Produs, char *Categorie_Produs, double Pret_Produs);
        ~NOD_DETALII_PRODUS();
    };

    NOD_DETALII_PRODUS *head_produs = nullptr;
    NOD_DETALII_PRODUS *tail_produs = nullptr;

public:
    NOD_DETALII_PRODUS *getHead();
    NOD_DETALII_PRODUS *getTail();
    void inserareDateProdus(char *ID_Produs, char *Denumire_Produs, char *Categorie_Produs, double Pret_Produs);
    ~DETALII_PRODUS();
};

#endif