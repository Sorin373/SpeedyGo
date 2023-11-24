/*
 *
 *  ROUTE_HPP
 *  Route (Dijkstra) management
 *
 *  This C++ header file defines the Route class which stores the data calculated by the Dijkstra alghorithm
 *
 */

#pragma once
#ifndef __ROUTE_HPP__
#define __ROUTE_HPP__

#include <vector>

class ROUTE
{
public:
    struct ROUTE_NODE
    {
    private:
        int start = 0, destination = 0;
        double distance = 0.0;
        std::vector<int> route;

    public:
        ROUTE_NODE *prev = nullptr, *next = nullptr;

        ROUTE_NODE(const int start, const int destination, const double distance, std::vector<int> &route);
        ~ROUTE_NODE();

        int getStart(void) const noexcept;
        int getDestination(void) const noexcept;
        double getDistance(void) const noexcept;
        std::vector<int> getRoute(void) const noexcept;
    };

    ROUTE_NODE *head_route = nullptr, *tail_route = nullptr;

public:
    ROUTE_NODE *getHead() const noexcept;
    ROUTE_NODE *getTail() const noexcept;
    void getData(const int start, const int destination, const double distance, std::vector<int> &route);
    ~ROUTE();
};

#endif // ROUTE_HPP