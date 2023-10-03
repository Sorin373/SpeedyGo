#pragma once
#ifndef ROUTES_HPP
#define ROUTES_HPP

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
        ROUTE_NODE *prev = nullptr;
        ROUTE_NODE *next = nullptr;

        ROUTE_NODE(const int start, const int destination, const double distance, std::vector<int> &route);
        ~ROUTE_NODE();
        int getStart(void);
        int getDestination(void);
        double getDistance(void);
        std::vector<int> getRoute(void);
    };

    ROUTE_NODE *head_route = nullptr;
    ROUTE_NODE *tail_route = nullptr;

public:
    ROUTE_NODE *getHead();
    ROUTE_NODE *getTail();
    void getData(const int start, const int destination, const double distance, std::vector<int> &route);
    ~ROUTE();
};

#endif