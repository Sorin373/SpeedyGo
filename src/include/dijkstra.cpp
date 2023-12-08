#include "../../include/dijkstra.hpp"
#include "../../include/declarations.hpp"

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <string>

void Dijkstra::generateDistanceSolution(const int start, std::vector<double> &distance, std::vector<int> &pathVector, bool debug, bool createRoutes)
{
    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
    {
        if (i != start)
        {
            if (debug) // Debugging option
                std::cout << "The shortest distance from " << start << " to " << i << " is: " << distance[i] << " : optimalRoute: ";

            std::vector<int> optimalRoute;
            int node = i;

            while (node != -1)
            {
                optimalRoute.push_back(node);
                node = pathVector[node];
            }

            reverse(optimalRoute.begin(), optimalRoute.end());

            if (createRoutes)
                route.getData(start, i, distance[i], optimalRoute);

            if (debug) // Debugging option
                for (unsigned int j = 0; j < optimalRoute.size(); j++)
                    std::cout << optimalRoute[j] << " ";

            if (debug) // Debugging option
                std::cout << "\n";
        }
    }
}

void Dijkstra::dijkstra(const int start, std::vector<double> &distance, std::vector<int> &pathVector)
{
    std::vector<bool> visited(VERTEX_COUNT, false);
    distance[start] = 0.0;

    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
    {
        int min_index = 0;
        double min_dist = std::numeric_limits<double>::infinity();

        for (unsigned int j = 0; j < VERTEX_COUNT; j++)
            if (!visited[j] && distance[j] < min_dist)
            {
                min_index = j;
                min_dist = distance[j];
            }

        visited[min_index] = true;

        for (unsigned int j = 0; j < VERTEX_COUNT; j++)
        {
            double newDistance = distance[min_index] + adjacencyMatrix[min_index][j].distance;

            if (!visited[j] && adjacencyMatrix[min_index][j].distance > 0 && newDistance < distance[j])
            {
                distance[j] = newDistance;
                pathVector[j] = min_index;
            }
        }
    }
}

void Dijkstra::PrintCityToCityOptimalRoutes(const int _ID, const int start)
{
    bool isFound = false;

    if (isolatedVertex[_ID] == true)
    {
        std::cout << "\n"
                  << std::setw(5) << " "
                  << "There is no route with this ID. Isolated depot...";
        return;
    }

    for (ROUTE::ROUTE_NODE *date_traseu = route.getHead(); date_traseu != nullptr; date_traseu = date_traseu->next)
    {
        if (date_traseu->getDestination() == _ID && date_traseu->getStart() == start)
        {
            isFound = true;
            std::cout << "\n"
                      << std::setw(5) << " "
                      << "Distance: ";
            std::cout << date_traseu->getDistance() << "km\n"
                      << std::setw(5) << " ";

            const std::vector<int> &optimalRoute = date_traseu->getRoute();

            for (unsigned int i = 0; i < optimalRoute.size(); i++)
                for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
                {
                    int ID = std::stoi(city_data->getCityID());

                    if (ID == optimalRoute[i])
                    {
                        std::cout << city_data->getCityName();
                        if (i != optimalRoute.size() - 1)
                            std::cout << " --> ";
                        break;
                    }
                }
            break;
        }
    }

    if (!isFound)
    {
        std::cout << "\n"
                  << std::setw(5) << " "
                  << "There is no route with this ID!";
        return;
    }
}

void Dijkstra::DisplayCityToCityOptimalRoutes(const int start)
{
    clear_screen();

    std::cout << "\n\n"
              << std::setw(5) << " "
              << "Type 'EXIT' to return\n";

    underline(45, true);

    char *city_start = (char *)malloc(MAXL * sizeof(char) + 1);

    if (city_start == NULL)
        return;

    for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
    {
        int ID = std::stoi(city_data->getCityID());
        if (ID == start)
        {
            strcpy(city_start, city_data->getCityName());
            break;
        }
    }

    for (ROUTE::ROUTE_NODE *date_traseu = route.getHead(); date_traseu != nullptr; date_traseu = date_traseu->next)
        if (date_traseu->getStart() == start)
            for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
                if (std::stoi(city_data->getCityID()) == date_traseu->getDestination())
                {
                    std::cout << std::setw(5 + 1)
                              << " [" << city_data->getCityID() << "] " << city_start << " -> " << city_data->getCityName() << "\n";
                    break;
                }

    underline(45, true);

    char *_ID = (char *)malloc(MAXL * sizeof(char) + 1);

    if (_ID == NULL)
    {
        free(city_start);
        return;
    }

    std::cout << std::setw(5) << " "
              << "Menu ID: ";
    std::cin >> _ID;

    if (_strcasecmp_(_ID, "exit") == 0)
    {
        free(_ID);
        free(city_start);
        return;
    }
    else
    {
        free(city_start);

        int _ID_temp = std::stoi(_ID);

        free(_ID);

        PrintCityToCityOptimalRoutes(_ID_temp, start);
        _getch();
        DisplayCityToCityOptimalRoutes(start);
    }
}

void Dijkstra::CityToCityDistanceCalculator(void)
{
    if (!dijkstraRoutesCalculated)
    {
        for (unsigned int i = 0; i < VERTEX_COUNT; i++)
        {
            std::vector<int> pathVector(VERTEX_COUNT, -1);
            std::vector<double> distance(VERTEX_COUNT, std::numeric_limits<double>::infinity());

            if (centralDepos[i])
            {
                dijkstra(i, distance, pathVector);
                if (!dijkstraRoutesCalculated)
                    Dijkstra::generateDistanceSolution(i, distance, pathVector, false, true);
                else
                {
                    dijkstraRoutesCalculated = true;
                    Dijkstra::generateDistanceSolution(i, distance, pathVector, false, false);
                }
            }
            pathVector.clear();
            distance.clear();
        }
        dijkstraRoutesCalculated = true;
    }

    clear_screen();

    CITY::CITY_NODE::printCentralDepots();

    std::cout << std::setw(5) << " "
              << "Type 'EXIT' to return\n\n";

    char *_ID = (char *)malloc(MAXL * sizeof(char) + 1);

    if (_ID == NULL)
        return;

    std::cout << std::setw(5) << " "
              << "City ID: ";
    std::cin >> _ID;

    if (_strcasecmp_(_ID, "exit") == 0)
    {
        free(_ID);
        return;
    }
    else
    {       
        int _ID_temp = std::stoi(_ID);

        if (centralDepos[_ID_temp])
        {
            free(_ID);
            DisplayCityToCityOptimalRoutes(_ID_temp);
            CityToCityDistanceCalculator();
        }
        else
        {
            std::cout << "\n"
                      << std::setw(5) << " "
                      << "No central depot with this ID exists...";
            free(_ID);
            _getch();
            CityToCityDistanceCalculator();
        }
    }
}