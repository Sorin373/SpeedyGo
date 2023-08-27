#include "../../include/classes/aprovizionare.hpp"
#include <cstring>

APROVIZIONARE::NOD_APROVIZIONARE::NOD_APROVIZIONARE(char *ID_Produs, int Cantitate_totala_necesara)
{
    this->ID_Produs = strdup(ID_Produs);
    this->cantitate_totala_necesara = Cantitate_totala_necesara;
    this->next = nullptr;
    this->prev = nullptr;
}

APROVIZIONARE::NOD_APROVIZIONARE::~NOD_APROVIZIONARE()
{
    delete[] ID_Produs;
}

APROVIZIONARE::NOD_APROVIZIONARE *APROVIZIONARE::getHead()
{
    return head_aprovizionare;
}

APROVIZIONARE::NOD_APROVIZIONARE *APROVIZIONARE::getTail()
{
    return tail_aprovizionare;
}

void APROVIZIONARE::inserareDateAprovizionare(char *ID_Produs, int Cantitate_totala_necesara)
{
    NOD_APROVIZIONARE *newnod = new NOD_APROVIZIONARE(ID_Produs, Cantitate_totala_necesara);

    if (head_aprovizionare == nullptr)
    {
        head_aprovizionare = newnod;
        tail_aprovizionare = newnod;
    }
    else
    {
        tail_aprovizionare->next = newnod;
        newnod->prev = tail_aprovizionare;
        tail_aprovizionare = newnod;
    }
}

APROVIZIONARE::~APROVIZIONARE()
{
    NOD_APROVIZIONARE *ptr = head_aprovizionare;

    while (ptr != nullptr)
    {
        NOD_APROVIZIONARE *temp = ptr;
        ptr = ptr->next;
        delete temp;
    }
}