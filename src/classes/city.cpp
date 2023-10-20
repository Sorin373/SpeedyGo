#include "../../include/classes/city.hpp"
#include "../include/database.hpp"
#include "../include/declarations.hpp"
#include "../include/logic.hpp"

#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

CITY::CITY_NODE::CITY_NODE(const char *City_ID, const char *City_Name, const char *City_Type, const double latitude, const double longitude)
{
    this->City_ID = strdup(City_ID);
    this->City_Name = strdup(City_Name);
    this->City_Type = strdup(City_Type);
    this->latitude = latitude;
    this->longitude = longitude;
    next = nullptr;
    prev = nullptr;
}

CITY::CITY_NODE::~CITY_NODE(void)
{
    delete[] City_ID;
    delete[] City_Name;
    delete[] City_Type;
}

CITY::CITY_NODE *CITY::getHead(void) const
{
    return head_city;
}

CITY::CITY_NODE *CITY::getTail(void) const
{
    return tail_city;
}

char *CITY::CITY_NODE::getCityID(void) const
{
    return City_ID;
}

char *CITY::CITY_NODE::getCityName(void) const
{
    return City_Name;
}

char *CITY::CITY_NODE::getCityType(void) const
{
    return City_Type;
}

double CITY::CITY_NODE::getLatitude(void) const
{
    return latitude;
}

double CITY::CITY_NODE::getLongitude(void) const
{
    return longitude;
}

void CITY::CITY_NODE::displayCityData(void)
{
    clear_screen();

    std::cout << "\n\n"
              << std::setw(5) << " "
              << "+---------------+\n"
              << std::setw(5) << " "
              << "| TABEL-DEPOT |\n"
              << std::setw(5) << " "
              << "+---------------+\n\n";

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
    underline(80, true);

    for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
    {
        std::cout << std::setw(5 + 1) << " [" << date_oras->getCityID() << "]" << std::setw(maxCityIDLength - strlen(date_oras->getCityID()) + 8)
                  << " " << date_oras->getCityName() << std::setw(maxCityNameLength - strlen(date_oras->getCityName()) + 4)
                  << " " << date_oras->getCityType() << std::setw(11 - strlen(date_oras->getCityType()) + 5)
                  << " " << std::fixed << std::setprecision(2) << date_oras->getLatitude();
#ifdef _WIN32
        std::cout << "\370";
#elif __linux__
        std::cout << "\u00B0";
#endif
        std::cout << std::setw(maxCityLatitudeLength - std::to_string(round(date_oras->getLatitude())).length() + 13)
                  << " " << date_oras->getLongitude();
#ifdef _WIN32
        std::cout << "\370"
                  << "\n";
#elif __linux__
        std::cout << "\u00B0\n";
#endif
    }

    underline(80, true);
}

void CITY::CITY_NODE::swapData(CITY_NODE &node)
{
    std::swap(City_ID, node.City_ID);
    std::swap(City_Name, node.City_Name);
    std::swap(City_Type, node.City_Type);
    std::swap(latitude, node.latitude);
    std::swap(longitude, node.longitude);
}

void CITY::getData(const char *City_ID, const char *City_Name, const char *City_Type, const double latitude, const double longitude)
{
    CITY_NODE *newnod = new CITY_NODE(City_ID, City_Name, City_Type, latitude, longitude);

    if (head_city == nullptr)
    {
        head_city = newnod;
        tail_city = newnod;
    }
    else
    {
        tail_city->next = newnod;
        newnod->prev = tail_city;
        tail_city = newnod;
    }
}

void CITY::CITY_NODE::sortData(const int sortType)
{
    bool isSorted = true;

    CITY::CITY_NODE *ptr = nullptr, *l_ptr = nullptr;

    if (city.getHead() == nullptr)
        return;
    do
    {
        isSorted = true;
        ptr = city.getHead();

        while (ptr->next != l_ptr)
        {
            if (sortType == 1)
            {
                if (std::stoi(ptr->getCityID()) > std::stoi(ptr->next->getCityID()))
                {
                    ptr->CITY::CITY_NODE::swapData(*(ptr->next));
                    isSorted = false;
                }
            }
            else if (sortType == 2)
            {
                if (std::stoi(ptr->getCityID()) < std::stoi(ptr->next->getCityID()))
                {
                    ptr->CITY::CITY_NODE::swapData(*ptr->next);
                    isSorted = false;
                }
            }

            ptr = ptr->next;
        }
        l_ptr = ptr;
    } while (!isSorted);
}

