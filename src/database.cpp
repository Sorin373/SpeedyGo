#include "../include/database.hpp"
#include "../include/declarations.hpp"
#include "../include/haversine.hpp"
#include "../include/logic.hpp"

#include <cstring>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>

using std::cout, std::cin, std::cerr, std::endl, std::setw, std::to_string, std::stoi,
      std::ifstream, std::ofstream, std::string;

sql::Driver *driver;
sql::Connection *con;

bool fetchTables(void)
{
    try
    {
        sql::Statement *stmt = nullptr;
        sql::ResultSet *res = nullptr;

        driver = sql::mysql::get_mysql_driver_instance();

        con = driver->connect("tcp://" + string(AUTHENTICATION::getNode()->host_name),
                          string(AUTHENTICATION::getNode()->username),
                          string(AUTHENTICATION::getNode()->password));

        if (con == nullptr)
        {
            delete res;
            delete stmt;
            return EXIT_FAILURE;
        }

        con->setSchema(AUTHENTICATION::getNode()->database);

        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * FROM depozit");

        while (res->next())
        {
            int iProduct_ID = res->getInt("ID_Produs");
            char *cProduct_ID = (char *)malloc(to_string(iProduct_ID).length() + 1);
            strcpy(cProduct_ID, to_string(iProduct_ID).c_str());

            int iCity_ID = res->getInt("ID_Oras");
            char *cCity_ID = (char *)malloc(to_string(iCity_ID).length() + 1);
            strcpy(cCity_ID, to_string(iCity_ID).c_str());

            double Prdouct_Quantity = res->getDouble("Cantitate_Produs");

            depot.getData(cProduct_ID, cCity_ID, Prdouct_Quantity);

            free(cProduct_ID);
            free(cCity_ID);
        }

        res->close();
        stmt->close();
        delete res;
        delete stmt;

        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * FROM produs");

        while (res->next())
        {
            sql::SQLString sqlstr;

            int iProduct_ID = res->getInt("ID_Produs");
            char *cProduct_ID = (char *)malloc(to_string(iProduct_ID).length() + 1);
            strcpy(cProduct_ID, to_string(iProduct_ID).c_str());

            sqlstr = res->getString("Denumire_Produs");
            char *Product_Name = (char *)malloc(sqlstr.asStdString().length() + 1);
            strcpy(Product_Name, sqlstr.asStdString().c_str());

            sqlstr = res->getString("Categorie_Produs");
            char *Product_Category = (char *)malloc(sqlstr.asStdString().length() + 1);
            strcpy(Product_Category, sqlstr.asStdString().c_str());

            double Product_Price = res->getDouble("Pret_Produs");

            product.getData(cProduct_ID, Product_Name, Product_Category, Product_Price);

            free(cProduct_ID);
            free(Product_Name);
            free(Product_Category);
        }

        res->close();
        stmt->close();
        delete res;
        delete stmt;

        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * FROM oras");

        while (res->next())
        {
            sql::SQLString sqlstr;

            int iCity_ID = res->getInt("ID_Oras");
            char *cCity_ID = (char *)malloc(to_string(iCity_ID).length() + 1);
            strcpy(cCity_ID, to_string(iCity_ID).c_str());

            sqlstr = res->getString("Denumire_Oras");
            char *City_Name = (char *)malloc(sqlstr.asStdString().length() + 1);
            strcpy(City_Name, sqlstr.asStdString().c_str());

            sqlstr = res->getString("Tip_Depozit");
            char *City_Type = (char *)malloc(sqlstr.asStdString().length() + 1);    // City_Type = the type of depot (local / central)
            strcpy(City_Type, sqlstr.asStdString().c_str());

            double latitude = res->getDouble("latitudine");
            double longitude = res->getDouble("longitudine");

            city.getData(cCity_ID, City_Name, City_Type, latitude, longitude);

            contor_noduri_graf++;

            free(cCity_ID);
            free(City_Name);
            free(City_Type);
        }

        res->close();
        stmt->close();

        delete res;
        delete stmt;
    }
    catch (sql::SQLException &e)
    {
        cerr << "\n"
             << setw(5) << " "
             << "Error code: " << e.getErrorCode() << endl
             << setw(5) << " "
             << "Error message: " << e.what() << endl
             << setw(5) << " "
             << "SQLState: " << e.getSQLState() << endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

bool addGraphEdge(void)
{
    clear_screen();
    cout << "\n\n";

    afisare_date_tabel_oras();

    cout << setw(5) << " "
         << "Enter the city edges ('0' to return):\n";

    underline(80, true);

    ofstream edgeFile;
    edgeFile.open("utils/legaturi.txt", std::ios::out | std::ios::app);
    if (!edgeFile.is_open())
    {
        cerr << setw(5) << " "
             << "--> Failed to open the edge file!\n";
        _getch();
        return EXIT_FAILURE;
    }

    cin.ignore(9999, '\n');
    while (true)
    {
        string edge;
        int City_ID = -1;
        bool isFound = false;

        cout << setw(5) << " " << denumire_depozit_nou << " <--> ";
        std::getline(cin, edge);

        if (_strcasecmp_(edge.c_str(), "0") == 0)
            break;

        for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
            if (_strcasecmp_(date_oras->getCityName(), edge.c_str()) == 0)
            {
                isFound = true;
                City_ID = stoi(date_oras->getCityID());
                break;
            }

        if (isFound)
            edgeFile << contor_noduri_graf - 1 << " " << City_ID << "\n";
        else
            cerr << setw(5) << " " << "Legatura invalida!\n\n";
    }
    
    _init_();

    edgeFile.close();

    return EXIT_SUCCESS;
}

// TO DO
bool SQL_Data_Update(const int input)
{
    if (input == 1)
    {
        if (CITY::addCity() == EXIT_FAILURE)
        {
            cerr << setw(5) << " "
                 << "sql-> Failed to add city!\n";
            _getch();
            return EXIT_FAILURE;
        }
        else
        {
            city.~CITY();
            city.head_city = nullptr;
            city.tail_city = nullptr;

            product.~PRODUCT();
            product.head_product = nullptr;
            product.head_product = nullptr;

            depot.~DEPOT();
            depot.head_depot = nullptr;
            depot.tail_depot = nullptr;

            contor_noduri_graf = 0;

            if (fetchTables() == EXIT_FAILURE)
            {
                _getch();
                return EXIT_FAILURE;
            }

            if (addGraphEdge() == EXIT_FAILURE)
            {
                cerr << setw(5) << " " << "Failed to add edge!\n";
                _getch();
                return EXIT_FAILURE;
            }
                
        }
    }
    else if (input == 2)
    {
        if (CITY::deleteCity() == EXIT_FAILURE)
        {
            cerr << setw(5) << " "
                 << "SQL: Failed to delete row!\n";
            _getch();
        }
        else
        {
            for (unsigned int i = 0; i < contor_noduri_graf; i++)
            {
                matrice_drum[contor_noduri_graf - 1][i].distanta = matrice_drum[i][contor_noduri_graf - 1].distanta = 0.0;
                matrice_drum[contor_noduri_graf - 1][i].durata = matrice_drum[i][contor_noduri_graf - 1].durata = 0;
            }

            ofstream fisier("utils/legaturi.txt");
            if (!fisier.is_open())
            {
                cerr << setw(5) << " " << "Failed to open file!\n";
                _getch();
                return EXIT_FAILURE;
            }

            for (unsigned int i = 0; i < contor_noduri_graf; i++)
                for (unsigned int j = 0; j < contor_noduri_graf; j++)
                    if (matrice_drum[i][j].distanta != 0 && i < j)
                        fisier << j << " " << i << "\n";

            fisier.close();

            city.~CITY();
            city.head_city = nullptr;
            city.tail_city = nullptr;

            product.~PRODUCT();
            product.head_product = nullptr;
            product.tail_product = nullptr;

            depot.~DEPOT();
            depot.head_depot = nullptr;
            depot.tail_depot = nullptr;

            contor_noduri_graf = 0;

            if (fetchTables() == EXIT_FAILURE)
            {
                _getch();
                return EXIT_FAILURE;
            }
        }
    }

    return EXIT_SUCCESS;
}