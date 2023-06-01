#ifndef DATABASE
#define DATABASE

#include "declarations.hpp"
#include "logic.hpp"

using namespace sql;
using namespace std;

bool accesareDate(void)
{
    try
    {
        Driver *driver;
        Connection *con;
        Statement *stmt;
        ResultSet *res;

        driver = get_driver_instance();

        con = driver->connect("tcp://" + string(autentificare.get_nod()->host_name),
                              string(autentificare.get_nod()->username),
                              string(autentificare.get_nod()->parola));

        if (con == nullptr)
        {
            delete res;
            delete stmt;
            delete con;
            return EXIT_FAILURE;
        }

        con->setSchema("SpeedyGo");

        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * FROM depozit");

        while (res->next())
        {
            string str;
            SQLString sqlstr;

            int idProdus = res->getInt("ID_Produs");
            str = to_string(idProdus);
            char *tempIdProdus = (char *)malloc(str.length() + 1);
            strcpy(tempIdProdus, str.c_str());

            int idOras = res->getInt("ID_Oras");
            str = to_string(idOras);
            char *tempIdOras = (char *)malloc(str.length() + 1);
            strcpy(tempIdOras, str.c_str());

            double tempCantitate_Produs = res->getDouble("Cantitate_Produs");

            depozit.inserareDateDepozit(tempIdProdus, tempIdOras, tempCantitate_Produs);

            free(tempIdProdus);
            free(tempIdOras);
        }

        res->close();
        stmt->close();
        delete res;
        delete stmt;

        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * FROM produs");

        while (res->next())
        {
            SQLString sqlstr;
            string str;

            int idProdus = res->getInt("ID_Produs");
            str = to_string(idProdus);
            char *tempIdProdus = (char *)malloc(str.length() + 1);
            strcpy(tempIdProdus, str.c_str());

            sqlstr = res->getString("Denumire_Produs");
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
        delete res;
        delete stmt;

        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * FROM oras");

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

            contor_noduri_graf++;

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
        cout << "\n"
             << setw(5) << " "
             << "Error code: " << e.getErrorCode() << endl;
        cout << setw(5) << " "
             << "Error message: " << e.what() << endl;
        cout << setw(5) << " "
             << "SQLState: " << e.getSQLState() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

bool update_database(void)
{
    Driver *driver;
    Connection *con;

    driver = mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://" + string(autentificare.get_nod()->host_name),
                          string(autentificare.get_nod()->username),
                          string(autentificare.get_nod()->parola));
    con->setSchema("SpeedyGo");

    if (con == nullptr)
    {
        delete con;
        return EXIT_FAILURE;
    }

    string deleteQuery = "DELETE FROM depozit";
    Statement *stmt = con->createStatement();
    stmt->executeUpdate(deleteQuery);
    delete stmt;

    string insertQuery = "INSERT INTO depozit (ID_Produs, ID_Oras, Cantitate_Produs) VALUES (?, ?, ?)";
    PreparedStatement *prepStmt = con->prepareStatement(insertQuery);

    for (DEPOZIT::NOD_DEPOZIT *date_depozit = depozit.getHead(); date_depozit != nullptr; date_depozit = date_depozit->next)
    {
        int ID_Produs = stoi(date_depozit->ID_Produs);
        int ID_Oras = stoi(date_depozit->ID_Oras);
        int Cantitate_Produs = date_depozit->Cantitate_Produs;

        prepStmt->setInt(1, ID_Produs);
        prepStmt->setInt(2, ID_Oras);
        prepStmt->setInt(3, Cantitate_Produs);

        prepStmt->executeUpdate();
    }

    delete con;
    delete prepStmt;

    return EXIT_SUCCESS;
}

#endif