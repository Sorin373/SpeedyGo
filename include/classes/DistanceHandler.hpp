/*
 *
 *  DISTANCE_HANDLER_HPP
 *
 *  This C++ header file defines the graph init class
 *  It holds the data which will later be used to init the graph. See in logic.cpp (speedyGo::_ADJACENCY_MATRIX_INIT_)
 *  This object is filled with data in GoogleMatrixAPI.cpp.
 *
 */

#pragma once
#ifndef __DISTANCE_HANDLER_HPP__
#define __DISTANCE_HANDLER_HPP__

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

        double getDistance(void) const noexcept;
        int getDuration(void) const noexcept;
        char *getStart(void) const noexcept;
        char *getDestination(void) const noexcept;

        static void searchIsolatedVertices(void);
    };

    ADJACENCY_MATRIX_INITIALIZER_NODE *head = nullptr, *tail = nullptr;

public:
    ADJACENCY_MATRIX_INITIALIZER_NODE *getHead(void) const noexcept;
    ADJACENCY_MATRIX_INITIALIZER_NODE *getTail(void) const noexcept;
    void getData(const double distance, const int duration, const char *start, const char *destination);
    ~ADJACENCY_MATRIX_INITIALIZER();
};

#endif // DISTANCE_HANDLER_HPP