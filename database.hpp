#ifndef DATABASE
#define DATABASE

#include <iostream>
#include <string.h>
#include "declarations.hpp"
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace sql;
using namespace std;

NOD_PRODUS_DEPOZIT *head_depozit = nullptr;
NOD_DETALII_PRODUS *head_produs = nullptr;

void inserareDateDepozit(char *vID_Produs, char *vID_Depozit, double vCantitate_Produs)
{
    NOD_PRODUS_DEPOZIT *newnod = (NOD_PRODUS_DEPOZIT *)malloc(sizeof(NOD_PRODUS_DEPOZIT));

    newnod->ID_Produs = strdup(vID_Produs);
    newnod->ID_Depozit = strdup(vID_Depozit);
    newnod->Cantitate_Produs = vCantitate_Produs;

    if (head_depozit != nullptr)
    {
        head_depozit->prev_d = newnod;
        newnod->next_d = head_depozit;
        head_depozit->prev_d = newnod;
    }

    head_depozit = newnod;
}

void inserareDateProduse(char *vID_Produs, char *vDenumire_Produs, char *vCategorie_Produs, double vPret_Produs)
{
    NOD_DETALII_PRODUS *newnod = (NOD_DETALII_PRODUS *)malloc(sizeof(NOD_DETALII_PRODUS));

    newnod->ID_Produs = strdup(vID_Produs);
    newnod->Denumire_Produs = strdup(vDenumire_Produs);
    newnod->Categorie_Produs = strdup(vCategorie_Produs);
    newnod->pret_produs = vPret_Produs;

    if (head_produs != nullptr)
    {
        head_produs->prev_p = newnod;
        newnod->next_p = head_produs;
        head_produs->prev_p = newnod;
    }

    head_produs = newnod;
}

void afisareDateDepozit()
{
    NOD_PRODUS_DEPOZIT *ptr;
    ptr = head_depozit;
    while (ptr != nullptr)
    {
        cout << "ID_Produs: " << ptr->ID_Produs << ", ";
        cout << "ID_Depozit: " << ptr->ID_Depozit << ", ";
        cout << "Cantitate_Produs: " << ptr->Cantitate_Produs << endl;
        ptr = ptr->next_d;
    }
}

void afisareDateProdus()
{
    NOD_DETALII_PRODUS *ptr;
    ptr = head_produs;
    while (ptr != nullptr)
    {
        cout << "ID_Produs: " << ptr->ID_Produs << " ";
        cout << "Nume_Produs: " << ptr->Denumire_Produs << endl;
        ptr = ptr->next_p;
    }
}

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

            char *tempIdProdus = (char *)malloc(str.length());
            strcpy(tempIdProdus, str.c_str());

            int idProdusDepozit = res->getInt("ID_Depozit");
            str = to_string(idProdusDepozit);

            char *tempIDProdusDepozit = (char *)malloc(str.length());
            strcpy(tempIDProdusDepozit, str.c_str());

            double tempCantitate_Produs = res->getDouble("Cantitate_Produs");

            // cout << tempIdProdus << " " << tempIDProdusDepozit << " " << tempCantitate_Produs << endl;

            inserareDateDepozit(tempIdProdus, tempIDProdusDepozit, tempCantitate_Produs);
        }

        res = stmt->executeQuery("SELECT * FROM date_produse");

        while (res->next())
        {
            SQLString sqlstr;
            string str;

            int idProdus = res->getInt("ID_Produs");
            str = to_string(idProdus);
            char *tempIdProdus = (char *)malloc(str.length());
            strcpy(tempIdProdus, str.c_str());

            sqlstr = res->getString("Nume_Produs");
            str = sqlstr.asStdString();
            char *tempNumeProdus = (char *)malloc(str.length());
            strcpy(tempNumeProdus, str.c_str());

            sqlstr = res->getString("Categorie_Produs");
            str = sqlstr.asStdString();
            char *tempCantegorieProdus = (char *)malloc(str.length());
            strcpy(tempCantegorieProdus, str.c_str());

            double pret_produs = res->getDouble("Pret_Produs");

            inserareDateProduse(tempIdProdus, tempNumeProdus, tempCantegorieProdus, pret_produs);
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