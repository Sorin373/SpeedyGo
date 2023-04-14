#ifndef LOGIC
#define LOGIC

#include <iostream>
#include "declarations.hpp"

using namespace std;

void inserareDateDepozit(char *vID_Produs, char *vID_Depozit, double vCantitate_Produs)
{
    NOD_PRODUS_DEPOZIT *newnod = (NOD_PRODUS_DEPOZIT *)malloc(sizeof(NOD_PRODUS_DEPOZIT));
    newnod->ID_Produs = strdup(vID_Produs);
    newnod->ID_Depozit = strdup(vID_Depozit);
    newnod->Cantitate_Produs = vCantitate_Produs;
    if (head_depozit != nullptr)
    {
        head_depozit->prev_d = newnod;
        newnod->next_d = head_depozit;
        head_depozit->prev_d = newnod;
    }
    head_depozit = newnod;
}

void inserareDateProduse(char *vID_Produs, char *vDenumire_Produs, char *vCategorie_Produs, double vPret_Produs)
{
    NOD_DETALII_PRODUS *newnod = (NOD_DETALII_PRODUS *)malloc(sizeof(NOD_DETALII_PRODUS));

    newnod->ID_Produs = strdup(vID_Produs);
    newnod->Denumire_Produs = strdup(vDenumire_Produs);
    newnod->Categorie_Produs = strdup(vCategorie_Produs);
    newnod->pret_produs = vPret_Produs;

    if (head_produs != nullptr)
    {
        head_produs->prev_p = newnod;
        newnod->next_p = head_produs;
        head_produs->prev_p = newnod;
    }

    head_produs = newnod;
}

void inserareDateProduseLocal(char *vID_Oras, char *vID_Produs, double vCantitate_Produs_Local)
{
    NOD_PRODUS_DEPOZIT_LOCAL *newnod = (NOD_PRODUS_DEPOZIT_LOCAL *)malloc(sizeof(NOD_PRODUS_DEPOZIT_LOCAL));

    newnod->ID_Oras = strdup(vID_Oras);
    newnod->ID_Produs = strdup(vID_Produs);
    newnod->cantitate_produs = vCantitate_Produs_Local;

    if (head_local != nullptr)
    {
        head_local->prev_l = newnod;
        newnod->next_l = head_local;
        head_local->prev_l = newnod;
    }

    head_local = newnod;
}

void afisareDateLocal()
{
    NOD_PRODUS_DEPOZIT_LOCAL *ptr;
    ptr = head_local;
    while (ptr != nullptr)
    {
        cout << "ID_Oras: " << ptr->ID_Oras << " ";
        cout << "ID_Produs: " << ptr->ID_Produs << " ";
        cout << "Cantitate: " << ptr->cantitate_produs << endl;
        ptr = ptr->next_l;
    }
}

void afisareDateDepozit()
{
    NOD_PRODUS_DEPOZIT *ptr;
    ptr = head_depozit;
    while (ptr != nullptr)
    {
        cout << "ID_Produs: " << ptr->ID_Produs << ", ";
        cout << "ID_Depozit: " << ptr->ID_Depozit << ", ";
        cout << "Cantitate_Produs: " << ptr->Cantitate_Produs << endl;
        ptr = ptr->next_d;
    }
}

void afisareDateProdus()
{
    NOD_DETALII_PRODUS *ptr;
    ptr = head_produs;
    while (ptr != nullptr)
    {
        cout << "ID_Produs: " << ptr->ID_Produs << " ";
        cout << "Nume_Produs: " << ptr->Denumire_Produs << endl;
        ptr = ptr->next_p;
    }
}

void statisticaStoc()
{
    NOD_PRODUS_DEPOZIT_LOCAL *ptr = head_local;
    while (ptr != nullptr)
    {
        if (ptr->cantitate_produs < 50)
        {
            int tempIdOras = stoi(ptr->ID_Oras);
            if (tempIdOras > 5)
                matrice_drum[tempIdOras][tempIdOras] = true;
        }
        ptr = ptr->next_l;
    }
    
    /*
    for (unsigned int i = 1; i < 45; i++)
    {
        for (unsigned int j = 1; j < 45; j++)
            if (matrice_drum[i][j] == true)
                cout << 1;
            else
                cout << 0;
        cout << endl;
    }
    */
}

int detDepozitStart(const int tempID_Oras)
{

}

void determinareStartTraseu()
{
    
}

void determinareTraseu()
{

}

#endif