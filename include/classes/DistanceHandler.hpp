#pragma once
#ifndef DISTANCE_HANDLER_HPP
#define DISTANCE_HANDLER_HPP

class ADJACENCY_MATRIX_INITIALIZER
{
public:
    struct ADJACENCY_MATRIX_INITIALIZER_NODE
    {
        double distance = 0.0;
        int duration = 0;
        char *start = nullptr;
        char *destination = nullptr;
        ADJACENCY_MATRIX_INITIALIZER_NODE *prev = nullptr;
        ADJACENCY_MATRIX_INITIALIZER_NODE *next = nullptr;

        ADJACENCY_MATRIX_INITIALIZER_NODE(const double distance, const int duration, const char *start, const char *destination);
        ~ADJACENCY_MATRIX_INITIALIZER_NODE();
        double getDistance(void);
        int getDuration(void);
        char *getStart(void);
        char *getDestination(void);
    };

    ADJACENCY_MATRIX_INITIALIZER_NODE *head = nullptr;
    ADJACENCY_MATRIX_INITIALIZER_NODE *tail = nullptr;

public:
    ADJACENCY_MATRIX_INITIALIZER_NODE *getHead(void);
    ADJACENCY_MATRIX_INITIALIZER_NODE *getTail(void);
    void getData(const double distance, const int duration, const char *start, const char *destination);
    ~ADJACENCY_MATRIX_INITIALIZER();
};

#endif