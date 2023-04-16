#ifndef DATABASE
#define DATABASE

#include "string.h"
#include "declarations.hpp"
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace sql;
using namespace std;

void accesareDate()
{
    try
    {
        Driver *driver;
        Connection *con;
        Statement *stmt;
        ResultSet *res;

        driver = get_driver_instance();
        con = driver->connect("tcp://localhost:3306", "root", "Sorin!2005");
        con->setSchema("MyDepoChain");

        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * FROM data");

        while (res->next())
        {
            string str;

            int idProdus = res->getInt("ID_Produs");
            str = to_string(idProdus);

            char *tempIdProdus = (char *)malloc(str.length() + 1);
            strcpy(tempIdProdus, str.c_str());

            int idProdusDepozit = res->getInt("ID_Depozit");
            str = to_string(idProdusDepozit);

            char *tempIDProdusDepozit = (char *)malloc(str.length() + 1);
            strcpy(tempIDProdusDepozit, str.c_str());

            double tempCantitate_Produs = res->getDouble("Cantitate_Produs");

            inserareDateDepozit(tempIdProdus, tempIDProdusDepozit, tempCantitate_Produs);
        }

        res = stmt->executeQuery("SELECT * FROM date_produse");

        while (res->next())
        {
            SQLString sqlstr;
            string str;

            int idProdus = res->getInt("ID_Produs");
            str = to_string(idProdus);
            char *tempIdProdus = (char *)malloc(str.length() + 1);
            strcpy(tempIdProdus, str.c_str());

            sqlstr = res->getString("Nume_Produs");
            str = sqlstr.asStdString();
            char *tempNumeProdus = (char *)malloc(str.length() + 1);
            strcpy(tempNumeProdus, str.c_str());

            sqlstr = res->getString("Categorie_Produs");
            str = sqlstr.asStdString();
            char *tempCantegorieProdus = (char *)malloc(str.length() + 1);
            strcpy(tempCantegorieProdus, str.c_str());

            double pret_produs = res->getDouble("Pret_Produs");

            inserareDateProduse(tempIdProdus, tempNumeProdus, tempCantegorieProdus, pret_produs);
        }

        res = stmt->executeQuery("SELECT * FROM date_produs_local");

        while (res->next())
        {
            string str;

            int idOras = res->getInt("ID_Oras");
            str = to_string(idOras);
            char *tempIdOras = (char *)malloc(str.length() + 1);
            strcpy(tempIdOras, str.c_str());

            int idProdus = res->getInt("ID_Produs");
            str = to_string(idProdus);
            char *tempIdProdus = (char *)malloc(str.length() + 1);
            strcpy(tempIdProdus, str.c_str());

            double cantitate_produs_local = res->getDouble("Cantitate_Produs");

            inserareDateProduseLocal(tempIdOras, tempIdProdus, cantitate_produs_local);
        }

        res = stmt->executeQuery("SELECT * FROM orase");

        while (res->next())
        {
            SQLString sqlstr;
            string str;

            int idOras = res->getInt("ID_Oras");
            str = to_string(idOras);
            char *tempIdOras = (char *)malloc(str.length() + 1);
            strcpy(tempIdOras, str.c_str());

            sqlstr = res->getString("Denumire_Oras");
            str = sqlstr.asStdString();
            char *tempDenumireOras = (char *)malloc(str.length() + 1);
            strcpy(tempDenumireOras, str.c_str());

            double tempLat = res->getDouble("latitudine");
            double tempLong = res->getDouble("longitudine");

            insearareDateOrase(tempIdOras, tempDenumireOras, tempLat, tempLong);
        }   

        delete res;
        delete stmt;
        delete con;
    }
    catch (SQLException &e)
    {
        cout << e.what() << endl;
    }
}

#endif