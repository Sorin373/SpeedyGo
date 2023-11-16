#include "../../include/speedyGo.hpp"
#include "../../include/tsp.hpp"
#include "../../include/GoogleMatrixAPI.hpp"
#include "../../include/haversine.hpp"
#include "../../include/declarations.hpp"
#include "../../include/database.hpp"

#include <iostream>
#include <iomanip>

#ifdef max
#undef max
#endif

namespace speedyGo {
    CinProxy cin;
}

bool speedyGo::_INIT_(void)
{
#ifdef _WIN32
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    originalAttributes = csbi.wAttributes;
#endif

    if (speedyGo::validateMysqlCredentials() == EXIT_FAILURE)
    {
        std::cerr << "\n"
                  << std::setw(5) << " "
                  << "-- Authentication process did not complete!\n";
        ERROR_CNT++;
        return EXIT_FAILURE;
    }
    else
        std::cout << std::setw(5) << " "
                  << "-- Authentication successful!\n";

    if (speedyGo::_ADJACENCY_MATRIX_INIT_() == EXIT_FAILURE)
    {
        std::cerr << std::setw(5) << " "
                  << "-- Application initialisation process did not complete!\n";
        ERROR_CNT++;
        return EXIT_FAILURE;
    }
    else if (ERROR_CNT == 1)
        std::cout << std::setw(5) << " "
                  << "-- All the necessary files were initialized with " << ERROR_CNT << " error!\n";
    else
        std::cout << std::setw(5) << " "
                  << "-- All the necessary files were initialized with " << ERROR_CNT << " errors!\n";

    std::cout << "\n"
              << std::setw(5) << " "
              << "-- Starting application configuration...\n\n";

    DEPOT::DEPOT_NODE::sortData();
    std::cout << std::setw(6) << " "
              << "--> Data sort complete (1)\n";

    CITY::CITY_NODE::sortData(1);
    std::cout << std::setw(6) << " "
              << "--> Data sort complete (2)\n";

    PRODUCT::PRODUCT_NODE::sortData(1);
    std::cout << std::setw(6) << " "
              << "--> Data sort complete (3)\n";

    CITY::CITY_NODE::selectCityType();
    std::cout << std::setw(6) << " "
              << "--> Data configuration complete (1)\n";

    DEPOT::DEPOT_NODE::searchLimitedStockCities();
    std::cout << std::setw(6) << " "
              << "--> Data configuration complete (2)\n";

    ADJACENCY_MATRIX_INITIALIZER::ADJACENCY_MATRIX_INITIALIZER_NODE::searchIsolatedVertices();
    std::cout << std::setw(6) << " "
              << "--> Data configuration complete (3)\n";

    tsp::productTransportTSP();
    std::cout << std::setw(6) << " "
              << "--> Data configuration complete (4)\n";

    speedyGo::alignConsoleOutput();
    std::cout << std::setw(6) << " "
              << "--> Data configuration complete (5)\n";

    minimumRouteTSP[1] = -1;

    return EXIT_SUCCESS;
}

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
              << std::setw(13) << " "
              << "DATABASE CONNECTION\n"
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

        if (_HAVERSINE_INIT_(EDGES) == EXIT_FAILURE)
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

void speedyGo::displaySpeedyGoDetails(void)
{
    std::cout << "\n";
    underline(100, true);

    std::cout << "\033[3m"
              << std::setw(5) << " "
              << "Welcome to the MySQL Console. Enter an SQL query (or 'exit' to end)\n"
              << "\033[0m";

    underline(100, true);

    sql::Statement *stmt = con->createStatement();
    sql::ResultSet *res = stmt->executeQuery("SHOW TABLES");

    std::cout << std::setw(5) << " "
              << "+" << std::string(22, '-') << "+"
              << "\n";
    std::cout << std::setw(5) << " "
              << "| " << std::left << std::setw(20) << "SpeedyGo - Tables"
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

void speedyGo::alignConsoleOutput(void)
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
                    std::cout << std::setw(5) << " "
                              << depotName << " -> " << city_data->getCityName()
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

void speedyGo::stockStatusVisualization(void)
{
    DEPOT::DEPOT_NODE::searchLimitedStockCities();

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
              << "+--------------------+\n"
              << std::setw(5) << " "
              << " Limited stock cities\n"
              << std::setw(5) << " "
              << "+--------------------+\n";

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
              << "Type 'EXIT' to return\n\n";
    resetText();
#elif __linux__
    std::cout << std::setw(5) << " "
              << "\033[3m"
              << "Type 'EXIT' to return\n\n"
              << "\033[0m";
#endif

    char *t_CityID = (char *)malloc(MAXL * sizeof(char) + 1);

    if (t_CityID == NULL)
        return;

    std::cout << std::setw(5) << " "
              << "City ID: ";
    std::cin >> t_CityID;

    if (_strcasecmp_(t_CityID, "exit") == 0)
    {
        free(t_CityID);
        return;
    }
    else
    {
        int CityID = std::stoi(t_CityID);
        free(t_CityID);
        city_data = city.getHead();

        while (city_data != nullptr)
        {
            int t_ID = std::stoi(city_data->getCityID());
            if (t_ID == CityID && limitedStockCities[t_ID] == true)
            {
                unsigned int MENU;

                do
                {
                    clear_screen();

                    std::cout << "\n\n"
                              << std::setw(5) << " "
                              << "+--------------------------------------------+\n"
                              << std::setw(7) << " " << city_data->getCityName() << " | Depot type: " << city_data->getCityName() << "\n"
                              << std::setw(5) << " "
                              << "+--------------------------------------------+\n";

                    underline(50, true);

                    std::cout << std::setw(5) << " "
                              << "[1] View products with limited stock\n"
                              << std::setw(5) << " "
                              << "[2] View connections with other depots\n"
                              << std::setw(5) << " "
                              << "[0] Back\n";

                    underline(50, true);

                    std::cout << std::setw(5) << " "
                              << "Enter menu number: ";
                    speedyGo::cin >> MENU;

                    switch (MENU)
                    {
                    case 1:
                        limitedStockProductSearchByID(CityID);
                        _getch();
                        break;
                    case 2:
                        connectedNodes(CityID);
                        _getch();
                        break;

                    default:
                        break;
                    }

                } while (MENU != 0);

                break;
            }
            city_data = city_data->next;
        }
        stockStatusVisualization();
    }
}

void speedyGo::free_memory(void)
{
    adjacencyMatrix.clear();
    centralDepos.clear();
    limitedStockCities.clear();
    isolatedVertex.clear();
    oneEdgeVertex.clear();
    stack.clear();
    minimumRouteTSP.clear();
    free(newCityName);
    AUTHENTICATION::cleanup();
    delete con;
}