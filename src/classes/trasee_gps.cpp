#include "../../include/classes/trasee_gps.hpp"
#include <cstring>

TRASEE_GPS::NOD_TRASEE_GPS::NOD_TRASEE_GPS(double distanta, int durata, char *start, char *destinatie)
{
    this->distanta = distanta;
    this->durata = durata;
    this->start = strdup(start);
    this->destinatie = strdup(destinatie);
    prev = nullptr;
    next = nullptr;
}

TRASEE_GPS::NOD_TRASEE_GPS::~NOD_TRASEE_GPS()
{
    delete[] start;
    delete[] destinatie;
}

TRASEE_GPS::NOD_TRASEE_GPS *TRASEE_GPS::getHead()
{
    return head_gps;
}

TRASEE_GPS::NOD_TRASEE_GPS *TRASEE_GPS::getTail()
{
    return tail_gps;
}

void TRASEE_GPS::introducere_date_trasee_gps(double distanta, int durata, char *start, char *destinatie)
{
    NOD_TRASEE_GPS *newnod = new NOD_TRASEE_GPS(distanta, durata, start, destinatie);

    if (head_gps == nullptr)
    {
        head_gps = newnod;
        tail_gps = newnod;
    }
    else
    {
        tail_gps->next = newnod;
        newnod->prev = tail_gps;
        tail_gps = newnod;
    }
}

TRASEE_GPS::~TRASEE_GPS()
{
    NOD_TRASEE_GPS *ptr = head_gps;

    while (ptr != nullptr)
    {
        NOD_TRASEE_GPS *temp = ptr;
        ptr = ptr->next;
        delete temp;
    }
}