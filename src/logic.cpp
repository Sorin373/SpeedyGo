#include "../include/logic.hpp"
#include "../include/declarations.hpp"
#include "../include/database.hpp"
#include "../include/GoogleMatrixAPI.hpp"
#include "../include/haversine.hpp"

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>

bool speedyGo::validateMysqlCredentials(void)
{
    clear_screen();

    char *host_name = (char *)malloc(MAXL * sizeof(char) + 1);

    if (host_name == NULL)
        return EXIT_FAILURE;

    char *username = (char *)malloc(MAXL * sizeof(char) + 1);

    if (username == NULL)
    {
        free(host_name);
        return EXIT_FAILURE;
    }

    char *database = (char *)malloc(MAXL * sizeof(char) + 1);

    if (database == NULL)
    {
        free(host_name);
        free(username);
        return EXIT_FAILURE;
    }

    char *password = (char *)malloc(MAXL * sizeof(char) + 1);

    if (password == NULL)
    {
        free(host_name);
        free(username);
        free(database);
        return EXIT_FAILURE;
    }

    std::cout << "\n\n"
              << std::setw(10) << " "
              << "CONEXIUNE LA BAZA DE DATE\n"
              << std::setw(4) << " "
              << "======================================\n"
              << std::setw(5) << " "
              << "Hostname: ";
    std::cin >> host_name;

    std::cout << std::setw(5) << " "
              << "Username: ";
    std::cin >> username;

    std::cout << std::setw(5) << " "
              << "Password: ";
    std::cin >> password;

    std::cout << std::setw(5) << " "
              << "Database name: ";
    std::cin >> database;

    std::cout << std::setw(4) << " "
              << "======================================\n";

    if (strlen(host_name) > MAX_SIZE || strlen(username) > MAX_SIZE || strlen(password) > MAX_SIZE || strlen(database) > MAX_SIZE)
        return EXIT_FAILURE;

    AUTHENTICATION::getData(host_name, username, password, database);

    if (fetchTables() == EXIT_FAILURE)
    {
        _getch();
        free(host_name);
        free(username);
        free(password);
        free(database);
        return validateMysqlCredentials();
    }

    free(host_name);
    free(username);
    free(password);
    free(database);

    return EXIT_SUCCESS;
}

bool speedyGo::_ADJACENCY_MATRIX_INIT_(void)
{
    if (_GOOGLE_MATRIX_API_INIT_() == EXIT_FAILURE)
    {
        std::cout << std::setw(5) << " "
                  << "-- Initialization of the Google API service could not be completed!\n";
        ERROR_CNT++;

        if (_HAVERSINE_INIT_("utils/legaturi.txt") == EXIT_FAILURE)
        {
            std::cout << std::setw(5) << " "
                      << "-- Manual calculation using the Haversine formula was unsuccessful during initialization!\n";
            ERROR_CNT++;

            return EXIT_FAILURE;
        }
    }

    for (ADJACENCY_MATRIX_INITIALIZER::ADJACENCY_MATRIX_INITIALIZER_NODE *date_gps = adjacency_matrix_init.getHead(); date_gps != nullptr; date_gps = date_gps->next)
    {
        char *city1 = (char *)malloc(MAXL * sizeof(char) + 1), *city2;

        if (city1 == NULL)
            return EXIT_FAILURE;

        city2 = (char *)malloc(MAXL * sizeof(char) + 1);

        if (city2 == NULL)
        {
            free(city1);
            return EXIT_FAILURE;
        }

        int city1_ID = 0, city2_ID = 0, durata = date_gps->getDuration();
        double distance = date_gps->getDistance();

        strcpy(city1, date_gps->getStart());
        strcpy(city2, date_gps->getDestination());

        for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
            if (_strcasecmp_(city_data->getCityName(), city1) == 0)
            {
                city1_ID = std::stoi(city_data->getCityID());
                break;
            }

        for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
            if (_strcasecmp_(city_data->getCityName(), city2) == 0)
            {
                city2_ID = std::stoi(city_data->getCityID());
                break;
            }

        adjacencyMatrix[city1_ID][city2_ID].distance = adjacencyMatrix[city2_ID][city1_ID].distance = distance;
        adjacencyMatrix[city1_ID][city2_ID].duration = adjacencyMatrix[city2_ID][city1_ID].duration = durata;

        free(city1);
        free(city2);
    }

    return EXIT_SUCCESS;
}

void alignConsoleOutput(void)
{
    for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
    {
        if (strlen(city_data->getCityName()) > maxCityNameLength)
            maxCityNameLength = strlen(city_data->getCityName());

        char temp[MAXL];
        sprintf(temp, "%f", city_data->getLatitude());

        for (unsigned int i = 0; i < strlen(temp); i++)
            if (temp[i] == '.')
                temp[i] = '\0';

        if (strlen(temp) > maxCityLatitudeLength)
            maxCityLatitudeLength = strlen(temp);
    }

    for (PRODUCT::PRODUCT_NODE *product_data = product.getHead(); product_data != nullptr; product_data = product_data->next)
    {
        if (strlen(product_data->getProductName()) > maxProductNameLength)
            maxProductNameLength = strlen(product_data->getProductName());

        if (strlen(product_data->getProductCategory()) > maxProductCategoryLength)
            maxProductCategoryLength = strlen(product_data->getProductCategory());

        char *pret = (char *)malloc(MAXL * sizeof(char) + 1);

        if (pret == NULL)
            return;

        sprintf(pret, "%f", product_data->getProductPrice());

        for (unsigned int i = 0; i < strlen(pret); i++)
            if (pret[i] == '.')
                pret[i] = '\0';

        if (strlen(pret) > maxProductPriceLength)
            maxProductPriceLength = strlen(pret);

        if (strlen(product_data->getProductID()) > maxProductIDLength)
            maxProductIDLength = strlen(product_data->getProductID());

        free(pret);
    }

    for (DEPOT::DEPOT_NODE *depot_data = depot.getHead(); depot_data != nullptr; depot_data = depot_data->next)
    {
        int cantitiate_temp = depot_data->getProductQuantity(), contor_cifre = 0;

        while (cantitiate_temp > 0)
        {
            cantitiate_temp /= 10;
            contor_cifre++;
        }
        if (contor_cifre > maxProductQuantityLength)
            maxProductQuantityLength = contor_cifre;
    }

    int temp = VERTEX_COUNT;
    while (temp)
    {
        temp /= 10;
        maxCityIDLength++;
    }

    maxCityLatitudeLength += 3;
    maxProductPriceLength += 3;
}

