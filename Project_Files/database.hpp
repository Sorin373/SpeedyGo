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
        cerr << "\n"
             << setw(5) << " "
             << "Error code: " << e.getErrorCode() << endl;
        cerr << setw(5) << " "
             << "Error message: " << e.what() << endl;
        cerr << setw(5) << " "
             << "SQLState: " << e.getSQLState() << endl;

        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

#pragma region Database_Update
bool update_database(void)
{
    try
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
    catch (SQLException &e)
    {
        cerr << "\n"
             << setw(5) << " "
             << "Error code: " << e.getErrorCode() << endl;
        cerr << setw(5) << " "
             << "Error message: " << e.what() << endl;
        cerr << setw(5) << " "
             << "SQLState: " << e.getSQLState() << endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

bool adaugare_depozit(void)
{
    char *Denumire_Depozit = (char *)malloc(MAXL * sizeof(char) + 1);
    char *Tip_Depozit = (char *)malloc(MAXL * sizeof(char) + 1);
    double lat = 0.0, lon = 0.0;

    clear_screen();

    cout << "\n\n"
         << setw(16) << " "
         << "Introduceti datele noului depozit\n";
    underline(60, true);

    cout << setw(5) << " "
         << "ID depozit: " << contor_noduri_graf << "\n";

    cout << setw(5) << " "
         << "Denumire depozit: ";
    cin.get();
    cin.get(Denumire_Depozit, MAXL);
    strcpy(denumire_depozit_nou, Denumire_Depozit);

    cout << setw(5) << " "
         << "Tip depozit:\n\n";
    cout << setw(5) << " "
         << "[1] Local / [2] Centralizat\n";

    cout << setw(5) << " "
         << "I: ";
    char *input = (char *)malloc(MAXL * sizeof(char) + 1);
    cin >> input;

    if (strcasecmp(input, "1") == 0)
        strcpy(Tip_Depozit, "local");
    else if (strcasecmp(input, "2") == 0)
        strcpy(Tip_Depozit, "centralizat");

    clear_screen();

    cout << "\n\n"
         << setw(16) << " "
         << "Introduceti datele noului depozit\n";
    underline(60, true);

    cout << setw(5) << " "
         << "ID depozit: " << contor_noduri_graf << "\n"
         << setw(5) << " "
         << "Denumire depozit: " << Denumire_Depozit << "\n"
         << setw(5) << " "
         << "Tip depozit: " << Tip_Depozit << "\n";

    cout << setw(5) << " "
         << "Latitudine: ";
    cin >> lat;
    cout << setw(5) << " "
         << "Longitudine: ";
    cin >> lon;

    underline(60, true);

    cout << setw(5) << " "
         << "Pentru a finaliza tastati '1', iar '0' pentru a anulat: ";
    cin >> input;

    if (strcasecmp(input, "0") == 0)
        return EXIT_FAILURE;
    else if (strcasecmp(input, "1") == 0)
    {
        try
        {
            cout << "\n"
                 << setw(5) << " "
                 << "Start...\n";
            sleepcp(1000);

            Driver *driver;
            Connection *con;
            Statement *stmt;

            driver = get_driver_instance();
            con = driver->connect("tcp://" + string(autentificare.get_nod()->host_name),
                                  string(autentificare.get_nod()->username),
                                  string(autentificare.get_nod()->parola));

            if (con == nullptr)
            {
                delete stmt;
                delete con;
                return EXIT_FAILURE;
            }

            con->setSchema("SpeedyGo");

            string table_name = "oras";
            string query = "INSERT INTO " + table_name + " (ID_Oras, Denumire_Oras, latitudine, longitudine, Tip_Depozit) VALUES (" + to_string(contor_noduri_graf) + ", '" + Denumire_Depozit + "', " + to_string(lat) + ", " + to_string(lon) + ", '" + Tip_Depozit + "')";
            stmt = con->createStatement();
            stmt->execute(query);
            stmt->close();

            delete stmt;
            delete con;

            cout << setw(5) << " "
                 << "Success...\n";
            sleepcp(1000);

            return EXIT_SUCCESS;
        }
        catch (SQLException &e)
        {
            cerr << "\n"
                 << setw(5) << " "
                 << "Error code: " << e.getErrorCode() << endl;
            cerr << setw(5) << " "
                 << "Error message: " << e.what() << endl;
            cerr << setw(5) << " "
                 << "SQLState: " << e.getSQLState() << endl;

            return EXIT_FAILURE;
        }
    }

    free(Denumire_Depozit);
    free(Tip_Depozit);
    free(input);

    return EXIT_SUCCESS;
}

bool stergere_depozit(void)
{
    clear_screen();

    afisare_date_tabel_oras();

    string ID;
    cout << setw(5) << " "
         << "Introduceti ID-ul corespunzator ('EXIT' pentru a anula): ";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, ID);

    const char* cStrID = ID.c_str();

    if (strcasecmp(cStrID, "EXIT") == 0)
        return EXIT_SUCCESS;

    try
    {
        cout << "\n"
             << setw(5) << " "
             << "Start...\n";
        sleepcp(1000);

        Driver *driver;
        Connection *con;
        Statement *stmt;

        driver = get_driver_instance();
        con = driver->connect("tcp://" + string(autentificare.get_nod()->host_name),
                              string(autentificare.get_nod()->username),
                              string(autentificare.get_nod()->parola));

        if (con == nullptr)
        {
            delete stmt;
            delete con;
            return EXIT_FAILURE;
        }

        con->setSchema("SpeedyGo");

        string table_name = "oras";
        string query = "DELETE FROM " + table_name + " WHERE ID_Oras = " + ID;
        stmt = con->createStatement();
        stmt->execute(query);
        stmt->close();

        delete stmt;
        delete con;

        cout << setw(5) << " "
             << "Success...\n";
        sleepcp(1000);

        return EXIT_SUCCESS;
    }
    catch (SQLException &e)
    {
        cerr << "\n"
             << setw(5) << " "
             << "Error code: " << e.getErrorCode() << endl;
        cerr << setw(5) << " "
             << "Error message: " << e.what() << endl;
        cerr << setw(5) << " "
             << "SQLState: " << e.getSQLState() << endl;

        return EXIT_FAILURE;
    }
}

void legaturi_graf(void)
{
    clear_screen();
    cout << "\n\n";

    afisare_date_tabel_oras();

    cout << setw(5) << " "
         << "Introduceti legaturile depozitului ('0' pentru a te intoarce):\n";
    underline(80, true);

    cout << setw(5) << " " << denumire_depozit_nou << " <--> ";

    char *legatura = (char *)malloc(MAXL * sizeof(char) + 1);
    int ID_Legatura = 0;
    bool gasit = false;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get(legatura, MAXL);

    for (ORAS::NOD_ORAS *date_oras = oras.getHead(); date_oras != nullptr; date_oras = date_oras->next)
        if (strcasecmp(date_oras->denumire_oras, legatura) == 0)
        {
            gasit = true;
            ID_Legatura = stoi(date_oras->ID_Oras);
            break;
        }

    ofstream fisier;
    fisier.open("legaturi.txt", ios::out | ios::app);
    if (!fisier.is_open())
    {
        cerr << setw(5) << " "
             << "Failed to open file!";
        getch();
        return;
    }

    if (gasit)
        fisier << "\n" << contor_noduri_graf - 1 << " " << ID_Legatura << "\n";
    else
        cerr << setw(5) << " " << "Legatura invalida!\n\n";

    while (true)
    {
        gasit = false;
        cout << setw(5) << " " << denumire_depozit_nou << " <--> ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get(legatura, MAXL);

        if (strcasecmp(legatura, "0") == 0)
            break;

        for (ORAS::NOD_ORAS *date_oras = oras.getHead(); date_oras != nullptr; date_oras = date_oras->next)
            if (strcasecmp(date_oras->denumire_oras, legatura) == 0)
            {
                gasit = true;
                ID_Legatura = stoi(date_oras->ID_Oras);
                break;
            }

        if (gasit)
            fisier << contor_noduri_graf - 1 << " " << ID_Legatura << "\n";
        else
            cerr << setw(5) << " " << "Legatura invalida!\n\n";
    }

    if (load_data("legaturi.txt") == EXIT_FAILURE)
    {
        cerr << setw(5) << " " << "Failed to update matrix!\n";
        getch();
        return; 
    }

    fisier.close();
    free(legatura);
}

bool SQL_Data_Update(const int input)
{
    if (input == 1)
    {
        if (adaugare_depozit() == EXIT_FAILURE)
        {
            cerr << setw(5) << " "
                 << "SQL: Failed to add row!\n";
            getch();
        }
        else
        {
            oras.~ORAS();
            oras.head_oras = nullptr;
            oras.tail_oras = nullptr;

            produs.~DETALII_PRODUS();
            produs.head_produs = nullptr;
            produs.tail_produs = nullptr;

            depozit.~DEPOZIT();
            depozit.head_depozit = nullptr;
            depozit.tail_depozit = nullptr;

            contor_noduri_graf = 0;
            accesareDate();
            legaturi_graf();
        }
    }
    else if (input == 2)
    {
        if (stergere_depozit() == EXIT_FAILURE)
        {
            cerr << setw(5) << " "
                 << "SQL: Failed to delete row!\n";
            getch();
        }
        else
        {
            for (unsigned int i = 0; i < contor_noduri_graf; i++)
            {
                matrice_drum[contor_noduri_graf - 1][i].distanta = matrice_drum[i][contor_noduri_graf - 1].distanta = 0.0;
                matrice_drum[contor_noduri_graf - 1][i].durata = matrice_drum[i][contor_noduri_graf - 1].durata = 0;
            }

            ofstream fisier("legaturi.txt");
            if (!fisier.is_open())
            {
                cerr << setw(5) << " " << "Failed to open file!\n";
                getch();
                return EXIT_FAILURE;
            }

            for (unsigned int i = 0; i < contor_noduri_graf; i++)
                for (unsigned int j = 0; j < contor_noduri_graf; j++)
                    if (matrice_drum[i][j].distanta != 0 && i < j)
                        fisier << j << " " << i << "\n";

            fisier.close();

            oras.~ORAS();
            oras.head_oras = nullptr;
            oras.tail_oras = nullptr;

            produs.~DETALII_PRODUS();
            produs.head_produs = nullptr;
            produs.tail_produs = nullptr;

            depozit.~DEPOZIT();
            depozit.head_depozit = nullptr;
            depozit.tail_depozit = nullptr;

            contor_noduri_graf = 0;
            accesareDate();

            return EXIT_SUCCESS;
        }
    }
    return EXIT_SUCCESS;
}
#pragma endregion

#endif