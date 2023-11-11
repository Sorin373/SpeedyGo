/*
 * DIJKSTRA_HPP
 *
 * This C++ header file implements the Dijkstra algorithm for finding the shortest paths
 * in a weighted graph.
 *
 * Usage:
 * - To use the Dijkstra algorithm for city-to-city distance calculation, call the
 *   static method CityToCityDistanceCalculator().
 *
 */

#pragma once
#ifndef __DIJKSTRA_HPP__
#define __DIJKSTRA_HPP__

#include <vector>

class Dijkstra
{
private:
    static void generateDistanceSolution(const int start, std::vector<double> &distance, std::vector<int> &pathVector, bool debug, bool createRoutes);
    static void dijkstra(const int start, std::vector<double> &distance, std::vector<int> &pathVector);
    static void PrintCityToCityOptimalRoutes(const int _ID, const int start);
    static void DisplayCityToCityOptimalRoutes(const int start);

public:
    static void CityToCityDistanceCalculator(void);
};

#endif // DIJKSTRA_HPP