void limitedStockProductSearchByID(const int ProductID)
{
    clear_screen();

    std::cout << "\n\n"
              << std::setw(5) << " "
              << "Product_ID" << std::setw(3) << " "
              << "Product_Name" << std::setw(11) << " "
              << "Product_Quantity\n";
    underline(55, true);

    for (PRODUCT::PRODUCT_NODE *product_data = product.getHead(); product_data != nullptr; product_data = product_data->next)
    {
        int PRODUCT_ID = std::stoi(product_data->getProductID());

        for (DEPOT::DEPOT_NODE *depot_data = depot.getHead(); depot_data != nullptr; depot_data = depot_data->next)
            if (std::stoi(depot_data->getProductID()) == PRODUCT_ID && std::stoi(depot_data->getCityID()) == ProductID)
                if (depot_data->getProductQuantity() < MINIMUM_STOCK_VALUE)
                    std::cout << std::setw(5 + 1) << " [" << product_data->getProductID() << "]" << std::setw(maxProductIDLength - strlen(product_data->getProductID()) + 9)
                              << " " << product_data->getProductName()
                              << std::setw(maxProductNameLength - strlen(product_data->getProductName()) + 3) << " " << depot_data->getProductQuantity()
                              << std::setw(maxProductQuantityLength - std::to_string(round(product_data->getProductPrice())).length() + 5) << " "
                              << "buc.\n";
    }

    underline(55, true);

    std::cout << std::setw(5) << " "
              << "Press 'ENTER' to return...";
}

void connectedNodes(const int DepotID)
{
    std::cout << "\n";

    std::vector<bool> depots(VERTEX_COUNT, false);
    CITY::CITY_NODE *city_data = city.getHead();
    char *depotName = (char *)malloc(MAXL * sizeof(char) + 1);

    if (depotName == NULL)
        return;

    while (city_data != nullptr)
    {
        if (std::stoi(city_data->getCityID()) == DepotID)
            strcpy(depotName, city_data->getCityName());
        city_data = city_data->next;
    }

    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
        if (adjacencyMatrix[DepotID][i].distance != 0)
            depots[i] = true;

    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
        if (depots[i] == true)
        {
            city_data = city.getHead();
            while (city_data != nullptr)
            {
                if (std::stoi(city_data->getCityID()) == i && _strcasecmp_(depotName, city_data->getCityName()) != 0)
                {
                    std::cout << std::setw(5) << " " << depotName << " -> " << city_data->getCityName()
                              << std::setw(maxCityNameLength - strlen(city_data->getCityName()) + 5)
                              << " (" << adjacencyMatrix[DepotID][i].distance << "km | " << adjacencyMatrix[DepotID][i].duration << "min)\n";
                    break;
                }
                city_data = city_data->next;
            }
        }

    std::cout << "\n"
              << std::setw(5) << " "
              << "Press 'ENTER' to return...";

    free(depotName);
}

void selectDepotType(void)
{
    CITY::CITY_NODE *city_data = city.getHead();
    while (city_data != nullptr)
    {
        if (_strcasecmp_(city_data->getCityType(), "centralizat") == 0)
            centralDepos[std::stoi(city_data->getCityID())] = true;
        city_data = city_data->next;
    }
}

void searchLimitedStockCities(void)
{
    limited_stock_cities_count = 0;
    CITY::CITY_NODE *city_data = city.getHead();
    while (city_data != nullptr)
    {
        int _ID_Oras = std::stoi(city_data->getCityID());

        DEPOT::DEPOT_NODE *depot_data = depot.getHead();
        while (depot_data != nullptr)
        {
            int _ID_Depozit = std::stoi(depot_data->getCityID());
            if (_ID_Depozit == _ID_Oras && centralDepos[_ID_Depozit] == false)
                if (depot_data->getProductQuantity() < MINIMUM_STOCK_VALUE)
                {
                    limitedStockCities[_ID_Depozit] = true;
                    limited_stock_cities_count++;
                    break;
                }
            depot_data = depot_data->next;
        }
        city_data = city_data->next;
    }
}

void searchIsolatedVertices(void)
{
    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
    {
        bool isolated = true;
        for (unsigned int j = 0; j < VERTEX_COUNT && isolated; j++)
            if (adjacencyMatrix[i][j].distance > 0)
                isolated = false;

        if (isolated)
            isolatedVertex[i] = true;
    }
}

void stockStatusVisualization(void)
{
    searchLimitedStockCities();

    if (limited_stock_cities_count == 0)
    {
        std::cout << "\n"
                  << std::setw(5) << " "
                  << "No supply needed for depots...";
        _getch();
        return;
    }

    clear_screen();

    std::cout << "\n\n"
              << std::setw(5) << " "
              << "+-------------------+\n"
              << std::setw(6) << " "
              << "Limited stock cities\n"
              << std::setw(5) << " "
              << "+-------------------+\n";

    underline(40, true);

    CITY::CITY_NODE *city_data = city.getHead();
    int cmax = 0;

    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
        if (limitedStockCities[i])
        {
            city_data = city.getHead();
            while (city_data != nullptr)
            {
                int _ID = std::stoi(city_data->getCityID());
                if (_ID == i)
                    std::cout << std::setw(5 + 1) << " [" << city_data->getCityID() << "] " << city_data->getCityName()
                              << std::setw(maxCityNameLength - strlen(city_data->getCityName()) + 5) << " ";
                city_data = city_data->next;
            }
            std::cout << "\n";
        }

    underline(40, true);

#ifdef _WIN32
    changeText(FOREGROUND_INTENSITY);
    std::cout << std::setw(5) << " "
              << "Scrieti 'exit' pentru a iesi\n\n";
    resetText();
#elif __linux__
    std::cout << std::setw(5) << " "
              << "\033[3m"
              << "Scrieti 'exit' pentru a iesi\n\n"
              << "\033[0m";
#endif

    char *t_ID_Oras = (char *)malloc(MAXL * sizeof(char) + 1);
    std::cout << std::setw(5) << " "
              << "Introduceti ID-ul orasului: ";
    std::cin >> t_ID_Oras;

    if (_strcasecmp_(t_ID_Oras, "exit") == 0)
    {
        free(t_ID_Oras);
        return;
    }
    else
    {
        int _ID_Oras = std::stoi(t_ID_Oras);
        free(t_ID_Oras);
        city_data = city.getHead();

        while (city_data != nullptr)
        {
            int t_ID = std::stoi(city_data->getCityID());
            if (t_ID == _ID_Oras && limitedStockCities[t_ID] == true)
            {
                unsigned int sMENIU;

                do
                {
                    clear_screen();

                    std::cout << "\n\n"
                              << std::setw(5) << " "
                              << "+--------------------------------------------+\n"
                              << std::setw(7) << " " << city_data->getCityName() << " | Tip depot: " << city_data->getCityName() << "\n"
                              << std::setw(5) << " "
                              << "+--------------------------------------------+\n";

                    underline(50, true);

                    std::cout << std::setw(5) << " "
                              << "[1] Vizualizare produse cu stoc limitat\n"
                              << std::setw(5) << " "
                              << "[2] Vizualizare conexiuni cu alte depozite\n"
                              << std::setw(5) << " "
                              << "[0] Inapoi\n";

                    underline(50, true);

                    std::cout << std::setw(5) << " "
                              << "Enter menu number: ";
                    std::cin >> sMENIU;

                    switch (sMENIU)
                    {
                    case 1:
                        limitedStockProductSearchByID(_ID_Oras);
                        _getch();
                        break;
                    case 2:
                        connectedNodes(_ID_Oras);
                        _getch();
                        break;

                    default:
                        break;
                    }

                } while (sMENIU != 0);

                break;
            }
            city_data = city_data->next;
        }
        stockStatusVisualization();
    }
}

