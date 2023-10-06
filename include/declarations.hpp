#pragma once
#ifndef DECLARATIONS_HPP
#define DECLARATIONS_HPP

#include "classes/supply.hpp"
#include "classes/authentification.hpp"
#include "classes/depot.hpp"
#include "classes/product.hpp"
#include "classes/city.hpp"
#include "classes/DistanceHandler.hpp"
#include "classes/route.hpp"

#include <vector>
#include <cstring>
#include <limits.h>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_error.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#define STRCASECMP _stricmp
#elif __linux__
#define STRCASECMP strcasecmp
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <mysql_error.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#endif

#define M_PI 3.14159265358979323846
#define MAX_SIZE 32
#define MAXL 256
#define MINIMUM_STOCK_VALUE 5
#define MAXIMUM_STOCK_VALUE 50
#define EARTH_RADIUS_KM 6371.0

constexpr int N = 1001;

typedef struct GRAPH
{
    double distance;
    int duration;
} UNDIRECTED_GRAPH;

extern ADJACENCY_MATRIX_INITIALIZER adjacency_matrix_init;
extern DEPOT depot;
extern CITY city;
extern PRODUCT product;
extern ROUTE route;
extern SUPPLY supply;

#ifdef _WIN32
extern HANDLE hConsole;
extern CONSOLE_SCREEN_BUFFER_INFO csbi;
extern WORD originalAttributes;
#endif

extern std::vector<std::vector<UNDIRECTED_GRAPH>> adjacencyMatrix;
extern std::vector<bool> centralDepos;
extern std::vector<bool> limitedStockCities;
extern std::vector<bool> isolatedVertex;
extern std::vector<bool> oneEdgeVertex;
extern std::vector<int> stack;
extern std::vector<int> minimumRouteTSP;

extern long long unsigned int logCounter;
extern int VERTEX_COUNT, limited_stock_cities_count, stackCounter, TSP_RouteCounter, consolePage, ERROR_CNT;
extern bool dijkstraRoutesCalculated, TSP_RoutesCompleted, buffer, _ENABLE_API;
extern double minimumDistanceCostTSP, minimumDurationCostTSP, traveledDistanceTSP, elapsedDurationTSP, totalSupplyCost, totalSuppliedQuantity;
extern char *newCityName;

extern int maxProductNameLength, maxCityNameLength, maxProductCategoryLength, maxProductPriceLength, maxCityIDLength, maxCityLatitudeLength,
    maxProductIDLength, maxProductQuantityLength;

extern int _strcasecmp_(const char *str1, const char *str2);
extern void underline(const unsigned int vWidth, const bool bSetw);
#ifdef _WIN32
extern void changeText(WORD attributes);
extern void resetText();
#endif
#ifdef __linux__
extern char _getch(void);
#endif
extern void clear_screen(void);

#endif