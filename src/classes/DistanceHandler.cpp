#include "../../include/classes/DistanceHandler.hpp"
#include <cstring>

ADJACENCY_MATRIX_INITIALIZER::ADJACENCY_MATRIX_INITIALIZER_NODE::ADJACENCY_MATRIX_INITIALIZER_NODE(const double distance, const int duration, const char *start, const char *destination)
{
    this->distance = distance;
    this->duration = duration;
    this->start = strdup(start);
    this->destination = strdup(destination);
    prev = nullptr;
    next = nullptr;
}

ADJACENCY_MATRIX_INITIALIZER::ADJACENCY_MATRIX_INITIALIZER_NODE::~ADJACENCY_MATRIX_INITIALIZER_NODE()
{
    delete[] start;
    delete[] destination;
}

ADJACENCY_MATRIX_INITIALIZER::ADJACENCY_MATRIX_INITIALIZER_NODE *ADJACENCY_MATRIX_INITIALIZER::getHead(void)
{
    return head;
}

ADJACENCY_MATRIX_INITIALIZER::ADJACENCY_MATRIX_INITIALIZER_NODE *ADJACENCY_MATRIX_INITIALIZER::getTail(void)
{
    return tail;
}

double ADJACENCY_MATRIX_INITIALIZER::ADJACENCY_MATRIX_INITIALIZER_NODE::getDistance(void)
{
    return distance;
}

int ADJACENCY_MATRIX_INITIALIZER::ADJACENCY_MATRIX_INITIALIZER_NODE::getDuration(void)
{
    return duration;
}

char *ADJACENCY_MATRIX_INITIALIZER::ADJACENCY_MATRIX_INITIALIZER_NODE::getStart(void)
{
    return start;
}

char *ADJACENCY_MATRIX_INITIALIZER::ADJACENCY_MATRIX_INITIALIZER_NODE::getDestination(void)
{
    return destination;
}

void ADJACENCY_MATRIX_INITIALIZER::getData(const double distance, const int duration, const char *start, const char *destination)
{
    ADJACENCY_MATRIX_INITIALIZER_NODE *newnod = new ADJACENCY_MATRIX_INITIALIZER_NODE(distance, duration, start, destination);

    if (head == nullptr)
    {
        head = newnod;
        tail = newnod;
    }
    else
    {
        tail->next = newnod;
        newnod->prev = tail;
        tail = newnod;
    }
}

ADJACENCY_MATRIX_INITIALIZER::~ADJACENCY_MATRIX_INITIALIZER()
{
    ADJACENCY_MATRIX_INITIALIZER_NODE *ptr = head;

    while (ptr != nullptr)
    {
        ADJACENCY_MATRIX_INITIALIZER_NODE *temp = ptr;
        ptr = ptr->next;
        delete temp;
    }
}