void CITY::CITY_NODE::filterCityByCategory(void)
{
    clear_screen();

    CITY::CITY_NODE::displayCityData();

    char *UserInput_City_Type = (char *)malloc((MAXL + 1) * sizeof(char));

#ifdef _WIN32
    changeText(FOREGROUND_INTENSITY);
    std::cout << std::setw(5) << " "
              << "Enter '0' to return\n\n";
    resetText();
    std::cout << std::setw(5) << " "
              << "Enter the depot type: ";
#elif __linux__
    std::cout << setw(5) << " "
              << "\033[3m"
              << "Enter '0' to return\n\n"
              << "\033[0m" << setw(5) << " "
              << "Enter the depot type: ";
#endif

    std::cin >> UserInput_City_Type;

    if (_strcasecmp_(UserInput_City_Type, "0") == 0)
    {
        free(UserInput_City_Type);
        return;
    }
    else
    {
        clear_screen();

        std::cout << "\n\n"
                  << std::setw(5) << " "
                  << "+-------------+\n"
                  << std::setw(5) << " "
                  << "| DEPOT TABLE |\n"
                  << std::setw(5) << " "
                  << "+-------------+\n\n";

        std::cout << std::setw(5) << " "
                  << "City_ID"
                  << std::setw(5) << " "
                  << "City_Name"
                  << std::setw(5) << " "
                  << "Depot_Type"
                  << std::setw(5) << " "
                  << "Latitude"
                  << std::setw(5) << " "
                  << "Longitude\n";

        underline(80, true);

        for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
            if (_strcasecmp_(city_data->City_Type, UserInput_City_Type) == 0)
            {
                std::cout << std::setw(5 + 1)
                          << " [" << city_data->City_ID << "]"
                          << std::setw(maxCityIDLength - strlen(city_data->City_ID) + 8)
                          << " " << city_data->City_Name
                          << std::setw(maxCityNameLength - strlen(city_data->City_Name) + 4)
                          << " " << city_data->City_Type
                          << std::setw(11 - strlen(city_data->City_Type) + 5) << " " << std::fixed << std::setprecision(2)
                          << city_data->latitude;
#ifdef _WIN32
                std::cout << "\370";
#elif __linux__
                std::cout << "\u00B0";
#endif
                std::cout << std::setw(maxCityLatitudeLength - std::to_string(round(city_data->latitude)).length() + 13)
                          << " " << city_data->longitude;
#ifdef _WIN32
                std::cout << "\370"
                          << "\n";
#elif __linux__
                std::cout << "\u00B0\n";
#endif
            }

        underline(80, true);

        std::cout << "\n\n"
                  << std::setw(5) << " "
                  << "Press 'ENTER' to return...";

        free(UserInput_City_Type);
        _getch();
        filterCityByCategory();
    }
}

void CITY::CITY_NODE::alphabeticalCitySort(const int sortType)
{
    clear_screen();

    std::cout << "\n\n"
              << std::setw(5) << " "
              << "+-------------+\n"
              << std::setw(5) << " "
              << "| DEPOT TABLE |\n"
              << std::setw(5) << " "
              << "+-------------+\n\n";

    std::cout << std::setw(5) << " "
              << "City_ID"
              << std::setw(5) << " "
              << "City_Name"
              << std::setw(5) << " "
              << "Depot_Type"
              << std::setw(5) << " "
              << "Latitude"
              << std::setw(5) << " "
              << "Longitude\n";

    underline(80, true);

    bool sort = true;

    CITY::CITY_NODE *ptr = nullptr, *l_ptr = nullptr;

    if (city.getHead() == nullptr)
        return;

    do
    {
        sort = true;
        ptr = city.getHead();

        while (ptr->next != l_ptr)
        {
            if (sortType == 1)
            {
                if (strcmp(ptr->City_Name, ptr->next->City_Name) > 0)
                {
                    ptr->swapData(*(ptr->next));
                    sort = false;
                }
            }
            else if (sortType == 2)
            {
                if (strcmp(ptr->City_Name, ptr->next->City_Name) < 0)
                {
                    ptr->swapData(*(ptr->next));
                    sort = false;
                }
            }

            ptr = ptr->next;
        }
        l_ptr = ptr;
    } while (!sort);
}

