#pragma once
#ifndef DISTANCE_HANDLER_HPP
#define DISTANCE_HANDLER_HPP

class ADJACENCY_MATRIX_INITIALIZER
{
public:
    struct ADJACENCY_MATRIX_INITIALIZER_NODE
    {
    private:
        char *start = nullptr, *destination = nullptr;
        double distance = 0.0;
        int duration = 0;

    public:
        ADJACENCY_MATRIX_INITIALIZER_NODE *prev = nullptr, *next = nullptr;

        ADJACENCY_MATRIX_INITIALIZER_NODE(const double distance, const int duration, const char *start, const char *destination);
        ~ADJACENCY_MATRIX_INITIALIZER_NODE();

        double getDistance(void) const;
        int getDuration(void) const;
        char *getStart(void) const;
        char *getDestination(void) const;
    };

    ADJACENCY_MATRIX_INITIALIZER_NODE *head = nullptr, *tail = nullptr;

public:
    ADJACENCY_MATRIX_INITIALIZER_NODE *getHead(void) const;
    ADJACENCY_MATRIX_INITIALIZER_NODE *getTail(void) const;
    void getData(const double distance, const int duration, const char *start, const char *destination);
    ~ADJACENCY_MATRIX_INITIALIZER();
};

#endif