void Dijkstra::generateDistanceSolution(const int start, std::vector<double> &distance, std::vector<int> &pathVector, bool debug, bool createRoutes)
{
    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
    {
        if (i != start)
        {
            if (debug) // Debugging option
                std::cout << "The shortest distance from " << start << " to " << i << " is: " << distance[i] << " : optimalRoute: ";

            std::vector<int> optimalRoute;
            int nod = i;

            while (nod != -1)
            {
                optimalRoute.push_back(nod);
                nod = pathVector[nod];
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

void printCentralDepots(void)
{
        std::cout << "\n\n"
              << std::setw(5) << " "
              << "+----------------+\n"
              << std::setw(5) << " "
              << "| Central depots |\n"
              << std::setw(5) << " "
              << "+----------------+\n";
    underline(40, true);

    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
        if (centralDepos[i])
            for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
                if (std::stoi(city_data->getCityID()) == i)
                {
                    std::cout << std::setw(5 + 1) << " [" << std::stoi(city_data->getCityID()) << "] " << city_data->getCityName() << "\n";
                    break;
                }

    underline(40, true);
}

void Dijkstra::PrintCityToCityOptimalRoutes(const int _ID, const int start)
{
    bool gasit = false;

    if (isolatedVertex[_ID] == true)
    {
        std::cout << "\n"
                  << std::setw(5) << " "
                  << "Nu exista traseul cu acest ID. Depozit izolat...";
        return;
    }

    for (ROUTE::ROUTE_NODE *date_traseu = route.getHead(); date_traseu != nullptr; date_traseu = date_traseu->next)
    {
        if (date_traseu->getDestination() == _ID && date_traseu->getStart() == start)
        {
            gasit = true;
            std::cout << "\n"
                      << std::setw(5) << " "
                      << "Distanta: ";
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

    if (!gasit)
    {
        std::cout << "\n"
                  << std::setw(5) << " "
                  << "Nu exista traseul cu acest ID!";
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

    printCentralDepots();

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

void afisare_depozite_izolate(void)
{
    clear_screen();

    std::cout << "\n\n"
              << std::setw(5) << " "
              << "+------------------+\n"
              << std::setw(5) << " "
              << "| DEPOZITE IZOLATE |\n"
              << std::setw(5) << " "
              << "+------------------+\n\n";

    std::cout << std::setw(5) << " "
              << "ID_Oras"
              << std::setw(5) << " "
              << "Denumire_Oras"
              << std::setw(5) << " "
              << "Tip_Depozit"
              << std::setw(5) << " "
              << "Latitudine"
              << std::setw(5) << " "
              << "Longitudine\n";

    underline(75, true);

    int cmax = -1;
    for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
    {
        if (strlen(city_data->getCityName()) > cmax)
            cmax = strlen(city_data->getCityName());
    }

    bool gasit = false;
    for (unsigned int i = 0; i < isolatedVertex.size(); i++)
        if (isolatedVertex[i] == true)
        {
            gasit = true;
            for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
            {
                int ID = std::stoi(city_data->getCityID());
                if (ID == i)
                {
                    std::cout << std::setw(5 + 1) << " [" << city_data->getCityID() << "]" << std::setw(maxCityIDLength - strlen(city_data->getCityID()) + 8)
                              << " " << city_data->getCityName() << std::setw(maxCityNameLength - strlen(city_data->getCityName()) + 4)
                              << " " << city_data->getCityType() << std::setw(11 - strlen(city_data->getCityType()) + 5)
                              << " " << std::fixed << std::setprecision(2) << city_data->getLatitude();
#ifdef _WIN32
                    std::cout << "\370";
#elif __linux__
                    std::cout << "\u00B0";
#endif
                    std::cout << std::setw(maxCityLatitudeLength - std::to_string(round(city_data->getLatitude())).length() + 13)
                              << " " << city_data->getLongitude();
#ifdef _WIN32
                    std::cout << "\370"
                              << "\n";
#elif __linux__
                    std::cout << "\u00B0\n";
#endif
                    break;
                }
            }
        }

    underline(75, true);

    if (!gasit)
    {
        std::cout << std::setw(5) << " "
                  << "Nu exista depozite izolate...";
        return;
    }
    else
        std::cout << "\n"
                  << std::setw(5) << " "
                  << " Apasati 'ENTER' pentru a iesi...";
}

void afisare_depozite_unic_drum(void)
{
    clear_screen();

    std::cout << "\n\n";
    std::cout << std::setw(5) << " "
              << "+-------------------------------+\n";
    std::cout << std::setw(5) << " "
              << "| DEPOZITE CU O UNICA CONEXIUNE |\n";
    std::cout << std::setw(5) << " "
              << "+-------------------------------+\n\n";

    std::cout << std::setw(5) << " "
              << "ID_Oras"
              << std::setw(5) << " "
              << "Denumire_Oras"
              << std::setw(5) << " "
              << "Tip_Depozit"
              << std::setw(5) << " "
              << "Latitudine"
              << std::setw(5) << " "
              << "Longitudine\n";

    underline(75, true);

    bool gasit = false;
    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
    {
        int contor = 0;
        for (unsigned int j = 0; j < VERTEX_COUNT; j++)
            if (adjacencyMatrix[i][j].distance != 0)
            {
                contor++;
                if (contor >= 2)
                    break;
            }

        if (contor == 1)
        {
            gasit = true;
            oneEdgeVertex[i] = true;
        }
    }

    if (gasit)
    {
        int cmax = -1;
        for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
        {
            if (strlen(city_data->getCityName()) > cmax)
                cmax = strlen(city_data->getCityName());
        }

        for (unsigned int i = 0; i < oneEdgeVertex.size(); i++)
        {
            if (oneEdgeVertex[i])
            {
                for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
                {
                    int ID = std::stoi(city_data->getCityID());
                    if (ID == i)
                    {
                        std::cout << std::setw(5 + 1) << " [" << city_data->getCityID() << "]" << std::setw(maxCityIDLength - strlen(city_data->getCityID()) + 8)
                                  << " " << city_data->getCityName() << std::setw(maxCityNameLength - strlen(city_data->getCityName()) + 4)
                                  << " " << city_data->getCityType() << std::setw(11 - strlen(city_data->getCityType()) + 5)
                                  << " " << std::fixed << std::setprecision(2) << city_data->getLatitude();
#ifdef _WIN32
                        std::cout << "\370";
#elif __linux__
                        std::cout << "\u00B0";
#endif
                        std::cout << std::setw(maxCityLatitudeLength - std::to_string(round(city_data->getLatitude())).length() + 13)
                                  << " " << city_data->getLongitude();
#ifdef _WIN32
                        std::cout << "\370"
                                  << "\n";
#elif __linux__
                        std::cout << "\u00B0\n";
#endif
                        break;
                    }
                }
            }
        }
        underline(75, true);

        std::cout << "\n"
                  << std::setw(5) << " "
                  << " Apasati 'ENTER' pentru a iesi...";
    }
    else
    {
        std::cout << std::setw(5) << " "
                  << "Nu exista depozite cu o unica conexiune...";
        return;
    }
}

void tsp::hamiltonianGraph::initStack(void)
{
    stack[stackCounter] = -1;
}

bool tsp::hamiltonianGraph::successor(void)
{
    if (stack[stackCounter] < VERTEX_COUNT - 1)
    {
        stack[stackCounter]++;
        return true;
    }
    return false;
}

bool tsp::hamiltonianGraph::solution(void)
{
    if (stackCounter == limited_stock_cities_count)
        return true;
    return false;
}

bool tsp::hamiltonianGraph::valid(void)
{
    for (unsigned int i = 1; i < stackCounter; i++)
        if (stack[stackCounter] == stack[i])
            return false;

    if (stackCounter > 1)
        if (adjacencyMatrix[stack[stackCounter]][stack[stackCounter - 1]].distance == 0)
            return false;

    if (stackCounter > 1)
        if (centralDepos[stack[1]] == false)
            return false;

    return true;
}

void tsp::hamiltonianGraph::determiningMinimumHamiltonianCycle(void)
{
    double totalDistance = 0.0;
    int totalDuration = 0;

    for (int i = 1; i <= stackCounter; i++)
    {
        totalDistance += adjacencyMatrix[stack[i]][stack[i + 1]].distance;
        totalDuration += adjacencyMatrix[stack[i]][stack[i + 1]].duration;
    }

    if (totalDistance < minimumDistanceCostTSP || (totalDistance == minimumDistanceCostTSP && totalDuration < minimumDurationCostTSP))
    {
        minimumDistanceCostTSP = totalDistance;
        minimumDurationCostTSP = totalDuration;

        for (int i = 1; i <= stackCounter; i++)
        {
            minimumRouteTSP[i] = stack[i];
            TSP_RouteCounter = stackCounter;
        }
    }
}

void tsp::hamiltonianGraph::back(void)
{
    stackCounter = 1;
    hamiltonianGraph::initStack();
    while (stackCounter > 0)
    {
        int vSuccesor, vValid;
        do
        {
            vSuccesor = hamiltonianGraph::successor();
            if (vSuccesor == 1)
                vValid = hamiltonianGraph::valid();
        } while (vSuccesor == 1 && vValid == 0);
        if (vSuccesor == 1)
        {
            if (hamiltonianGraph::solution() == 1)
                hamiltonianGraph::determiningMinimumHamiltonianCycle();
            else
            {
                stackCounter++;
                hamiltonianGraph::initStack();
            }
        }
        else
            stackCounter--;
    }
}

void tsp::acyclicGraph::initStack(void)
{
    stack[stackCounter] = -1;
}

bool tsp::acyclicGraph::successor(void)
{
    if (stack[stackCounter] < VERTEX_COUNT - 1)
    {
        stack[stackCounter]++;
        return true;
    }
    return false;
}

bool tsp::acyclicGraph::solution(void)
{
    if (stackCounter == VERTEX_COUNT + 1)
        return true;
    return false;
}

bool tsp::acyclicGraph::valid(void)
{
    if (stackCounter == VERTEX_COUNT + 1)
    {
        for (unsigned int i = 0; i < VERTEX_COUNT; i++)
        {
            bool gasit = false;
            if (limitedStockCities[i] == true && !isolatedVertex[i])
            {
                for (unsigned int j = 1; j <= stackCounter; j++)
                {
                    if (stack[j] == i)
                    {
                        gasit = true;
                        break;
                    }
                }

                if (!gasit)
                    return false;
            }
        }
    }

    if (stackCounter > 1)
        if (adjacencyMatrix[stack[stackCounter]][stack[stackCounter - 1]].distance == 0)
            return false;

    if (stackCounter > 1)
        if (centralDepos[stack[1]] == false)
            return false;

    return true;
}

void tsp::acyclicGraph::determiningMinimumRoute(void)
{
    double totalDistance = 0.0;
    int totalDuration = 0;

    for (int i = 1; i < stackCounter; i++)
    {
        totalDistance += adjacencyMatrix[stack[i]][stack[i + 1]].distance;
        totalDuration += adjacencyMatrix[stack[i]][stack[i + 1]].duration;
    }

    if (totalDistance < minimumDistanceCostTSP || (totalDistance == minimumDistanceCostTSP && totalDuration < minimumDurationCostTSP))
    {
        minimumDistanceCostTSP = totalDistance;
        minimumDurationCostTSP = totalDuration;

        for (int i = 1; i <= stackCounter; i++)
        {
            minimumRouteTSP[i] = stack[i];
            TSP_RouteCounter = stackCounter;
        }
    }
}

void tsp::acyclicGraph::back(void)
{
    int vSuccesor, vValid;
    stackCounter = 1;
    acyclicGraph::initStack();
    while (stackCounter > 0)
    {
        do
        {
            vSuccesor = acyclicGraph::successor();
            if (vSuccesor == 1)
                vValid = acyclicGraph::valid();
        } while (vSuccesor == 1 && vValid == 0);
        if (vSuccesor == 1)
            if (acyclicGraph::solution() == 1)
                acyclicGraph::determiningMinimumRoute();
            else
            {
                stackCounter++;
                acyclicGraph::initStack();
            }
        else
            stackCounter--;
    }
}

void tsp::TSP(void)
{
#ifdef _WIN32
    changeText(FOREGROUND_INTENSITY | FOREGROUND_BLUE);
    std::cout << std::setw(5) << " "
              << "Calculating the most optimal route...\n";
    resetText();
#elif __linux__
    std::cout << "\033[1;34m" << std::setw(5) << " "
              << "Calculating the most optimal route..."
              << "\033[0m" << endl;
#endif

    bool vIsolatedVertex = false;
    for (unsigned int i = 0; i < VERTEX_COUNT && !vIsolatedVertex; i++)
        if (isolatedVertex[i] == true)
            vIsolatedVertex = true;

    if (!vIsolatedVertex)
    {
        tsp::hamiltonianGraph::back();
        clear_screen();
        std::cout << "\n";

        if (!minimumRouteTSP.empty())
        {
#ifdef _WIN32
            changeText(FOREGROUND_INTENSITY);
            std::cout << std::setw(5) << " "
                      << "Route length: " << minimumDistanceCostTSP << "km | "
                      << "Route duration: " << minimumDurationCostTSP << "min"
                      << "\n"
                      << std::setw(5) << " ";
            resetText();
#elif __linux__
            std::cout << std::setw(5) << " "
                      << "\033[1m"
                      << "Route length: " << minimumDistanceCostTSP << "km | "
                      << "Route duration: " << minimumDurationCostTSP << "min\n"
                      << std::setw(5) << " "
                      << "\033[0m";
#endif
            for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
                for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
                    if (std::stoi(city_data->getCityID()) == minimumRouteTSP[i])
                    {
                        std::cout << city_data->getCityName();
                        if (i < TSP_RouteCounter)
                            std::cout << " --> ";
                        break;
                    }

            std::cout << "\n";
            underline(190, false);
        }
        else
        {
            minimumRouteTSP.clear();
            stack.clear();
            tsp::acyclicGraph::back(); // generating routes

            clear_screen();
            std::cout << "\n";

            if (!minimumRouteTSP.empty())
            {
#ifdef _WIN32
                changeText(FOREGROUND_INTENSITY);

                std::cout << std::setw(5) << " "
                          << "Route length: " << minimumDistanceCostTSP << "km | "
                          << "Route duration: " << minimumDurationCostTSP << "min"
                          << "\n"
                          << std::setw(5) << " ";

                resetText();
#elif __linux__
                std::cout << std::setw(5) << " "
                          << "\033[1m"
                          << "Route length: " << minimumDistanceCostTSP << "km | "
                          << "Route duration: " << minimumDurationCostTSP << "min\n"
                          << std::setw(5) << " "
                          << "\033[0m";
#endif
                for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
                    for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
                        if (std::stoi(city_data->getCityID()) == minimumRouteTSP[i])
                        {
                            std::cout << city_data->getCityName();
                            if (i < TSP_RouteCounter)
                                std::cout << " --> ";
                            break;
                        }

                std::cout << "\n";
                underline(190, false);
            }
            else
                std::cout << std::setw(5) << " "
                          << "--> All depots are isolated!\n";
        }
    }
    else
    {
        tsp::acyclicGraph::back();
        clear_screen();

        std::cout << "\n";
        if (!minimumRouteTSP.empty())
        {
#ifdef _WIN32
            changeText(FOREGROUND_INTENSITY);

            std::cout << std::setw(5) << " "
                      << "Route length: " << minimumDistanceCostTSP << "km | "
                      << "Route duration: " << minimumDurationCostTSP << "min"
                      << "\n"
                      << std::setw(5) << " ";

            resetText();
#elif __linux__
            std::cout << std::setw(5) << " "
                      << "\033[1m"
                      << "Route length: " << minimumDistanceCostTSP << "km | "
                      << "Route duration: " << minimumDurationCostTSP << "min\n"
                      << std::setw(5) << " "
                      << "\033[0m";
#endif

            for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
                for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
                    if (std::stoi(city_data->getCityID()) == minimumRouteTSP[i])
                    {
                        std::cout << city_data->getCityName();
                        if (i < TSP_RouteCounter)
                            std::cout << " --> ";
                        break;
                    }

            std::cout << "\n";
            underline(190, false);
        }
        else
            std::cout << "\n"
                      << std::setw(5) << " "
                      << "--> All depots are isolated!\n";
    }
}

void tsp::productTransportTSP(void)
{
    for (PRODUCT::PRODUCT_NODE *product_data = product.getHead(); product_data != nullptr; product_data = product_data->next)
    {
        int PRODUCT_ID = std::stoi(product_data->getProductID()), quantity = 0;
        for (DEPOT::DEPOT_NODE *depot_data = depot.getHead(); depot_data != nullptr; depot_data = depot_data->next)
            if (limitedStockCities[std::stoi(depot_data->getCityID())] == true &&
                isolatedVertex[std::stoi(depot_data->getCityID())] == false &&
                centralDepos[std::stoi(depot_data->getCityID())] == false)
                if (PRODUCT_ID == std::stoi(depot_data->getProductID()))
                    quantity += MAXIMUM_STOCK_VALUE - depot_data->getProductQuantity();

        supply.getData(product_data->getProductID(), quantity);
    }
}

void tsp::mainPageTSP(void)
{
    clear_screen();

    traveledDistanceTSP = 0.0;
    elapsedDurationTSP = 0;

    std::cout << "\n";
    if (!minimumRouteTSP.empty())
    {
        std::cout << std::setw(5) << " "
                  << "Route length: " << minimumDistanceCostTSP << "km | "
                  << "Route duration: " << minimumDurationCostTSP << "min\n"
                  << std::setw(5) << " ";

        for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
            for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
            {
                int ID = std::stoi(city_data->getCityID());
                if (i == 1)
                    if (ID == minimumRouteTSP[1])
                    {
#ifdef _WIN32
                        changeText(FOREGROUND_INTENSITY | COMMON_LVB_UNDERSCORE | FOREGROUND_RED | FOREGROUND_GREEN);
                        std::cout << city_data->getCityName();
                        resetText();
                        std::cout << " --> ";
#elif __linux__
                        std::cout << "\033[4m"
                                  << "\033[1m" << city_data->getCityName() << "\033[0m"
                                  << " --> ";
#endif
                    }

                if (ID == minimumRouteTSP[i] && i != 1)
                {
                    std::cout << city_data->getCityName();
                    if (i < TSP_RouteCounter)
                        std::cout << " --> ";
                    break;
                }
            }
    }

    std::cout << "\n";
    underline(190, false);

    std::cout << "\n";
    for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
        if (std::stoi(city_data->getCityID()) == minimumRouteTSP[1])
        {
            std::cout << std::setw(5) << " "
                      << "+-----------------------+\n"
                      << std::setw(8) << " [" << city_data->getCityID() << "] " << city_data->getCityName() << " - START\n"
                      << std::setw(5) << " "
                      << "+-----------------------+";
            break;
        }

#ifdef _WIN32
    changeText(FOREGROUND_INTENSITY);
    std::cout << std::setw(100) << " "
              << "Distance traveled: " << traveledDistanceTSP;
    resetText();
    std::cout << "km | ";
    changeText(FOREGROUND_INTENSITY);
    std::cout << "Travel duration: " << elapsedDurationTSP;
    resetText();
    std::cout << "min\n";
#elif __linux__
    std::cout << std::setw(100) << " "
              << "\033[1m"
              << "Distance traveled: "
              << "\033[0m" << traveledDistanceTSP << "km | "
              << "\033[1m"
              << "Travel duration: "
              << "\033[0m" << elapsedDurationTSP << "min\n";
#endif
    std::cout << "\n\n"
              << std::setw(5) << " "
              << "Product_ID" << std::setw(7) << " "
              << "Product_Name" << std::setw(13) << " "
              << "Total_transport_quantity\n";
    underline(70, true);

    for (SUPPLY::SUPPLY_NODE *supply_data = supply.getHead(); supply_data != nullptr; supply_data = supply_data->next)
    {
        int supplyProductID = std::stoi(supply_data->getID());
        std::cout << std::setw(5 + 1)
                  << " [" << supplyProductID << "] "
                  << std::setw(13) << " ";

        for (PRODUCT::PRODUCT_NODE *product_data = product.getHead(); product_data != nullptr; product_data = product_data->next)
            if (std::stoi(product_data->getProductID()) == supplyProductID)
            {
                std::cout << product_data->getProductName()
                          << std::setw(maxProductNameLength - strlen(product_data->getProductName()) + 5) << " ";
                break;
            }

        std::cout << supply_data->getQuantity() << " buc.\n";
    }

    underline(70, true);
    std::cout << "\n\n";
    underline(190, false);
    std::cout << std::setw(5) << " "
              << "[1] PREVIOUS" << std::setw(80) << " -" << consolePage << "- " << std::setw(80) << " "
              << "[2] NEXT\n";
    underline(190, false);

    traveledDistanceTSP += adjacencyMatrix[minimumRouteTSP[1]][minimumRouteTSP[2]].distance;
    elapsedDurationTSP += adjacencyMatrix[minimumRouteTSP[1]][minimumRouteTSP[2]].duration;
}

void tsp::finalPageTSP(void)
{
    std::ifstream get_contor("utils/contor_TSP_log.txt");
    if (!get_contor.is_open())
    {
        std::cerr << std::setw(5) << " "
                  << "Failed to open utils file!\n";
        _getch();
    }
    else
    {
        long long unsigned int contor_temp;
        get_contor >> contor_temp;
        logCounter = contor_temp;
    }
    get_contor.close();

    TSP_RoutesCompleted = true;
    logCounter++;

    std::ofstream file("utils/contor_TSP_log.txt");
    if (!file.is_open())
    {
        std::cerr << std::setw(5) << " "
                  << "Failed to open file!\n";
        _getch();
    }
    else
        file << logCounter;

    file.close();

    if (!depot.refreshData())
        std::cerr << std::setw(5) << " "
                  << "Could not update the database!\n";
    else if (fetchTables() == EXIT_FAILURE)
        _getch();

    std::ofstream log_out;
    log_out.open("logs/TSP_log.txt", std::ios::app);

    if (!log_out.is_open())
    {
        std::cerr << std::setw(5) << " "
                  << "Failed to open TSP log!\n";
        _getch();
    }
    else
    {
        std::string s(500, '=');
        log_out << "LOG [" << logCounter << "]\n";

        for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
        {
            for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
            {
                int ID = std::stoi(city_data->getCityID());
                if (ID == minimumRouteTSP[i])
                {
                    log_out << city_data->getCityName();
                    if (i < TSP_RouteCounter)
                        log_out << " --> ";
                    break;
                }
            }
        }

        log_out << "\n"
                << s << "\n"
                << "Distance traveled: " << minimumDistanceCostTSP << "km\n"
                << "Cantitate totala transportata: " << totalSuppliedQuantity << "BUC.\n"
                << "Cost total: " << totalSupplyCost << "RON\nEND-LOG\n"
                << s << "\n\n\n";

        log_out.close();
    }

    clear_screen();

    std::cout << "\n";
    underline(50, true);

    std::cout << std::setw(5) << " "
              << "-> Aprovizionare completa!\n"
              << std::setw(5) << " "
              << "-> Baza de date reinprostpatata!\n"
              << std::setw(5) << " "
              << "-> Mai multe detalii in fisierul TSP_log.txt!\n";

    underline(50, true);

    std::cout << std::setw(5) << " "
              << "Apasati 'ENTER' pentru a va intoarce...";

    _getch();
}

void tsp::leftPageTSP(void)
{
    clear_screen();

    if (consolePage - 1 > 1)
    {
        consolePage--;

        traveledDistanceTSP -= adjacencyMatrix[minimumRouteTSP[consolePage]][minimumRouteTSP[consolePage + 1]].distance;
        elapsedDurationTSP -= adjacencyMatrix[minimumRouteTSP[consolePage]][minimumRouteTSP[consolePage + 1]].duration;

        std::cout << "\n";
        if (!minimumRouteTSP.empty())
        {
            std::cout << std::setw(5) << " "
                      << "Route length: " << minimumDistanceCostTSP << " | "
                      << "Route duration: " << minimumDurationCostTSP << "min\n"
                      << std::setw(5) << " ";

            for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
            {
                for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
                {
                    int ID = std::stoi(city_data->getCityID());

                    if (ID == minimumRouteTSP[consolePage] && i == consolePage)
                    {
#ifdef _WIN32
                        changeText(FOREGROUND_INTENSITY | COMMON_LVB_UNDERSCORE | FOREGROUND_RED | FOREGROUND_GREEN);

                        std::cout << city_data->getCityName();

                        resetText();
#elif __linux__
                        std::cout << "\033[4m"
                                  << "\033[1m" << city_data->denumire_oras << "\033[0m"
                                  << " --> ";
#endif
                        if (i < TSP_RouteCounter)
                            std::cout << " --> ";
                        break;
                    }

                    if (ID == minimumRouteTSP[i] && i != consolePage)
                    {
                        std::cout << city_data->getCityName();
                        if (i < TSP_RouteCounter)
                            std::cout << " --> ";
                        break;
                    }
                }
            }
        }
        std::cout << "\n";
        underline(190, false);
        std::cout << "\n";

        for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
        {
            int ID = std::stoi(city_data->getCityID());
            if (ID == minimumRouteTSP[consolePage])
            {
                std::cout << std::setw(5) << " "
                          << "+-----------------------+\n"
                          << std::setw(8) << " [" << city_data->getCityID() << "] " << city_data->getCityName() << "\n"
                          << std::setw(5) << " "
                          << "+-----------------------+";
                break;
            }
        }

#ifdef _WIN32
        changeText(FOREGROUND_INTENSITY);
        std::cout << std::setw(100) << " "
                  << "Distance traveled: " << traveledDistanceTSP;
        resetText();
        std::cout << "km | ";
        changeText(FOREGROUND_INTENSITY);
        std::cout << "Travel duration: " << elapsedDurationTSP;
        resetText();
        std::cout << "min\n";

#elif __linux__
        std::cout << std::setw(100) << " "
                  << "\033[1m"
                  << "Distance traveled: "
                  << "\033[0m" << traveledDistanceTSP << "km | "
                  << "\033[1m"
                  << "Travel duration: "
                  << "\033[0m" << elapsedDurationTSP << "min\n";
#endif

        std::cout << "\n\n"
                  << std::setw(5) << " "
                  << "Product_ID" << std::setw(5) << " "
                  << "Product_Name" << std::setw(13) << " "
                  << "Quantity Transported/Quantity Remaining\n";
        underline(85, true);

        if (centralDepos[minimumRouteTSP[consolePage]] == false)
        {
            if (limitedStockCities[minimumRouteTSP[consolePage]] == false)
            {
#ifdef _WIN32
                changeText(FOREGROUND_INTENSITY);
                std::cout << std::setw(5) << " "
                          << "The depot has been restocked!"
                          << "\n";
                resetText();
#elif __linux__
                std::cout << std::setw(5) << " "
                          << "\033[1m"
                          << "The depot has been restocked!\n"
                          << "\033[0m";
#endif
            }
        }
        else
        {
#ifdef _WIN32
            changeText(FOREGROUND_INTENSITY);
            std::cout << std::setw(5) << " "
                      << "Central depot!"
                      << "\n";
            resetText();
#elif __linux__
            std::cout << std::setw(5) << " "
                      << "\033[1m"
                      << "Central depot!\n"
                      << "\033[0m";
#endif
        }

        underline(85, true);
        std::cout << "\n\n";
        underline(190, false);
        std::cout << std::setw(5) << " "
                  << "[1] PREVIOUS" << std::setw(80) << " -" << consolePage << "- " << std::setw(80) << " "
                  << "[2] NEXT\n";
        underline(190, false);
    }
    else
    {
        consolePage = 1;
        tsp::mainPageTSP();
    }
}

void tsp::rightPageTSP(void)
{
    clear_screen();

    if (consolePage < TSP_RouteCounter)
    {
        if (consolePage > 2)
        {
            traveledDistanceTSP += adjacencyMatrix[minimumRouteTSP[consolePage]][minimumRouteTSP[consolePage + 1]].distance;
            elapsedDurationTSP += adjacencyMatrix[minimumRouteTSP[consolePage]][minimumRouteTSP[consolePage + 1]].duration;
        }

        consolePage++;

        std::cout << "\n";
        if (!minimumRouteTSP.empty())
        {
            std::cout << std::setw(5) << " "
                      << "Route length: " << minimumDistanceCostTSP << "km | "
                      << "Route duration: " << minimumDurationCostTSP << "min\n"
                      << std::setw(5) << " ";

            for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
            {
                for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
                {
                    int ID = std::stoi(city_data->getCityID());

                    if (ID == minimumRouteTSP[consolePage] && i == consolePage)
                    {
#ifdef _WIN32
                        changeText(FOREGROUND_INTENSITY | COMMON_LVB_UNDERSCORE | FOREGROUND_RED | FOREGROUND_GREEN);

                        std::cout << city_data->getCityName();

                        resetText();
#elif __linux__
                        std::cout << "\033[4m"
                                  << "\033[1m" << city_data->denumire_oras << "\033[0m"
                                  << " --> ";
#endif
                        if (i < TSP_RouteCounter)
                            std::cout << " --> ";
                        break;
                    }

                    if (ID == minimumRouteTSP[i] && i != consolePage)
                    {
                        std::cout << city_data->getCityName();
                        if (i < TSP_RouteCounter)
                            std::cout << " --> ";
                        break;
                    }
                }
            }
        }
        std::cout << "\n";
        underline(190, false);
        std::cout << "\n";

        for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
        {
            int ID = std::stoi(city_data->getCityID());
            if (ID == minimumRouteTSP[consolePage])
            {
                std::cout << std::setw(5) << " "
                          << "+-----------------------+\n"
                          << std::setw(8) << " [" << city_data->getCityID() << "] " << city_data->getCityName() << "\n"
                          << std::setw(5) << " "
                          << "+-----------------------+";
                break;
            }
        }

#ifdef _WIN32
        changeText(FOREGROUND_INTENSITY);
        std::cout << std::setw(100) << " "
                  << "Distance traveled: " << traveledDistanceTSP;
        resetText();
        std::cout << "km | ";
        changeText(FOREGROUND_INTENSITY);
        std::cout << "Travel duration: " << elapsedDurationTSP;
        resetText();
        std::cout << "min\n";

#elif __linux__
        std::cout << std::setw(100) << " "
                  << "\033[1m"
                  << "Distance traveled: "
                  << "\033[0m" << traveledDistanceTSP << "km | "
                  << "\033[1m"
                  << "Travel duration: "
                  << "\033[0m" << elapsedDurationTSP << "min\n";
#endif

        std::cout << "\n\n"
                  << std::setw(5) << " "
                  << "Product_ID" << std::setw(5) << " "
                  << "Product_Name" << std::setw(13) << " "
                  << "Quantity Transported/Quantity Remaining\n";
        underline(85, true);

        if (centralDepos[minimumRouteTSP[consolePage]] == false)
        {
            if (limitedStockCities[minimumRouteTSP[consolePage]])
            {
                for (PRODUCT::PRODUCT_NODE *product_data = product.getHead(); product_data != nullptr; product_data = product_data->next)
                {
                    int PRODUCT_ID = std::stoi(product_data->getProductID());
                    double requiredQuantity = 0.0;

                    for (DEPOT::DEPOT_NODE *depot_data = depot.getHead(); depot_data != nullptr; depot_data = depot_data->next)
                    {
                        int ID_PRODUS_DEPOZIT = std::stoi(depot_data->getProductID()), ID_DEPOZIT = std::stoi(depot_data->getCityID());
                        if (ID_DEPOZIT == minimumRouteTSP[consolePage] && !isolatedVertex[ID_DEPOZIT])
                            if (ID_PRODUS_DEPOZIT == PRODUCT_ID)
                            {
                                requiredQuantity += depot_data->getProductQuantity();
                                requiredQuantity = MAXIMUM_STOCK_VALUE - requiredQuantity;
                                totalSupplyCost += requiredQuantity * product_data->getProductPrice();
                                totalSuppliedQuantity += requiredQuantity;
                                depot_data->updateQuantity(MAXIMUM_STOCK_VALUE);

                                std::cout << std::setw(5 + 1)
                                          << " [" << depot_data->getProductID() << "] "
                                          << std::setw(11) << " "
                                          << product_data->getProductName()
                                          << std::setw(maxProductNameLength - strlen(product_data->getProductName()) + 5)
                                          << " " << requiredQuantity << " buc. /";

                                for (SUPPLY::SUPPLY_NODE *supply_data = supply.getHead(); supply_data != nullptr; supply_data = supply_data->next)
                                    if (std::stoi(supply_data->getID()) == PRODUCT_ID)
                                    {
                                        std::cout << supply_data->getQuantity() << " buc.";
                                        int currentQuantity = supply_data->getQuantity();
                                        supply_data->updateQuantity(currentQuantity - requiredQuantity);
                                        break;
                                    }

                                std::cout << "\n";
                            }
                    }
                }
            }
            else
            {
#ifdef _WIN32
                changeText(FOREGROUND_INTENSITY);
                std::cout << std::setw(5) << " "
                          << "The depot has been restocked!"
                          << "\n";
                resetText();
#elif __linux__
                std::cout << std::setw(5) << " "
                          << "\033[1m"
                          << "The depot has been restocked!\n"
                          << "\033[0m";
#endif
            }
        }
        else
        {
#ifdef _WIN32
            changeText(FOREGROUND_INTENSITY);
            std::cout << std::setw(5) << " "
                      << "Central depot!"
                      << "\n";
            resetText();
#elif __linux__
            std::cout << std::setw(5) << " "
                      << "\033[1m"
                      << "Central depot!\n"
                      << "\033[0m";
#endif
        }

        underline(85, true);
        std::cout << "\n\n";
        underline(190, false);
        std::cout << std::setw(5) << " "
                  << "[1] PREVIOUS" << std::setw(80) << " -" << consolePage << "- " << std::setw(80) << " "
                  << "[2] NEXT\n";
        underline(190, false);

        limitedStockCities[minimumRouteTSP[consolePage]] = false;

        if (consolePage - 1 == 1)
        {
            traveledDistanceTSP += adjacencyMatrix[minimumRouteTSP[consolePage]][minimumRouteTSP[consolePage + 1]].distance;
            elapsedDurationTSP += adjacencyMatrix[minimumRouteTSP[consolePage]][minimumRouteTSP[consolePage + 1]].duration;
        }
    }
    else
        tsp::finalPageTSP();
}

void tsp::routeTraversalTSP(void)
{
    consolePage = 1;
    std::cout << "\n";

    bool isFound = false;

    for (unsigned int i = 0; i < limited_stock_cities_count && !isFound; i++)
        if (limitedStockCities[i] == true)
            isFound = true;

    if (!TSP_RoutesCompleted && isFound)
    {
        if (minimumRouteTSP[1] == -1)
            tsp::TSP();
        else
        {
            clear_screen();
#ifdef _WIN32
            changeText(FOREGROUND_INTENSITY);

            std::cout << std::setw(5) << " "
                      << "Route length: " << minimumDistanceCostTSP << "km | "
                      << "Route duration: " << minimumDurationCostTSP << "\n"
                      << std::setw(5) << " ";

            resetText();
#elif __linux__
            std::cout << std::setw(5) << " "
                      << "\033[1m"
                      << "Route length: " << minimumDistanceCostTSP << "km | "
                      << "Route duration: " << minimumDurationCostTSP << "\n"
                      << std::setw(5) << " "
                      << "\033[0m";
#endif
            for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
                for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
                    if (std::stoi(city_data->getCityID()) == minimumRouteTSP[i])
                    {
                        std::cout << city_data->getCityName();
                        if (i < TSP_RouteCounter)
                            std::cout << " --> ";
                        break;
                    }

            std::cout << "\n";
            underline(190, false);
        }
    }
    else
    {
        clear_screen();

#ifdef _WIN32
        changeText(FOREGROUND_INTENSITY);

        std::cout << "\n\n"
                  << std::setw(5) << " "
                  << "There are no depots with limited/exhausted stock...";

        resetText();
#elif __linux__
        std::cout << "\n\n"
                  << std::setw(5) << " "
                  << "\033[1m"
                  << "There are no depots with limited/exhausted stock..."
                  << "\033[0m";
#endif
        minimumRouteTSP.clear();
        _getch();
        return;
    }

    char *input = (char *)malloc(MAXL * sizeof(char) + 1);

    if (input == NULL)
        return;

    std::cout << std::setw(5) << " "
              << "Enter '0' to return\n\n"
              << std::setw(5) << " "
              << "[S] Start: ";
    std::cin >> input;

    if (_strcasecmp_(input, "0") == 0)
    {
        free(input);
        return;
    }
    else if (_strcasecmp_(input, "s") == 0)
    {
        unsigned int MENU = 0;

        tsp::mainPageTSP();

        do
        {
            std::cout << "\n"
                      << std::setw(5) << " "
                      << "Enter menu number: ";
            std::cin >> MENU;

            switch (MENU)
            {
            case 1:
                tsp::leftPageTSP();
                break;
            case 2:
                tsp::rightPageTSP();
                break;

            default:
                break;
            }
        } while (MENU != 0 && !TSP_RoutesCompleted);
    }

    free(input);
}

void speedyGo::displaySpeedyGoDetails(void)
{
    std::cout << "\n";
    underline(100, true);

    std::cout << "\033[3m"
              << std::setw(5) << " "
              << "Bun venit in Consola MySQL. Introduceti o interogare SQL (sau 'exit' pentru a incheia)\n"
              << "\033[0m";

    underline(100, true);

    sql::Statement *stmt = con->createStatement();
    sql::ResultSet *res = stmt->executeQuery("SHOW TABLES");

    std::cout << std::setw(5) << " "
              << "+" << std::string(22, '-') << "+"
              << "\n";
    std::cout << std::setw(5) << " "
              << "| " << std::left << std::setw(20) << "SpeedyGo - Tabele"
              << " |"
              << "\n";
    std::cout << std::setw(5) << " "
              << "|" << std::string(22, '-') << "|"
              << "\n";

    while (res->next())
    {
        std::cout << std::setw(5) << " "
                  << "| " << std::setw(20) << std::left << res->getString(1) << " |"
                  << "\n";
    }

    std::cout << std::right;

    std::cout << std::setw(5) << " "
              << "+" << std::string(22, '-') << "+"
              << "\n";

    underline(100, true);

    delete stmt;
    delete res;
}

void speedyGo::MySQLconsole(void)
{
    clear_screen();

    if (buffer)
    {
        buffer = false;
        std::cin.ignore(9999, '\n');
    }

    displaySpeedyGoDetails();

    std::string query = "";

    while (true)
    {
        std::cout << "\n"
                  << std::setw(5) << " "
                  << "mysql> ";

        std::getline(std::cin, query);

        if (query == "exit")
        {
            buffer = true;
            break;
        }
        else if (query == "clear")
        {
            clear_screen();
            displaySpeedyGoDetails();
        }
        else
        {
            try
            {
                sql::Statement *stmt = con->createStatement();
                sql::ResultSet *res = stmt->executeQuery(query);

                int cnt_coloane = res->getMetaData()->getColumnCount();
                std::vector<int> coloane(cnt_coloane, 0);

                while (res->next())
                    for (unsigned int i = 1; i <= cnt_coloane; i++)
                    {
                        int crt_width = res->getString(i).length();
                        if (crt_width > coloane[i - 1])
                            coloane[i - 1] = crt_width;
                    }

                std::cout << "\n";
                for (unsigned int i = 1; i <= cnt_coloane; i++)
                    std::cout << std::setw(5) << " " << std::setw(coloane[i - 1] + 5) << res->getMetaData()->getColumnName(i) << " ";
                std::cout << "\n";

                underline(100, true);

                res->beforeFirst();

                while (res->next())
                {
                    for (unsigned int i = 1; i <= cnt_coloane; i++)
                        std::cout << std::setw(5) << " " << std::setw(coloane[i - 1] + 5) << res->getString(i) << " ";
                    std::cout << "\n";
                }

                underline(100, true);

                city.~CITY();
                product.~PRODUCT();
                depot.~DEPOT();

                if (fetchTables() == EXIT_FAILURE)
                    _getch();

                delete stmt;
                delete res;
            }
            catch (sql::SQLException &e)
            {
                std::cout << std::setw(5) << " "
                          << "Error code: " << e.getErrorCode() << "\n";
                std::cout << std::setw(5) << " "
                          << "Error message: " << e.what() << "\n";
                std::cout << std::setw(5) << " "
                          << "SQLState: " << e.getSQLState() << "\n";
            }
        }
    }

    query.clear();
}