void CITY::CITY_NODE::searchCityByID(void)
{
    clear_screen();

    CITY::CITY_NODE::displayCityData();

    char *UserInput_City_ID = (char *)malloc(MAXL * sizeof(char) + 1);
    bool gasit = false;

#ifdef _WIN32
    changeText(FOREGROUND_INTENSITY);
    std::cout << std::setw(5) << " "
              << "Type 'EXIT' to return\n\n";
    resetText();
    std::cout << std::setw(5) << " "
              << "Enter the city ID: ";
#elif __linux__
    std::cout << std::setw(5) << " "
              << "\033[3m"
              << "Type 'EXIT' to return\n\n"
              << "\033[0m"
              << std::setw(5) << " "
              << "Enter the city ID: ";
#endif

    std::cin >> UserInput_City_ID;

    if (_strcasecmp_(UserInput_City_ID, "exit") == 0)
    {
        free(UserInput_City_ID);
        return;
    }

    clear_screen();

    std::cout << "\n\n"
              << std::setw(5) << " "
              << "+-------------+\n"
              << std::setw(5) << " "
              << "| DEPOT TABLE |\n"
              << std::setw(5) << " "
              << "+-------------+\n\n";

    std::cout << std::setw(5) << " "
              << "City_ID"
              << std::setw(5) << " "
              << "City_Name"
              << std::setw(5) << " "
              << "Depot_Type"
              << std::setw(5) << " "
              << "Latitude"
              << std::setw(5) << " "
              << "Longitude\n";

    underline(80, true);

    for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
        if (_strcasecmp_(city_data->City_ID, UserInput_City_ID) == 0)
        {
            gasit = true;

            std::cout << std::setw(5 + 1)
                      << " [" << city_data->City_ID << "]"
                      << std::setw(maxCityIDLength - strlen(city_data->City_ID) + 8)
                      << " " << city_data->City_Name
                      << std::setw(maxCityNameLength - strlen(city_data->City_Name) + 4)
                      << " " << city_data->City_Type
                      << std::setw(11 - strlen(city_data->City_Type) + 5) << " " << std::fixed << std::setprecision(2)
                      << city_data->latitude;
#ifdef _WIN32
            std::cout << "\370";
#elif __linux__
            std::cout << "\u00B0";
#endif
            std::cout << std::setw(maxCityLatitudeLength - std::to_string(round(city_data->latitude)).length() + 13)
                      << " " << city_data->longitude;
#ifdef _WIN32
            std::cout << "\370"
                      << "\n";
#elif __linux__
            std::cout << "\u00B0\n";
#endif
            break;
        }

    underline(80, true);

    if (!gasit)
    {
        clear_screen();

        std::cout << "\n"
                  << std::setw(5) << " "
                  << "Invalid user input!";

        free(UserInput_City_ID);
        _getch();
        searchCityByID();
    }
    else
    {
        std::cout << "\n"
                  << std::setw(5) << " "
                  << "Press 'ENTER' to return...";

        _getch();
        free(UserInput_City_ID);
        searchCityByID();
    }
}

void CITY::CITY_NODE::searchCityByName(void)
{
    clear_screen();

    CITY::CITY_NODE::displayCityData();

    char *UserInput_City_Name = (char *)malloc(MAXL * sizeof(char) + 1);

    if (UserInput_City_Name == NULL)
        return;

    bool isFound = false;

#ifdef _WIN32
    changeText(FOREGROUND_INTENSITY);
    std::cout << std::setw(5) << " "
              << "Enter '0' to return\n\n";
    resetText();
    std::cout << std::setw(5) << " "
              << "Enter the city name: ";
#elif __linux__
    std::cout << std::setw(5) << " "
              << "\033[3m"
              << "Enter '0' to return\n\n"
              << "\033[0m" << std::setw(5) << " "
              << "Enter the city name: ";
#endif

    std::cin.get();
    std::cin.get(UserInput_City_Name, MAXL);

    if (_strcasecmp_(UserInput_City_Name, "0") == 0)
    {
        free(UserInput_City_Name);
        return;
    }

    clear_screen();

    std::cout << "\n\n"
              << std::setw(5) << " "
              << "+-------------+\n"
              << std::setw(5) << " "
              << "| DEPOT TABLE |\n"
              << std::setw(5) << " "
              << "+-------------+\n\n";

    std::cout << std::setw(5) << " "
              << "City_ID"
              << std::setw(5) << " "
              << "City_Name"
              << std::setw(5) << " "
              << "Depot_Type"
              << std::setw(5) << " "
              << "Latitude"
              << std::setw(5) << " "
              << "Longitude\n";

    underline(80, true);

    for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
        if (_strcasecmp_(city_data->City_Name, UserInput_City_Name) == 0)
        {
            isFound = true;

            std::cout << std::setw(5 + 1)
                      << " [" << city_data->City_ID << "]"
                      << std::setw(maxCityIDLength - strlen(city_data->City_ID) + 8)
                      << " " << city_data->City_Name
                      << std::setw(maxCityNameLength - strlen(city_data->City_Name) + 4)
                      << " " << city_data->City_Type
                      << std::setw(11 - strlen(city_data->City_Type) + 5)
                      << " " << std::fixed << std::setprecision(2)
                      << city_data->latitude;
#ifdef _WIN32
            std::cout << "\370";
#elif __linux__
            std::cout << "\u00B0";
#endif
            std::cout << std::setw(maxCityLatitudeLength - std::to_string(round(city_data->latitude)).length() + 13)
                      << " " << city_data->longitude;
#ifdef _WIN32
            std::cout << "\370"
                      << "\n";
#elif __linux__
            std::cout << "\u00B0\n";
#endif
            break;
        }

    underline(80, true);

    if (!isFound)
    {
        clear_screen();

        std::cout << "\n\n"
                  << std::setw(5) << " "
                  << "Invalid user input!";

        free(UserInput_City_Name);
        _getch();
        searchCityByName();
    }
    else
    {
        std::cout << "\n"
                  << std::setw(5) << " "
                  << "Press 'ENTER' to return...";

        _getch();
        free(UserInput_City_Name);
        searchCityByName();
    }
}

