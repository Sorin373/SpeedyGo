#include "../../include/classes/detalii_produs.hpp"
#include <cstring>

DETALII_PRODUS::NOD_DETALII_PRODUS::NOD_DETALII_PRODUS(char *ID_Produs, char *Denumire_Produs, char *Categorie_Produs, double Pret_Produs)
{
    this->ID_Produs = strdup(ID_Produs);
    this->Denumire_Produs = strdup(Denumire_Produs);
    this->Categorie_Produs = strdup(Categorie_Produs);
    this->pret_produs = Pret_Produs;
    next = nullptr;
    prev = nullptr;
}

DETALII_PRODUS::NOD_DETALII_PRODUS::~NOD_DETALII_PRODUS()
{
    delete[] ID_Produs;
    delete[] Denumire_Produs;
    delete[] Categorie_Produs;
}

DETALII_PRODUS::NOD_DETALII_PRODUS *DETALII_PRODUS::getHead()
{
    return head_produs;
}

DETALII_PRODUS::NOD_DETALII_PRODUS *DETALII_PRODUS::getTail()
{
    return tail_produs;
}

void DETALII_PRODUS::inserareDateProdus(char *ID_Produs, char *Denumire_Produs, char *Categorie_Produs, double Pret_Produs)
{
    NOD_DETALII_PRODUS *newnod = new NOD_DETALII_PRODUS(ID_Produs, Denumire_Produs, Categorie_Produs, Pret_Produs);

    if (head_produs == nullptr)
    {
        head_produs = newnod;
        tail_produs = newnod;
    }
    else
    {
        tail_produs->next = newnod;
        newnod->prev = tail_produs;
        tail_produs = newnod;
    }
}

DETALII_PRODUS::~DETALII_PRODUS()
{
    NOD_DETALII_PRODUS *ptr = head_produs;

    while (ptr != nullptr)
    {
        NOD_DETALII_PRODUS *temp = ptr;
        ptr = ptr->next;
        delete temp;
    }
}