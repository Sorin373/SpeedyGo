/*
 *
 *  DECLARATIONS_HPP
 *  Global Declarations and Header Includes
 *
 *  This C++ header file contains the main global varibles used in the codebase.
 *  It ensures that essential data is properly organized and accessible.
 *
 */

#pragma once
#ifndef __DECLARATIONS_HPP__
#define __DECLARATIONS_HPP__

#include "classes/supply.hpp"
#include "classes/authentification.hpp"
#include "classes/depot.hpp"
#include "classes/product.hpp"
#include "classes/city.hpp"
#include "classes/DistanceHandler.hpp"
#include "classes/route.hpp"

#include <vector>
#include <string.h>
#include <limits.h>
#include <limits>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#define STRCASECMP _stricmp
#elif __linux__
#define STRCASECMP strcasecmp
#include <unistd.h>
#include <termios.h>
#endif

#define TSP_LOG             "logs/TSP_log.txt"         // keeps track of routes
#define LOGS                "utils/contor_TSP_log.txt" // keeps track of the amount of logs in "logs/TSP_log.txt"
#define EDGES               "utils/edges.txt"          // Edges between depots ()
#define API                 "utils/config.json"        // Google Matrix API location
#define M_PI                3.14159265358979323846
#define MAX_SIZE            32
#define MAXL                256
#define MINIMUM_STOCK_VALUE 5                          // Condition to restock a depot
#define MAXIMUM_STOCK_VALUE 50                         // This is the value based on which the product quantities are calculated when restocking
#define EARTH_RADIUS_KM     6371.0

constexpr int N = 1001;                                // No. of vertices (depots)

typedef struct GRAPH
{
    double distance;
    int duration;
} UNDIRECTED_GRAPH;

extern ADJACENCY_MATRIX_INITIALIZER adjacency_matrix_init;
extern DEPOT   depot;
extern CITY    city;
extern PRODUCT product;
extern ROUTE   route;
extern SUPPLY  supply;

#ifdef _WIN32
extern HANDLE                     hConsole;
extern CONSOLE_SCREEN_BUFFER_INFO csbi;
extern WORD                       originalAttributes;
#endif

extern std::vector<std::vector<UNDIRECTED_GRAPH>> adjacencyMatrix; // Adjacency matrix
extern std::vector<bool> centralDepos;                             // Storage for central depots' IDs
extern std::vector<bool> limitedStockCities;                       // Storage for limited stock depots' IDs
extern std::vector<bool> isolatedVertex;                           // Storage for isolated depots' IDs
extern std::vector<bool> oneEdgeVertex;                            // Storage for depots with a unique connection to the graph
extern std::vector<int>  stack;                                    // Stack used in backtracking algorithms to determine the optimal route
extern std::vector<int>  minimumRouteTSP;                          // Storage for the optimal TSP (Traveling Salesman Problem) route

extern unsigned int logCounter;
extern int    VERTEX_COUNT, limited_stock_cities_count, stackCounter, TSP_RouteCounter, consolePage, ERROR_CNT;
extern bool   dijkstraRoutesCalculated, TSP_RoutesCompleted, buffer, _ENABLE_API, __DATABASE_RETRIEVED, __MySQL_CONNECTED;
extern double minimumDistanceCostTSP, minimumDurationCostTSP, traveledDistanceTSP, elapsedDurationTSP, totalSupplyCost, totalSuppliedQuantity;
extern char   *newCityName;

// Variables used for precise console output alignment
extern int maxProductNameLength, maxCityNameLength, maxProductCategoryLength, maxProductPriceLength, maxCityIDLength, maxCityLatitudeLength,
           maxProductIDLength, maxProductQuantityLength;

int  _strcasecmp_(const char *str1, const char *str2);        // Cross platform case-insensitive string comparison function
void underline(const unsigned int vWidth, const bool bSetw);  // I am using this custom function to style the output in the console window
#ifdef _WIN32
void changeText(WORD attributes);
void resetText();
#endif
#ifdef __linux__
char _getch(void);
#endif
void clear_screen(void); 

#endif // DECLARATIONS_HPP