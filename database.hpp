/*
                                TO DO
    ****************FIX mem leak from res ptr*************
*/

#ifndef DATABASE
#define DATABASE

#include "string.h"
#include "declarations.hpp"
#include "logic.hpp"
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace sql;
using namespace std;

bool accesareDate()
{
    try
    {
        Driver *driver;
        Connection *con;
        Statement *stmt;
        ResultSet *res;

        driver = get_driver_instance();
        con = driver->connect("tcp://localhost:3306", "root", "Sorin!2005");

        /*
         * -------------------------------------------------Scoate comm la sfarsit---------------------------------------------
        con = driver->connect("tcp://" + 
                    string(autentificare.get_nod()->host_name), 
                    string(autentificare.get_nod()->username), 
                    string(autentificare.get_nod()->parola));
        */
        
        if (con == nullptr)
        {
            delete res;
            delete stmt;
            delete con;
            return EXIT_FAILURE;
        }
            
        con->setSchema("MyDepoChain");

        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * FROM date");

        while (res->next())
        {
            string str;
            SQLString sqlstr;

            int idProdus = res->getInt("ID_Produs");
            str = to_string(idProdus);
            char *tempIdProdus = (char *)malloc(str.length() + 1);
            strcpy(tempIdProdus, str.c_str());

            int idProdusDepozit = res->getInt("ID_Depozit");
            str = to_string(idProdusDepozit);
            char *tempIDProdusDepozit = (char *)malloc(str.length() + 1);
            strcpy(tempIDProdusDepozit, str.c_str());

            int idOras = res->getInt("ID_Oras");
            str = to_string(idOras);
            char *tempIdOras = (char *)malloc(str.length() + 1);
            strcpy(tempIdOras, str.c_str());

            double tempCantitate_Produs = res->getDouble("Cantitate_Produs");

            depozit.inserareDateDepozit(tempIdProdus, tempIDProdusDepozit, tempIdOras, tempCantitate_Produs);

            free(tempIdProdus);
            free(tempIDProdusDepozit);
            free(tempIdOras);
        }

        res->close();
        stmt->close();
        stmt = con->createStatement();
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

            produs.inserareDateProdus(tempIdProdus, tempNumeProdus, tempCantegorieProdus, pret_produs);

            free(tempIdProdus);
            free(tempNumeProdus);
            free(tempCantegorieProdus);
        }

        res->close();
        stmt->close();
        stmt = con->createStatement();
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

            sqlstr = res->getString("Tip_Depozit");
            str = sqlstr.asStdString();
            char *tempTipDepozit = (char *)malloc(str.length() + 1);
            strcpy(tempTipDepozit, str.c_str());

            double tempLat = res->getDouble("latitudine");
            double tempLong = res->getDouble("longitudine");

            oras.insearareDateOrase(tempIdOras, tempDenumireOras, tempTipDepozit, tempLat, tempLong);

            free(tempIdOras);
            free(tempDenumireOras);
            free(tempTipDepozit);
        }

        res->close();
        stmt->close();

        delete res;
        delete stmt;
        delete con;
    }
    catch (SQLException &e)
    {
        cout << "Error code: " << e.getErrorCode() << endl;
        cout << "Error message: " << e.what() << endl;
        cout << "SQLState: " << e.getSQLState() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

#endif