bool CITY::addCity(void)
{
    clear_screen();

    char *Depot_Name = (char *)malloc(MAXL * sizeof(char) + 1),
         *Depot_Type = (char *)malloc(MAXL * sizeof(char) + 1);
         
    double latitude = 0.0, longitude = 0.0;

    std::cout << "\n\n"
              << std::setw(16) << " "
              << "Enter the data for the new depot:\n";

    underline(60, true);

    std::cout << std::setw(5) << " "
              << "City ID: " << VERTEX_COUNT << "\n"
              << std::setw(5) << " "
              << "City Name: ";

    std::cin.get();
    std::cin.get(Depot_Name, MAXL);
    strcpy(newCityName, Depot_Name);

    std::cout << std::setw(5) << " "
              << "Depot Type:\n\n"
              << std::setw(5) << " "
              << "[1] Local / [2] Central\n"
              << std::setw(5) << " "
              << "--> ";

    char *input = (char *)malloc(MAXL * sizeof(char) + 1);
    std::cin >> input;

    if (_strcasecmp_(input, "1") == 0)
        strcpy(Depot_Type, "local");
    else if (_strcasecmp_(input, "2") == 0)
        strcpy(Depot_Type, "centralizat");

    clear_screen();

    std::cout << "\n\n"
              << std::setw(16) << " "
              << "Enter the data for the new depot\n";

    underline(60, true);

    std::cout << std::setw(5) << " "
              << "Depot ID: " << VERTEX_COUNT << "\n"
              << std::setw(5) << " "
              << "Depot Name: " << Depot_Name << "\n"
              << std::setw(5) << " "
              << "Depot Type: " << Depot_Type << "\n"
              << std::setw(5) << " "
              << "Latitudine: ";
    std::cin >> latitude;

    std::cout << std::setw(5) << " "
              << "Longitudine: ";
    std::cin >> longitude;

    underline(60, true);

    std::cout << std::setw(5) << " "
              << "Are you sure? [Y]/[N] ";
    std::cin >> input;

    if (_strcasecmp_(input, "N") == 0)
        return EXIT_FAILURE;
    else if (_strcasecmp_(input, "Y") == 0)
    {
        try
        {
            std::cout << "\n"
                      << std::setw(5) << " "
                      << "Start...\n";

            sql::Statement *stmt = nullptr;

            std::string tableName = "oras",
                        query = "INSERT INTO " + tableName + " (ID_Oras, Denumire_Oras, latitudine, longitudine, Tip_Depozit) VALUES (" + std::to_string(VERTEX_COUNT) + ", '" + Depot_Name + "', " + std::to_string(latitude) + ", " + std::to_string(longitude) + ", '" + Depot_Type + "')";

            stmt = con->createStatement();
            stmt->execute(query);
            stmt->close();

            delete stmt;

            std::cout << std::setw(5) << " "
                      << "Success...\n";
        }
        catch (sql::SQLException &e)
        {
            std::cerr << "\n"
                      << std::setw(5) << " "
                      << "Error code: " << e.getErrorCode() << "\n"
                      << std::setw(5) << " "
                      << "Error message: " << e.what() << "\n"
                      << std::setw(5) << " "
                      << "SQLState: " << e.getSQLState() << "\n";

            return EXIT_FAILURE;
        }
    }

    free(Depot_Name);
    free(Depot_Type);
    free(input);

    VERTEX_COUNT = 0;

    if (fetchTables() == EXIT_FAILURE)
    {
        _getch();
        return EXIT_FAILURE;
    }

    if (addGraphEdge() == EXIT_FAILURE)
    {
        std::cerr << std::setw(5) << " "
                  << "Failed to add edge!\n";
        _getch();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

bool CITY::deleteCity(void)
{
    clear_screen();

    CITY::CITY_NODE::displayCityData();

    std::string ID = "";
    std::cout << std::setw(5) << " "
              << "Enter the corresponding City ID ('EXIT' to cancel): ";

    std::cin.ignore(9999, '\n');
    std::getline(std::cin, ID);

    const char *cStrID = ID.c_str();

    if (_strcasecmp_(cStrID, "EXIT") == 0)
        return EXIT_SUCCESS;

    try
    {
        std::cout << "\n"
                  << std::setw(5) << " "
                  << "Start...\n";

        sql::Statement *stmt = nullptr;
        std::string table_name = "oras",
                    query = "DELETE FROM " + table_name + " WHERE ID_Oras = " + ID;

        stmt = con->createStatement();
        stmt->execute(query);
        stmt->close();

        delete stmt;

        std::cout << std::setw(5) << " "
                  << "Success...\n";
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "\n"
                  << std::setw(5) << " "
                  << "Error code: " << e.getErrorCode() << "\n"
                  << std::setw(5) << " "
                  << "Error message: " << e.what() << "\n"
                  << std::setw(5) << " "
                  << "SQLState: " << e.getSQLState() << "\n";

        return EXIT_FAILURE;
    }

    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
    {
        adjacencyMatrix[std::stoi(ID.c_str())][i].distance = adjacencyMatrix[i][std::stoi(ID.c_str())].distance = 0.0;
        adjacencyMatrix[std::stoi(ID.c_str())][i].duration = adjacencyMatrix[i][std::stoi(ID.c_str())].duration = 0;
    }

    std::ofstream edgeFile("utils/legaturi.txt");
    if (!edgeFile.is_open())
    {
        std::cerr << std::setw(5) << " "
                  << "Failed to open file!\n";
        _getch();
        return EXIT_FAILURE;
    }

    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
        for (unsigned int j = 0; j < VERTEX_COUNT; j++)
            if (adjacencyMatrix[i][j].distance != 0 && i < j)
                edgeFile << j << " " << i << "\n";

    edgeFile.close();

    VERTEX_COUNT = 0;

    if (fetchTables() == EXIT_FAILURE)
    {
        _getch();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void CITY::fetchTable(void)
{
    sql::Statement *stmt = nullptr;
    sql::ResultSet *res = nullptr;

    stmt = con->createStatement();
    res = stmt->executeQuery("SELECT * FROM oras");

    while (res->next())
    {
        sql::SQLString sqlstr;

        int iCity_ID = res->getInt("ID_Oras");
        char *cCity_ID = (char *)malloc(std::to_string(iCity_ID).length() + 1);
        strcpy(cCity_ID, std::to_string(iCity_ID).c_str());

        sqlstr = res->getString("Denumire_Oras");
        char *City_Name = (char *)malloc(sqlstr.asStdString().length() + 1);
        strcpy(City_Name, sqlstr.asStdString().c_str());

        sqlstr = res->getString("Tip_Depozit");
        char *City_Type = (char *)malloc(sqlstr.asStdString().length() + 1); // City_Type = the type of depot (local / central)
        strcpy(City_Type, sqlstr.asStdString().c_str());

        double latitude = res->getDouble("latitudine");
        double longitude = res->getDouble("longitudine");

        city.getData(cCity_ID, City_Name, City_Type, latitude, longitude);

        VERTEX_COUNT++;

        free(cCity_ID);
        free(City_Name);
        free(City_Type);
    }

    res->close();
    stmt->close();

    delete res;
    delete stmt;
}

CITY::~CITY()
{
    CITY_NODE *ptr = head_city;

    while (ptr != nullptr)
    {
        CITY_NODE *temp = ptr;
        ptr = ptr->next;
        delete temp;
    }

    head_city = nullptr;
    tail_city = nullptr;
}