#include "../../include/classes/depozit.hpp"
#include <cstring>

DEPOZIT::NOD_DEPOZIT::NOD_DEPOZIT(char *ID_Produs, char *ID_Oras, double Cantitate_Produs)
{
    this->ID_Produs = strdup(ID_Produs);
    this->ID_Oras = strdup(ID_Oras);
    this->Cantitate_Produs = Cantitate_Produs;
    prev = nullptr;
    next = nullptr;
}

DEPOZIT::NOD_DEPOZIT::~NOD_DEPOZIT()
{
    delete[] ID_Produs;
    delete[] ID_Oras;
}

DEPOZIT::NOD_DEPOZIT *DEPOZIT::getHead()
{
    return head_depozit;
}

DEPOZIT::NOD_DEPOZIT *DEPOZIT::getTail()
{
    return tail_depozit;
}

void DEPOZIT::inserareDateDepozit(char *ID_Produs, char *ID_Oras, double Cantitate_Produs)
{
    NOD_DEPOZIT *newnod = new NOD_DEPOZIT(ID_Produs, ID_Oras, Cantitate_Produs);

    if (head_depozit == nullptr)
    {
        head_depozit = newnod;
        tail_depozit = newnod;
    }
    else
    {
        tail_depozit->next = newnod;
        newnod->prev = tail_depozit;
        tail_depozit = newnod;
    }
}

DEPOZIT::~DEPOZIT()
{
    NOD_DEPOZIT *ptr = head_depozit;

    while (ptr != nullptr)
    {
        NOD_DEPOZIT *temp = ptr;
        ptr = ptr->next;
        delete temp;
    }
}