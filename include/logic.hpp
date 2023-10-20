#pragma once
#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <vector>

namespace speedyGo
{
    bool validateMysqlCredentials(void);
    bool _ADJACENCY_MATRIX_INIT_(void);
    void displaySpeedyGoDetails(void);
    void MySQLconsole(void);
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

extern void afisare_depozite_izolate(void);
extern void afisare_depozite_unic_drum(void);

namespace tsp
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
