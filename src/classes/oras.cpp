#include "../../include/classes/oras.hpp"
#include <cstring>

ORAS::NOD_ORAS::NOD_ORAS(char *ID_Oras, char *Denumire_Oras, char *Tip_Depozit, double Latitudine, double Longitudine)
{
    this->ID_Oras = strdup(ID_Oras);
    this->denumire_oras = strdup(Denumire_Oras);
    this->tip_depozit = strdup(Tip_Depozit);
    this->latitudine = Latitudine;
    this->longitudine = Longitudine;
    next = nullptr;
    prev = nullptr;
}

ORAS::NOD_ORAS::~NOD_ORAS()
{
    delete[] ID_Oras;
    delete[] denumire_oras;
    delete[] tip_depozit;
}

ORAS::NOD_ORAS *ORAS::getHead()
{
    return head_oras;
}

ORAS::NOD_ORAS *ORAS::getTail()
{
    return tail_oras;
}

void ORAS::insearareDateOrase(char *ID_Oras, char *Denumire_Oras, char *Tip_Depozit, double Latitudine, double Longitudine)
{
    NOD_ORAS *newnod = new NOD_ORAS(ID_Oras, Denumire_Oras, Tip_Depozit, Latitudine, Longitudine);

    if (head_oras == nullptr)
    {
        head_oras = newnod;
        tail_oras = newnod;
    }
    else
    {
        tail_oras->next = newnod;
        newnod->prev = tail_oras;
        tail_oras = newnod;
    }
}

ORAS::~ORAS()
{
    NOD_ORAS *ptr = head_oras;

    while (ptr != nullptr)
    {
        NOD_ORAS *temp = ptr;
        ptr = ptr->next;
        delete temp;
    }
}