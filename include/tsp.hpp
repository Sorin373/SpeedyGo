/*
 * TSP_HPP
 *
 * This C++ header file implements a Traveling Salesman Problem (TSP) solver using a
 * backtracking algorithm designed by the author.
 *
 * It includes two separate implementations for Hamiltonian graphs and acyclic graphs.
 *
 * Note:
 * - The algorithm is limited based on the number of edges available.
 */

#pragma once
#ifndef __TSP_HPP__
#define __TSP_HPP__

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

    void TSP(void); // Execute the TSP algorithm
    void productTransportTSP(void);
    void mainPageTSP(void);
    void finalPageTSP(void);
    void leftPageTSP(void);
    void rightPageTSP(void);
    void routeTraversalTSP(void); // Traverse the route found by the TSP algorithm
}

#endif // TSP_HPP
