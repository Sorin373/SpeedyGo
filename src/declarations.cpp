#include "../include/declarations.hpp"

#include <iostream>
#include <iomanip>
#include <string>

ADJACENCY_MATRIX_INITIALIZER adjacency_matrix_init;
DEPOT   depot;
CITY    city;
PRODUCT product;
ROUTE   route;
SUPPLY  supply;

#ifdef _WIN32
HANDLE                     hConsole;
CONSOLE_SCREEN_BUFFER_INFO csbi;
WORD                       originalAttributes;
#endif

std::vector<std::vector<UNDIRECTED_GRAPH>> adjacencyMatrix(N, std::vector<UNDIRECTED_GRAPH>(N, {0.0, 0}));
std::vector<bool> centralDepos(N, false);
std::vector<bool> limitedStockCities(adjacencyMatrix.size(), false);
std::vector<bool> isolatedVertex(adjacencyMatrix.size(), false);
std::vector<bool> oneEdgeVertex(adjacencyMatrix.size(), false);
std::vector<int>  stack(adjacencyMatrix.size() * adjacencyMatrix.size());
std::vector<int>  minimumRouteTSP(adjacencyMatrix.size() * (adjacencyMatrix.size() - 1) / 2);

unsigned int logCounter;
int    VERTEX_COUNT, limited_stock_cities_count, stackCounter, TSP_RouteCounter, consolePage = 1, ERROR_CNT;
bool   dijkstraRoutesCalculated = false, TSP_RoutesCompleted = false, buffer = true, _ENABLE_API = false;
double minimumDistanceCostTSP = INT_MAX, minimumDurationCostTSP = INT_MAX, traveledDistanceTSP, elapsedDurationTSP, totalSupplyCost, totalSuppliedQuantity;
char   *newCityName = (char *)malloc(MAXL * sizeof(char) + 1);
int    maxProductNameLength, maxCityNameLength, maxProductCategoryLength, maxProductPriceLength, maxCityIDLength, maxCityLatitudeLength,
       maxProductIDLength, maxProductQuantityLength;

int _strcasecmp_(const char *str1, const char *str2)
{
    return STRCASECMP(str1, str2);
}

void underline(const unsigned int vWidth, const bool bSetw)
{
    if (bSetw)
        std::cout << std::setw(5 - 2) << " ";
    char fillLine;
    fillLine = std::cout.fill('_');
    std::cout.width(vWidth);
    std::cout << '_' << "\n";
    std::cout.fill(fillLine);
    std::cout << "\n";
}

#ifdef _WIN32
void changeText(WORD attributes)
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    SetConsoleTextAttribute(hConsole, attributes);
}

void resetText()
{
    SetConsoleTextAttribute(hConsole, originalAttributes);
}
#endif

#ifdef __linux__
char _getch(void)
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    printf("%c\n", buf);
    return buf;
}
#endif

void clear_screen(void)
{
#ifdef _WIN32
    system("CLS");
#elif __linux__
    if (system("clear"))
    {
    };
#endif
}