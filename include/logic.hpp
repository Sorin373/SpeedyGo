/*
*
*  LOGIC_HPP
*  Application Logic and Algorithms
*
*  This C++ header file has the core function prototypes for the SpeedyGo application.
*
*  Contents:
*  - Namespace 'speedyGo': Encapsulates MySQL credential validation, adjacency matrix initialization, and console operations.
*  - Namespace 'Dijkstra': Dijkstra's shortest path calculations and distance solutions.
*  - Namespace 'tsp': Solving Traveling Salesman Problems (TSP) with Hamiltonian and acyclic graph approaches.
*
*/

#pragma once
#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <vector>

namespace speedyGo
{
    bool validateMysqlCredentials(void); // Check MySQL database credentials
    bool _ADJACENCY_MATRIX_INIT_(void);  // Init graph
    void displaySpeedyGoDetails(void);
    void MySQLconsole(void);             // Console to write SQL
}

void alignConsoleOutput(void);
void limitedStockProductSearchByID(const int ProductID);
void connectedNodes(const int DepotID);
void selectDepotType(void);
void searchLimitedStockCities(void);
void searchIsolatedVertices(void);
void stockStatusVisualization(void);
void printCentralDepots(void);

namespace Dijkstra
{
    void generateDistanceSolution(const int start, std::vector<double> &distanta, std::vector<int> &distanta_minima, bool afisare, bool creare_trasee);
    void dijkstra(const int start, std::vector<double> &distanta, std::vector<int> &distanta_minima);
    void PrintCityToCityOptimalRoutes(const int _ID, const int vStart);
    void DisplayCityToCityOptimalRoutes(const int vStart);
    void CityToCityDistanceCalculator(void);
}

namespace tsp // I used backtracking to solve TSP (it is limited based on the no. of edges available) (I tried to cover as many possible outcomes as I could find)
{
    namespace hamiltonianGraph
    {
        void initStack(void);
        bool successor(void);
        bool solution(void);
        bool valid(void);
        void determiningMinimumHamiltonianCycle(void);
        void back(void);
    }

    namespace acyclicGraph
    {
        void initStack(void);
        bool successor(void);
        bool solution(void);
        bool valid(void);
        void determiningMinimumRoute(void);
        void back(void);
    }

    void TSP(void);
    void productTransportTSP(void);
    void mainPageTSP(void);
    void finalPageTSP(void);
    void leftPageTSP(void);
    void rightPageTSP(void);
    void routeTraversalTSP(void);
}

#endif // LOGIC_HPP
