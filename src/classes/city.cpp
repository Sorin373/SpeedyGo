#include "../../include/classes/city.hpp"
#include "../include/database.hpp"
#include "../include/declarations.hpp"
#include "../include/logic.hpp"

#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>

using std::cin, std::cout, std::cerr, std::endl, std::string, std::setw, std::to_string;

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

CITY::CITY_NODE *CITY::getHead(void)
{
    return head_city;
}

CITY::CITY_NODE *CITY::getTail(void)
{
    return tail_city;
}

char *CITY::CITY_NODE::getCityID(void)
{
    return City_ID;
}

char *CITY::CITY_NODE::getCityName(void)
{
    return City_Name;
}

char *CITY::CITY_NODE::getCityType(void)
{
    return City_Type;
}

double CITY::CITY_NODE::getLatitude(void)
{
    return latitude;
}

double CITY::CITY_NODE::getLongitude(void)
{
    return longitude;
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

void CITY::sortData(const int sortType)
{
    bool isSorted = true;

    CITY::CITY_NODE *ptr;
    CITY::CITY_NODE *l_ptr = nullptr;

    if (head_city == nullptr)
        return;
    do
    {
        isSorted = true;
        ptr = head_city;

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

bool CITY::addCity(void)
{
    clear_screen();

    char *Depot_Name = (char *)malloc(MAXL * sizeof(char) + 1),
         *Depot_Type = (char *)malloc(MAXL * sizeof(char) + 1);
    double latitude = 0.0, longitude = 0.0;

    cout << "\n\n"
         << setw(16) << " "
         << "Introduceti datele noului depozit\n";

    underline(60, true);

    cout << setw(5) << " "
         << "City ID: " << contor_noduri_graf << "\n"
         << setw(5) << " "
         << "City Name: ";

    cin.get();
    cin.get(Depot_Name, MAXL);
    strcpy(denumire_depozit_nou, Depot_Name);

    cout << setw(5) << " "
         << "Depot Type:\n\n"
         << setw(5) << " "
         << "[1] Local / [2] Centralizat\n"
         << setw(5) << " "
         << "I: ";

    char *input = (char *)malloc(MAXL * sizeof(char) + 1);
    cin >> input;

    if (_strcasecmp_(input, "1") == 0)
        strcpy(Depot_Type, "local");
    else if (_strcasecmp_(input, "2") == 0)
        strcpy(Depot_Type, "centralizat");

    clear_screen();

    cout << "\n\n"
         << setw(16) << " "
         << "Enter the data for the new depot\n";

    underline(60, true);

    cout << setw(5) << " "
         << "Depot ID: " << contor_noduri_graf << "\n"
         << setw(5) << " "
         << "Depot Name: " << Depot_Name << "\n"
         << setw(5) << " "
         << "Depot Type: " << Depot_Type << "\n"
         << setw(5) << " "
         << "Latitudine: ";
    cin >> latitude;

    cout << setw(5) << " "
         << "Longitudine: ";
    cin >> longitude;

    underline(60, true);

    cout << setw(5) << " "
         << "Doriti sa finalizati? [Y]/[N] ";
    cin >> input;

    if (_strcasecmp_(input, "N") == 0)
        return EXIT_FAILURE;
    else if (_strcasecmp_(input, "Y") == 0)
    {
        try
        {
            cout << "\n"
                 << setw(5) << " "
                 << "Start...\n";

            sql::Statement *stmt = nullptr;

            string tableName = "oras",
                   query = "INSERT INTO " + tableName + " (ID_Oras, Denumire_Oras, latitudine, longitudine, Tip_Depozit) VALUES (" + to_string(contor_noduri_graf) + ", '" + Depot_Name + "', " + to_string(latitude) + ", " + to_string(longitude) + ", '" + Depot_Type + "')";

            stmt = con->createStatement();
            stmt->execute(query);
            stmt->close();

            delete stmt;

            cout << setw(5) << " "
                 << "Success...\n";
        }
        catch (sql::SQLException &e)
        {
            cerr << "\n"
                 << setw(5) << " "
                 << "Error code: " << e.getErrorCode() << "\n"
                 << setw(5) << " "
                 << "Error message: " << e.what() << "\n"
                 << setw(5) << " "
                 << "SQLState: " << e.getSQLState() << "\n";

            return EXIT_FAILURE;
        }
    }

    free(Depot_Name);
    free(Depot_Type);
    free(input);

    return EXIT_SUCCESS;
}

bool CITY::deleteCity(void)
{
    clear_screen();

    afisare_date_tabel_oras();

    string ID;
    cout << setw(5) << " "
         << "Enter the corresponding City ID ('EXIT' to cancel): ";

    cin.ignore(9999, '\n');
    getline(cin, ID);

    const char *cStrID = ID.c_str();

    if (_strcasecmp_(cStrID, "EXIT") == 0)
        return EXIT_SUCCESS;

    try
    {
        cout << "\n"
             << setw(5) << " "
             << "Start...\n";

        sql::Statement *stmt = nullptr;
        string table_name = "oras",
               query = "DELETE FROM " + table_name + " WHERE ID_Oras = " + ID;

        stmt = con->createStatement();
        stmt->execute(query);
        stmt->close();

        delete stmt;

        cout << setw(5) << " "
             << "Success...\n";
    }
    catch (sql::SQLException &e)
    {
        cerr << "\n"
             << setw(5) << " "
             << "Error code: " << e.getErrorCode() << "\n"
             << setw(5) << " "
             << "Error message: " << e.what() << "\n"
             << setw(5) << " "
             << "SQLState: " << e.getSQLState() << "\n";

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
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
}