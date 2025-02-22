#include "../../../include/classes/DistanceHandler.hpp"
#include "../../../include/declarations.hpp"

#include <stdlib.h>

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
    free(start);
    free(destination);
}

ADJACENCY_MATRIX_INITIALIZER::ADJACENCY_MATRIX_INITIALIZER_NODE *ADJACENCY_MATRIX_INITIALIZER::getHead(void) const noexcept
{
    return head;
}

ADJACENCY_MATRIX_INITIALIZER::ADJACENCY_MATRIX_INITIALIZER_NODE *ADJACENCY_MATRIX_INITIALIZER::getTail(void) const noexcept
{
    return tail;
}

double ADJACENCY_MATRIX_INITIALIZER::ADJACENCY_MATRIX_INITIALIZER_NODE::getDistance(void) const noexcept
{
    return distance;
}

int ADJACENCY_MATRIX_INITIALIZER::ADJACENCY_MATRIX_INITIALIZER_NODE::getDuration(void) const noexcept
{
    return duration;
}

char *ADJACENCY_MATRIX_INITIALIZER::ADJACENCY_MATRIX_INITIALIZER_NODE::getStart(void) const noexcept
{
    return start;
}

char *ADJACENCY_MATRIX_INITIALIZER::ADJACENCY_MATRIX_INITIALIZER_NODE::getDestination(void) const noexcept
{
    return destination;
}

void ADJACENCY_MATRIX_INITIALIZER::ADJACENCY_MATRIX_INITIALIZER_NODE::searchIsolatedVertices(void)
{
    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
    {
        bool isolated = true;
        for (unsigned int j = 0; j < VERTEX_COUNT && isolated; j++)
            if (adjacencyMatrix[i][j].distance > 0)
                isolated = false;

        if (isolated)
            isolatedVertex[i] = true;
    }
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