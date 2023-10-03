#include "../../include/classes/route.hpp"
#include <cstring>
#include <vector>

ROUTE::ROUTE_NODE::ROUTE_NODE(const int start, const int destination, const double distance, std::vector<int> &route)
{
    this->start = start;
    this->destination = destination;
    this->distance = distance;
    this->route.assign(route.begin(), route.end());
}

ROUTE::ROUTE_NODE::~ROUTE_NODE()
{
    route.clear();
}

ROUTE::ROUTE_NODE *ROUTE::getHead(void)
{
    return head_route;
}

ROUTE::ROUTE_NODE *ROUTE::getTail(void)
{
    return tail_route;
}

int ROUTE::ROUTE_NODE::getStart(void)
{
    return start;
}

int ROUTE::ROUTE_NODE::getDestination(void)
{
    return destination;
}

double ROUTE::ROUTE_NODE::getDistance(void)
{
    return distance;
}

std::vector<int> ROUTE::ROUTE_NODE::getRoute(void)
{
    return route;
}

void ROUTE::getData(const int start, const int destination, const double distance, std::vector<int> &route)
{
    ROUTE_NODE *newnod = new ROUTE_NODE(start, destination, distance, route);

    if (head_route == nullptr)
    {
        head_route = newnod;
        tail_route = newnod;
    }
    else
    {
        tail_route->next = newnod;
        newnod->prev = tail_route;
        tail_route = newnod;
    }
}

ROUTE::~ROUTE()
{
    ROUTE_NODE *ptr = head_route;

    while (ptr != nullptr)
    {
        ROUTE_NODE *temp = ptr;
        ptr = ptr->next;
        delete temp;
    }
}