#include "../../include/classes/traseu.hpp"
#include <cstring>
#include <vector>

TRASEU::NOD_TRASEU::NOD_TRASEU(int start, int destinatie, double distanta, std::vector<int> &traseu)
{
    this->start = start;
    this->destinatie = destinatie;
    this->distanta = distanta;
    this->traseu.assign(traseu.begin(), traseu.end());
}

TRASEU::NOD_TRASEU::~NOD_TRASEU()
{
    traseu.clear();
}

TRASEU::NOD_TRASEU *TRASEU::getHead()
{
    return head_traseu;
}

TRASEU::NOD_TRASEU *TRASEU::getTail()
{
    return tail_traseu;
}

void TRASEU::inserareDateTraseu(int start, int destinatie, double distanta, std::vector<int> &traseu)
{
    NOD_TRASEU *newnod = new NOD_TRASEU(start, destinatie, distanta, traseu);

    if (head_traseu == nullptr)
    {
        head_traseu = newnod;
        tail_traseu = newnod;
    }
    else
    {
        tail_traseu->next = newnod;
        newnod->prev = tail_traseu;
        tail_traseu = newnod;
    }
}

TRASEU::~TRASEU()
{
    NOD_TRASEU *ptr = head_traseu;

    while (ptr != nullptr)
    {
        NOD_TRASEU *temp = ptr;
        ptr = ptr->next;
        delete temp;
    }
}