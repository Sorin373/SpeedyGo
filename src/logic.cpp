#include "../include/logic.hpp"
#include "../include/declarations.hpp"
#include "../include/database.hpp"
#include "../include/GoogleMatrixAPI.hpp"
#include "../include/haversine.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <vector>
#include <cmath>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::fixed;
using std::ifstream;
using std::numeric_limits;
using std::ofstream;
using std::setprecision;
using std::setw;
using std::stoi;
using std::string;
using std::swap;
using std::to_string;
using std::vector;

bool autentificare_cont(void)
{
    clear_screen();

    char *host_name = (char *)malloc(MAXL * sizeof(char) + 1),
         *username = (char *)malloc(MAXL * sizeof(char) + 1),
         *database = (char *)malloc(MAXL * sizeof(char) + 1),
         *password = (char *)malloc(MAXL * sizeof(char) + 1);

    cout << "\n\n"
         << setw(10) << " "
         << "CONEXIUNE LA BAZA DE DATE\n"
         << setw(4) << " "
         << "======================================\n"
         << setw(5) << " "
         << "Hostname: ";
    cin >> host_name;

    cout << setw(5) << " "
         << "Username: ";
    cin >> username;

    cout << setw(5) << " "
         << "Password: ";
    cin >> password;

    cout << setw(5) << " "
         << "Database name: ";
    cin >> database;

    cout << setw(4) << " "
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
        return autentificare_cont();
    }

    free(host_name);
    free(username);
    free(password);
    free(database);

    return EXIT_SUCCESS;
}

bool _init_(void)
{
    if (_GPS_UPDATE_DATA_() == EXIT_FAILURE)
    {
        cout << setw(5) << " "
             << "-- Initialization of the Google API service could not be completed!\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        ERROR_CNT++;

        if (load_data("utils/legaturi.txt") == EXIT_FAILURE)
        {
            cout << setw(5) << " "
                 << "-- Manual calculation using the Haversine formula was unsuccessful during initialization!\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            ERROR_CNT++;

            return EXIT_FAILURE;
        }
    }

    for (ADJACENCY_MATRIX_INITIALIZER::ADJACENCY_MATRIX_INITIALIZER_NODE *date_gps = adjacency_matrix_init.getHead(); date_gps != nullptr; date_gps = date_gps->next)
    {
        char *city1 = (char *)malloc(MAXL * sizeof(char) + 1), *city2 = (char *)malloc(MAXL * sizeof(char) + 1);
        int city1_ID = 0, city2_ID = 0, durata = date_gps->getDuration();;
        double distanta = date_gps->getDistance();

        strcpy(city1, date_gps->getStart());
        strcpy(city2, date_gps->getDestination());

        for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
            if (_strcasecmp_(date_oras->getCityName(), city1) == 0)
            {
                city1_ID = stoi(date_oras->getCityID());
                break;
            }

        for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
            if (_strcasecmp_(date_oras->getCityName(), city2) == 0)
            {
                city2_ID = stoi(date_oras->getCityID());
                break;
            }

        adjacencyMatrix[city1_ID][city2_ID].distance = adjacencyMatrix[city2_ID][city1_ID].distance = distanta;
        adjacencyMatrix[city1_ID][city2_ID].duration = adjacencyMatrix[city2_ID][city1_ID].duration = durata;

        free(city1);
        free(city2);
    }

    return EXIT_SUCCESS;
}

void nr_max_caractere_den(void)
{
    for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
    {
        if (strlen(date_oras->getCityName()) > cmax_denumire_orase)
            cmax_denumire_orase = strlen(date_oras->getCityName());

        char temp[MAXL];
        sprintf(temp, "%f", date_oras->getLatitude());

        for (unsigned int i = 0; i < strlen(temp); i++)
            if (temp[i] == '.')
                temp[i] = '\0';

        if (strlen(temp) > cmax_lat_oras)
            cmax_lat_oras = strlen(temp);
    }

    for (PRODUCT::PRODUCT_NODE *date_produs = product.getHead(); date_produs != nullptr; date_produs = date_produs->next)
    {
        if (strlen(date_produs->getProductName()) > cmax_denumire_produse)
            cmax_denumire_produse = strlen(date_produs->getProductName());

        if (strlen(date_produs->getProductCategory()) > cmax_categorie_produse)
            cmax_categorie_produse = strlen(date_produs->getProductCategory());

        char *pret = (char *)malloc(MAXL * sizeof(char) + 1);
        sprintf(pret, "%f", date_produs->getProductPrice());

        for (unsigned int i = 0; i < strlen(pret); i++)
            if (pret[i] == '.')
                pret[i] = '\0';

        if (strlen(pret) > cmax_pret_produse)
            cmax_pret_produse = strlen(pret);

        if (strlen(date_produs->getProductID()) > cmax_ID_produs)
            cmax_ID_produs = strlen(date_produs->getProductID());

        free(pret);
    }

    for (DEPOT::DEPOT_NODE *date_depozit = depot.getHead(); date_depozit != nullptr; date_depozit = date_depozit->next)
    {
        int cantitiate_temp = date_depozit->getProductQuantity(), contor_cifre = 0;

        while (cantitiate_temp > 0)
        {
            cantitiate_temp /= 10;
            contor_cifre++;
        }
        if (contor_cifre > cmax_cantitate_produs)
            cmax_cantitate_produs = contor_cifre;
    }

    int temp = VERTEX_COUNT;
    while (temp)
    {
        temp /= 10;
        cmax_ID_Oras++;
    }

    cmax_lat_oras += 3;
    cmax_pret_produse += 3;
}

void afisare_date_tabel_oras(void)
{
    clear_screen();

    cout << "\n\n"
         << setw(5) << " "
         << "+---------------+\n"
         << setw(5) << " "
         << "| TABEL-DEPOT |\n"
         << setw(5) << " "
         << "+---------------+\n\n";

    cout << setw(5) << " "
         << "ID_Oras"
         << setw(5) << " "
         << "Denumire_Oras"
         << setw(5) << " "
         << "Tip_Depozit"
         << setw(5) << " "
         << "Latitudine"
         << setw(5) << " "
         << "Longitudine\n";
    underline(80, true);

    for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
    {
        cout << setw(5 + 1) << " [" << date_oras->getCityID() << "]" << setw(cmax_ID_Oras - strlen(date_oras->getCityID()) + 8)
             << " " << date_oras->getCityName() << setw(cmax_denumire_orase - strlen(date_oras->getCityName()) + 4)
             << " " << date_oras->getCityType() << setw(11 - strlen(date_oras->getCityType()) + 5)
             << " " << fixed << setprecision(2) << date_oras->getLatitude();
#ifdef _WIN32
        cout << "\370";
#elif __linux__
        cout << "\u00B0";
#endif
        cout << setw(cmax_lat_oras - to_string(round(date_oras->getLatitude())).length() + 13)
             << " " << date_oras->getLongitude();
#ifdef _WIN32
        cout << "\370" << endl;
#elif __linux__
        cout << "\u00B0\n";
#endif
    }

    underline(80, true);
}

void afisare_date_tabel_depozit(void)
{
    DEPOT::DEPOT_NODE *ptr = depot.getHead();
    while (ptr != nullptr)
    {

        cout << "ID_Produs: " << ptr->getProductID() << ", "
             << "Cantitate_Produs: " << ptr->getProductQuantity() << ", "
             << "ID_oras: " << ptr->getCityID() << endl;

        ptr = ptr->next;
    }
}

void afisare_date_tabel_produs(void)
{
    clear_screen();

    cout << "\n\n"
         << setw(5) << " "
         << "+--------------+\n"
         << setw(5) << " "
         << "| TABEL-PRODUS |\n"
         << setw(5) << " "
         << "+--------------+\n\n";

    cout << setw(5) << " "
         << "ID_Produs"
         << setw(4) << " "
         << "Denumire_Produs"
         << setw(cmax_denumire_produse - 10) << " "
         << "Categorie_Produs"
         << setw(5) << " "
         << "Pret_Produs\n";

    underline(80, true);

    for (PRODUCT::PRODUCT_NODE *date_produs = product.getHead(); date_produs != nullptr; date_produs = date_produs->next)
    {
        cout << setw(5 + 1) << " [" << date_produs->getProductID() << "]" << setw(cmax_ID_produs - strlen(date_produs->getProductID()) + 9)
             << " " << date_produs->getProductName() << setw(cmax_denumire_produse - strlen(date_produs->getProductName()) + 5)
             << " " << date_produs->getProductCategory() << setw(cmax_categorie_produse - strlen(date_produs->getProductCategory()) + 10) << " ";

        char *pret = (char *)malloc(MAXL * sizeof(char) + 1);
        snprintf(pret, MAXL, "%g", date_produs->getProductPrice());

        cout << fixed << setprecision(2) << date_produs->getProductPrice()
             << setw(cmax_pret_produse - to_string(round(date_produs->getProductPrice())).length() + 10) << "RON\n";

        free(pret);
    }

    underline(80, true);
}

bool verificare_orase_stoc_limitat(void)
{
    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
        if (limitedStockCities[i])
            return true;
    return false;
}

void cautare_produse_ID_stoc_limitat(const int ID_Depozit)
{
    clear_screen();

    cout << "\n\n"
         << setw(5) << " "
         << "ID_Produs" << setw(4) << " "
         << "Denumire_Produs" << setw(8) << " "
         << "Nr.Buc\n";
    underline(55, true);

    for (PRODUCT::PRODUCT_NODE *date_produs = product.getHead(); date_produs != nullptr; date_produs = date_produs->next)
    {
        int ID_PRODUS = stoi(date_produs->getProductID());
        for (DEPOT::DEPOT_NODE *date_depozit = depot.getHead(); date_depozit != nullptr; date_depozit = date_depozit->next)
        {
            int ID_PRODUS_DEPOZIT = stoi(date_depozit->getProductID()), ID_DEPOZIT = stoi(date_depozit->getCityID());
            if (ID_PRODUS_DEPOZIT == ID_PRODUS && ID_DEPOZIT == ID_Depozit)
            {
                if (date_depozit->getProductQuantity() < VAL_STOC_MINIM)
                    cout << setw(5 + 1) << " [" << date_produs->getProductID() << "]" << setw(cmax_ID_produs - strlen(date_produs->getProductID()) + 9)
                         << " " << date_produs->getProductName()
                         << setw(cmax_denumire_produse - strlen(date_produs->getProductName()) + 3) << " " << date_depozit->getProductQuantity()
                         << setw(cmax_cantitate_produs - to_string(round(date_produs->getProductPrice())).length() + 5) << " "
                         << "buc.\n";
            }
        }
    }

    underline(55, true);

    cout << setw(5) << " "
         << "Apasa 'ENTER' pentru a te intoarce...";
}

void depozite_conectate(int ID_Depozit)
{
    cout << "\n";

    vector<bool> temp_depozite(VERTEX_COUNT, false);
    CITY::CITY_NODE *date_oras = city.getHead();
    char *t_denumire = (char *)malloc(MAXL * sizeof(char) + 1);

    while (date_oras != nullptr)
    {
        int _ID = stoi(date_oras->getCityID());
        if (_ID == ID_Depozit)
            strcpy(t_denumire, date_oras->getCityName());
        date_oras = date_oras->next;
    }

    int contor = 0;
    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
        if (adjacencyMatrix[ID_Depozit][i].distance != 0)
            temp_depozite[i] = true;

    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
        if (temp_depozite[i] == true)
        {
            date_oras = city.getHead();
            while (date_oras != nullptr)
            {
                int _ID = stoi(date_oras->getCityID());
                if (_ID == i && _strcasecmp_(t_denumire, date_oras->getCityName()) != 0)
                {
                    cout << setw(5) << " " << t_denumire << " -> " << date_oras->getCityName()
                         << setw(cmax_denumire_orase - strlen(date_oras->getCityName()) + 5)
                         << " (" << adjacencyMatrix[ID_Depozit][i].distance << "km | " << adjacencyMatrix[ID_Depozit][i].duration << "min)\n";
                    break;
                }
                date_oras = date_oras->next;
            }
        }

    cout << "\n"
         << setw(5) << " "
         << "Apasa 'ENTER' pentru a te intoarce...";

    free(t_denumire);
}

void determinare_tip_depozit(void)
{
    CITY::CITY_NODE *date_oras = city.getHead();
    while (date_oras != nullptr)
    {
        if (_strcasecmp_(date_oras->getCityType(), "centralizat") == 0)
        {
            int ID = stoi(date_oras->getCityID());
            centralDepos[ID] = true;
        }
        date_oras = date_oras->next;
    }
}

void cautare_orase_stoc_limitat(void)
{
    contor_orase_stoc_limitat = 0;
    CITY::CITY_NODE *date_oras = city.getHead();
    while (date_oras != nullptr)
    {
        int _ID_Oras = stoi(date_oras->getCityID());

        DEPOT::DEPOT_NODE *date_depozit = depot.getHead();
        while (date_depozit != nullptr)
        {
            int _ID_Depozit = stoi(date_depozit->getCityID());
            if (_ID_Depozit == _ID_Oras && centralDepos[_ID_Depozit] == false)
                if (date_depozit->getProductQuantity() < VAL_STOC_MINIM)
                {
                    limitedStockCities[_ID_Depozit] = true;
                    contor_orase_stoc_limitat++;
                    break;
                }
            date_depozit = date_depozit->next;
        }
        date_oras = date_oras->next;
    }
}

void cautare_orase_izolate(void)
{
    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
    {
        bool izolat = true;
        for (unsigned int j = 0; j < VERTEX_COUNT && izolat; j++)
            if (adjacencyMatrix[i][j].distance > 0)
                izolat = false;
        if (izolat)
            isolatedVertex[i] = true;
    }
}

void vizualizare_status_stoc(void)
{
    cautare_orase_stoc_limitat();

    if (contor_orase_stoc_limitat == 0)
    {
        cout << "\n"
             << setw(5) << " "
             << "Nu sunt depozite care necesita aprovizionare...";
        _getch();
        return;
    }

    clear_screen();

    cout << "\n\n"
         << setw(5) << " "
         << "+-----------------------+\n"
         << setw(6) << " "
         << "Orase cu stocuri reduse\n"
         << setw(5) << " "
         << "+-----------------------+\n";

    underline(40, true);

    CITY::CITY_NODE *date_oras = city.getHead();
    int cmax = 0;

    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
        if (limitedStockCities[i])
        {
            date_oras = city.getHead();
            while (date_oras != nullptr)
            {
                int _ID = stoi(date_oras->getCityID());
                if (_ID == i)
                    cout << setw(5 + 1) << " [" << date_oras->getCityID() << "] " << date_oras->getCityName()
                         << setw(cmax_denumire_orase - strlen(date_oras->getCityName()) + 5) << " ";
                date_oras = date_oras->next;
            }
            cout << "\n";
        }

    underline(40, true);

#ifdef _WIN32
    changeText(FOREGROUND_INTENSITY);
    cout << setw(5) << " "
         << "Scrieti 'exit' pentru a iesi\n\n";
    resetText();
#elif __linux__
    cout << setw(5) << " "
         << "\033[3m"
         << "Scrieti 'exit' pentru a iesi\n\n"
         << "\033[0m";
#endif

    char *t_ID_Oras = (char *)malloc(MAXL * sizeof(char) + 1);
    cout << setw(5) << " "
         << "Introduceti ID-ul orasului: ";
    cin >> t_ID_Oras;

    if (_strcasecmp_(t_ID_Oras, "exit") == 0)
    {
        free(t_ID_Oras);
        return;
    }
    else
    {
        int _ID_Oras = stoi(t_ID_Oras);
        free(t_ID_Oras);
        date_oras = city.getHead();

        while (date_oras != nullptr)
        {
            int t_ID = stoi(date_oras->getCityID());
            if (t_ID == _ID_Oras && limitedStockCities[t_ID] == true)
            {
                unsigned int sMENIU;

                do
                {
                    clear_screen();

                    cout << "\n\n"
                         << setw(5) << " "
                         << "+--------------------------------------------+\n"
                         << setw(7) << " " << date_oras->getCityName() << " | Tip depot: " << date_oras->getCityName() << "\n"
                         << setw(5) << " "
                         << "+--------------------------------------------+\n";

                    underline(50, true);

                    cout << setw(5) << " "
                         << "[1] Vizualizare produse cu stoc limitat\n"
                         << setw(5) << " "
                         << "[2] Vizualizare conexiuni cu alte depozite\n"
                         << setw(5) << " "
                         << "[0] Inapoi\n";

                    underline(50, true);

                    cout << setw(5) << " "
                         << "Introduceti numarul meniului: ";
                    cin >> sMENIU;

                    switch (sMENIU)
                    {
                    case 1:
                        cautare_produse_ID_stoc_limitat(_ID_Oras);
                        _getch();
                        break;
                    case 2:
                        depozite_conectate(_ID_Oras);
                        _getch();
                        break;

                    default:
                        break;
                    }

                } while (sMENIU != 0);

                break;
            }
            date_oras = date_oras->next;
        }
        vizualizare_status_stoc();
    }
}

#pragma region Dijkstra
void creare_solutie_distanta(int start, vector<double> &distanta, vector<int> &distanta_minima, bool afisare, bool creare_trasee)
{
    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
    {
        if (i != start)
        {
            if (afisare)
                cout << "Cea mai scurta distanta de la " << start << " la " << i << " este: " << distanta[i] << " : traseu: ";

            vector<int> traseu;

            int nod = i;

            while (nod != -1)
            {
                traseu.push_back(nod);
                nod = distanta_minima[nod];
            }

            reverse(traseu.begin(), traseu.end());

            if (creare_trasee)
                route.getData(start, i, distanta[i], traseu);

            if (afisare)
                for (unsigned int j = 0; j < traseu.size(); j++)
                    cout << traseu[j] << " ";

            if (afisare)
                cout << "\n";
        }
    }
}

void dijkstra(int start, vector<double> &distanta, vector<int> &distanta_minima)
{
    vector<bool> visited(VERTEX_COUNT, false);
    distanta[start] = 0.0;

    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
    {
        int min_index = 0;
        double min_dist = numeric_limits<double>::infinity();

        for (unsigned int j = 0; j < VERTEX_COUNT; j++)
            if (!visited[j] && distanta[j] < min_dist)
            {
                min_index = j;
                min_dist = distanta[j];
            }

        visited[min_index] = true;

        for (unsigned int j = 0; j < VERTEX_COUNT; j++)
        {
            double distanta_noua = distanta[min_index] + adjacencyMatrix[min_index][j].distance;

            if (!visited[j] && adjacencyMatrix[min_index][j].distance > 0 && distanta_noua < distanta[j])
            {
                distanta[j] = distanta_noua;
                distanta_minima[j] = min_index;
            }
        }
    }
}

void afisare_depozite_centralizare(void)
{
    cout << "\n"
         << setw(5) << " "
         << "Depozite centralizate\n";
    underline(40, true);

    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
    {
        if (centralDepos[i])
            for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
            {
                int ID = stoi(date_oras->getCityID());
                if (ID == i)
                {
                    cout << setw(5 + 1) << " [" << ID << "] " << date_oras->getCityName() << endl;
                    break;
                }
            }
    }

    underline(40, true);
}

void afisare_trasee_optime(const int _ID, const int vStart)
{
    bool gasit = false;

    if (isolatedVertex[_ID] == true)
    {
        cout << "\n"
             << setw(5) << " "
             << "Nu exista traseul cu acest ID. Depozit izolat...";
        return;
    }

    for (ROUTE::ROUTE_NODE *date_traseu = route.getHead(); date_traseu != nullptr; date_traseu = date_traseu->next)
    {
        if (date_traseu->getDestination() == _ID && date_traseu->getStart() == vStart)
        {
            gasit = true;
            cout << "\n"
                 << setw(5) << " "
                 << "Distanta: ";
            cout << date_traseu->getDistance() << "km\n"
                 << setw(5) << " ";

            const vector<int> &traseu = date_traseu->getRoute();

            for (unsigned int i = 0; i < traseu.size(); i++)
                for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
                {
                    int ID = stoi(date_oras->getCityID());

                    if (ID == traseu[i])
                    {
                        cout << date_oras->getCityName();
                        // corectare afisare '->'
                        if (i != traseu.size() - 1)
                            cout << " --> ";
                        break;
                    }
                }
            break;
        }
    }

    if (!gasit)
    {
        cout << "\n"
             << setw(5) << " "
             << "Nu exista traseul cu acest ID!";
        return;
    }
}

void afisare_optiuni_trasee_optime(const int vStart)
{
    clear_screen();

    cout << "\n\n"
         << setw(5) << " "
         << "SCRIE 'exit' PENTRU A TE INTOARCE...\n";

    underline(45, true);

    char *oras_start = (char *)malloc(MAXL * sizeof(char) + 1);

    for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
    {
        int ID = stoi(date_oras->getCityID());
        if (ID == vStart)
        {
            strcpy(oras_start, date_oras->getCityName());
            break;
        }
    }

    for (ROUTE::ROUTE_NODE *date_traseu = route.getHead(); date_traseu != nullptr; date_traseu = date_traseu->next)
    {
        if (date_traseu->getStart() == vStart)
        {
            for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
            {
                int ID = stoi(date_oras->getCityID());
                if (ID == date_traseu->getDestination())
                {
                    cout << setw(5 + 1) << " [" << date_oras->getCityID() << "] ";
                    cout << oras_start << " -> " << date_oras->getCityName() << endl;
                    break;
                }
            }
        }
    }

    underline(45, true);

    char *_ID = (char *)malloc(MAXL * sizeof(char) + 1);
    cout << setw(5) << " "
         << "Introduceti ID-ul corespunzator: ";
    cin >> _ID;
    if (_strcasecmp_(_ID, "exit") == 0)
    {
        free(_ID);
        free(oras_start);
        return;
    }
    else
    {
        free(oras_start);

        int _ID_temp = stoi(_ID);

        free(_ID);

        afisare_trasee_optime(_ID_temp, vStart);
        _getch();
        afisare_optiuni_trasee_optime(vStart);
    }
}

void sistem_aprovizionare_independent(void)
{
    if (!trasee)
    {
        for (unsigned int i = 0; i < VERTEX_COUNT; i++)
        {
            vector<int> distanta_minima(VERTEX_COUNT, -1);
            vector<double> distanta(VERTEX_COUNT, numeric_limits<double>::infinity());

            if (centralDepos[i])
            {
                dijkstra(i, distanta, distanta_minima);
                if (!trasee)
                    creare_solutie_distanta(i, distanta, distanta_minima, false, true);
                else
                {
                    trasee = true;
                    creare_solutie_distanta(i, distanta, distanta_minima, false, false);
                }
            }
            distanta_minima.clear();
            distanta.clear();
        }
        trasee = true;
    }

    clear_screen();

    cout << "\n\n"
         << setw(5) << " "
         << "SCRIE 'exit' PENTRU A TE INTOARCE...\n";

    afisare_depozite_centralizare();

    char *_ID = (char *)malloc(MAXL * sizeof(char) + 1);
    cout << setw(5) << " "
         << "Introduceti ID-ul corespunzator: ";
    cin >> _ID;
    if (_strcasecmp_(_ID, "exit") == 0)
    {
        free(_ID);
        return;
    }
    else
    {
        int _ID_temp = stoi(_ID);

        if (centralDepos[_ID_temp])
        {
            free(_ID);
            afisare_optiuni_trasee_optime(_ID_temp);
            sistem_aprovizionare_independent();
        }
        else
        {
            cout << "\n"
                 << setw(5) << " "
                 << "Nu exista depozitul central cu acest ID...";
            free(_ID);
            _getch();
            sistem_aprovizionare_independent();
        }
    }
}
#pragma endregion

void afisare_depozite_izolate(void)
{
    clear_screen();

    cout << "\n\n"
         << setw(5) << " "
         << "+------------------+\n"
         << setw(5) << " "
         << "| DEPOZITE IZOLATE |\n"
         << setw(5) << " "
         << "+------------------+\n\n";

    cout << setw(5) << " "
         << "ID_Oras"
         << setw(5) << " "
         << "Denumire_Oras"
         << setw(5) << " "
         << "Tip_Depozit"
         << setw(5) << " "
         << "Latitudine"
         << setw(5) << " "
         << "Longitudine\n";

    underline(75, true);

    int cmax = -1;
    for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
    {
        if (strlen(date_oras->getCityName()) > cmax)
            cmax = strlen(date_oras->getCityName());
    }

    bool gasit = false;
    for (unsigned int i = 0; i < isolatedVertex.size(); i++)
        if (isolatedVertex[i] == true)
        {
            gasit = true;
            for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
            {
                int ID = stoi(date_oras->getCityID());
                if (ID == i)
                {
                    cout << setw(5 + 1) << " [" << date_oras->getCityID() << "]" << setw(cmax_ID_Oras - strlen(date_oras->getCityID()) + 8)
                         << " " << date_oras->getCityName() << setw(cmax_denumire_orase - strlen(date_oras->getCityName()) + 4)
                         << " " << date_oras->getCityType() << setw(11 - strlen(date_oras->getCityType()) + 5)
                         << " " << fixed << setprecision(2) << date_oras->getLatitude();
#ifdef _WIN32
                    cout << "\370";
#elif __linux__
                    cout << "\u00B0";
#endif
                    cout << setw(cmax_lat_oras - to_string(round(date_oras->getLatitude())).length() + 13)
                         << " " << date_oras->getLongitude();
#ifdef _WIN32
                    cout << "\370" << endl;
#elif __linux__
                    cout << "\u00B0\n";
#endif
                    break;
                }
            }
        }

    underline(75, true);

    if (!gasit)
    {
        cout << setw(5) << " "
             << "Nu exista depozite izolate...";
        return;
    }
    else
        cout << "\n"
             << setw(5) << " "
             << " Apasati 'ENTER' pentru a iesi...";
}

void afisare_depozite_unic_drum(void)
{
    clear_screen();

    cout << "\n\n";
    cout << setw(5) << " "
         << "+-------------------------------+\n";
    cout << setw(5) << " "
         << "| DEPOZITE CU O UNICA CONEXIUNE |\n";
    cout << setw(5) << " "
         << "+-------------------------------+\n\n";

    cout << setw(5) << " "
         << "ID_Oras"
         << setw(5) << " "
         << "Denumire_Oras"
         << setw(5) << " "
         << "Tip_Depozit"
         << setw(5) << " "
         << "Latitudine"
         << setw(5) << " "
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
        for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
        {
            if (strlen(date_oras->getCityName()) > cmax)
                cmax = strlen(date_oras->getCityName());
        }

        for (unsigned int i = 0; i < oneEdgeVertex.size(); i++)
        {
            if (oneEdgeVertex[i])
            {
                for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
                {
                    int ID = stoi(date_oras->getCityID());
                    if (ID == i)
                    {
                        cout << setw(5 + 1) << " [" << date_oras->getCityID() << "]" << setw(cmax_ID_Oras - strlen(date_oras->getCityID()) + 8)
                             << " " << date_oras->getCityName() << setw(cmax_denumire_orase - strlen(date_oras->getCityName()) + 4)
                             << " " << date_oras->getCityType() << setw(11 - strlen(date_oras->getCityType()) + 5)
                             << " " << fixed << setprecision(2) << date_oras->getLatitude();
#ifdef _WIN32
                        cout << "\370";
#elif __linux__
                        cout << "\u00B0";
#endif
                        cout << setw(cmax_lat_oras - to_string(round(date_oras->getLatitude())).length() + 13)
                             << " " << date_oras->getLongitude();
#ifdef _WIN32
                        cout << "\370" << endl;
#elif __linux__
                        cout << "\u00B0\n";
#endif
                        break;
                    }
                }
            }
        }
        underline(75, true);

        cout << "\n"
             << setw(5) << " "
             << " Apasati 'ENTER' pentru a iesi...";
    }
    else
    {
        cout << setw(5) << " "
             << "Nu exista depozite cu o unica conexiune...";
        return;
    }
}

#pragma region backtracking
void init_stiva_hc(void)
{
    stack[contor_stiva] = -1;
}

bool succesor_hc(void)
{
    if (stack[contor_stiva] < VERTEX_COUNT - 1)
    {
        stack[contor_stiva]++;
        return true;
    }
    return false;
}

bool solutie_hc(void)
{
    if (contor_stiva == contor_orase_stoc_limitat)
        return true;
    return false;
}

bool valid_hc(void)
{
    for (unsigned int i = 1; i < contor_stiva; i++)
        if (stack[contor_stiva] == stack[i])
            return false;

    if (contor_stiva > 1)
        if (adjacencyMatrix[stack[contor_stiva]][stack[contor_stiva - 1]].distance == 0)
            return false;

    if (contor_stiva > 1)
        if (centralDepos[stack[1]] == false)
            return false;

    return true;
}

void determinare_ciclu_hc_minim(void)
{
    double suma_dist = 0.0;
    int suma_durata = 0;

    for (int i = 1; i <= contor_stiva; i++)
    {
        suma_dist += adjacencyMatrix[stack[i]][stack[i + 1]].distance;
        suma_durata += adjacencyMatrix[stack[i]][stack[i + 1]].duration;
    }

    if (suma_dist < cost_minim_TSP || (suma_dist == cost_minim_TSP && suma_durata < durata_minima_TSP))
    {
        cost_minim_TSP = suma_dist;
        durata_minima_TSP = suma_durata;

        for (int i = 1; i <= contor_stiva; i++)
        {
            minimumRouteTSP[i] = stack[i];
            contor_traseu_TSP = contor_stiva;
        }
    }
}

void back_hc(void)
{
    contor_stiva = 1;
    init_stiva_hc();
    while (contor_stiva > 0)
    {
        int vSuccesor, vValid;
        do
        {
            vSuccesor = succesor_hc();
            if (vSuccesor == 1)
                vValid = valid_hc();
        } while (vSuccesor == 1 && vValid == 0);
        if (vSuccesor == 1)
        {
            if (solutie_hc() == 1)
                determinare_ciclu_hc_minim();
            else
            {
                contor_stiva++;
                init_stiva_hc();
            }
        }
        else
            contor_stiva--;
    }
}

void init_stiva_ac(void)
{
    stack[contor_stiva] = -1;
}

bool succesor_ac(void)
{
    if (stack[contor_stiva] < VERTEX_COUNT - 1)
    {
        stack[contor_stiva]++;
        return true;
    }
    return false;
}

bool solutie_ac(void)
{
    if (contor_stiva == VERTEX_COUNT + 1)
        return true;
    return false;
}

bool valid_ac(void)
{
    if (contor_stiva == VERTEX_COUNT + 1)
    {
        for (unsigned int i = 0; i < VERTEX_COUNT; i++)
        {
            bool gasit = false;
            if (limitedStockCities[i] == true && !isolatedVertex[i])
            {
                for (unsigned int j = 1; j <= contor_stiva; j++)
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

    if (contor_stiva > 1)
        if (adjacencyMatrix[stack[contor_stiva]][stack[contor_stiva - 1]].distance == 0)
            return false;

    if (contor_stiva > 1)
        if (centralDepos[stack[1]] == false)
            return false;

    return true;
}

void determinare_traseu_minim(void)
{
    double suma_dist = 0.0;
    int suma_durata = 0;

    for (int i = 1; i < contor_stiva; i++)
    {
        suma_dist += adjacencyMatrix[stack[i]][stack[i + 1]].distance;
        suma_durata += adjacencyMatrix[stack[i]][stack[i + 1]].duration;
    }

    if (suma_dist < cost_minim_TSP || (suma_dist == cost_minim_TSP && suma_durata < durata_minima_TSP))
    {
        cost_minim_TSP = suma_dist;
        durata_minima_TSP = suma_durata;

        for (int i = 1; i <= contor_stiva; i++)
        {
            minimumRouteTSP[i] = stack[i];
            contor_traseu_TSP = contor_stiva;
        }
    }
}

void back_ac(void)
{
    int vSuccesor, vValid;
    contor_stiva = 1;
    init_stiva_ac();
    while (contor_stiva > 0)
    {
        do
        {
            vSuccesor = succesor_ac();
            if (vSuccesor == 1)
                vValid = valid_ac();
        } while (vSuccesor == 1 && vValid == 0);
        if (vSuccesor == 1)
            if (solutie_ac() == 1)
                determinare_traseu_minim();
            else
            {
                contor_stiva++;
                init_stiva_ac();
            }
        else
            contor_stiva--;
    }
}
#pragma endregion

#pragma region TSP
void TSP(void)
{
#ifdef _WIN32

    changeText(FOREGROUND_INTENSITY | FOREGROUND_BLUE);

    cout << setw(5) << " "
         << "Se calculeaza traseul cel mai optim...\n";

    resetText();
#elif __linux__
    cout << "\033[1;34m" << setw(5) << " "
         << "Se calculeaza traseul cel mai optim..."
         << "\033[0m" << endl;
#endif

    bool izolat = false;
    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
        if (isolatedVertex[i] == true)
        {
            izolat = true;
            break;
        }

    if (!izolat)
    {
        back_hc();
        clear_screen();
        cout << "\n";

        if (!minimumRouteTSP.empty())
        {
#ifdef _WIN32
            changeText(FOREGROUND_INTENSITY);

            cout << setw(5) << " "
                 << "Lungime traseu: " << cost_minim_TSP << "km | "
                 << " Durata traseu: " << durata_minima_TSP << "min" << endl
                 << setw(5) << " ";

            resetText();
#elif __linux__
            cout << setw(5) << " "
                 << "\033[1m"
                 << "Lungime traseu: " << cost_minim_TSP << "km | "
                 << "Durata traseu: " << durata_minima_TSP << "min\n"
                 << setw(5) << " "
                 << "\033[0m";
#endif

            for (unsigned int i = 1; i <= contor_traseu_TSP; i++)
            {
                for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
                {
                    int ID = stoi(date_oras->getCityID());
                    if (ID == minimumRouteTSP[i])
                    {
                        cout << date_oras->getCityName();
                        if (i < contor_traseu_TSP)
                            cout << " --> ";
                        break;
                    }
                }
            }

            cout << "\n";
            underline(190, false);
        }
        else
        {
            minimumRouteTSP.clear();
            stack.clear();

            back_ac();
            clear_screen();
            cout << "\n";

            if (!minimumRouteTSP.empty())
            {
#ifdef _WIN32
                changeText(FOREGROUND_INTENSITY);

                cout << setw(5) << " "
                     << "Lungime traseu: " << cost_minim_TSP << "km | "
                     << " Durata traseu: " << durata_minima_TSP << "min" << endl
                     << setw(5) << " ";

                resetText();
#elif __linux__
                cout << setw(5) << " "
                     << "\033[1m"
                     << "Lungime traseu: " << cost_minim_TSP << "km | "
                     << "Durata traseu: " << durata_minima_TSP << "min\n"
                     << setw(5) << " "
                     << "\033[0m";
#endif

                for (unsigned int i = 1; i <= contor_traseu_TSP; i++)
                {
                    for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
                    {
                        int ID = stoi(date_oras->getCityID());
                        if (ID == minimumRouteTSP[i])
                        {
                            cout << date_oras->getCityName();
                            if (i < contor_traseu_TSP)
                                cout << " --> ";
                            break;
                        }
                    }
                }
                cout << "\n";
                underline(190, false);
            }
            else
                cout << "Toate depozitele sunt izolate!";
        }
    }
    else
    {
        back_ac();
        clear_screen();

        cout << "\n";
        if (!minimumRouteTSP.empty())
        {
#ifdef _WIN32
            changeText(FOREGROUND_INTENSITY);

            cout << setw(5) << " "
                 << "Lungime traseu: " << cost_minim_TSP << "km | "
                 << " Durata traseu: " << durata_minima_TSP << "min" << endl
                 << setw(5) << " ";

            resetText();
#elif __linux__
            cout << setw(5) << " "
                 << "\033[1m"
                 << "Lungime traseu: " << cost_minim_TSP << "km | "
                 << "Durata traseu: " << durata_minima_TSP << "min\n"
                 << setw(5) << " "
                 << "\033[0m";
#endif

            for (unsigned int i = 1; i <= contor_traseu_TSP; i++)
            {
                for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
                {
                    int ID = stoi(date_oras->getCityID());
                    if (ID == minimumRouteTSP[i])
                    {
                        cout << date_oras->getCityName();
                        if (i < contor_traseu_TSP)
                            cout << " --> ";
                        break;
                    }
                }
            }
            cout << "\n";
            underline(190, false);
        }
        else
            cout << "\n"
                 << setw(5) << " "
                 << "Toate depozitele sunt izolate!";
    }
}

void produse_transport_TSP(void)
{
    for (PRODUCT::PRODUCT_NODE *date_produs = product.getHead(); date_produs != nullptr; date_produs = date_produs->next)
    {
        int cantitate = 0;
        int ID_PRODUS = stoi(date_produs->getProductID());
        for (DEPOT::DEPOT_NODE *date_depozit = depot.getHead(); date_depozit != nullptr; date_depozit = date_depozit->next)
        {
            int ID_DEPOZIT = stoi(date_depozit->getCityID());
            if (limitedStockCities[ID_DEPOZIT] == true && isolatedVertex[ID_DEPOZIT] == false && centralDepos[ID_DEPOZIT] == false)
            {
                int ID_PRODUS_DEPOZIT = stoi(date_depozit->getProductID());
                if (ID_PRODUS == ID_PRODUS_DEPOZIT)
                    cantitate += VAL_STOC_MAXIM - date_depozit->getProductQuantity();
            }
        }
        supply.getData(date_produs->getProductID(), cantitate);
    }
}

void pagina_principala_TSP(void)
{
    clear_screen();

    distanta_parcursa = 0.0;
    durata_parcursa = 0;

    cout << "\n";
    if (!minimumRouteTSP.empty())
    {
        cout << setw(5) << " "
             << "Lungime traseu: " << cost_minim_TSP << "km | "
             << "Durata traseu: " << durata_minima_TSP << "min\n"
             << setw(5) << " ";
        for (unsigned int i = 1; i <= contor_traseu_TSP; i++)
        {
            for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
            {
                int ID = stoi(date_oras->getCityID());

                if (i == 1)
                    if (ID == minimumRouteTSP[1])
                    {
#ifdef _WIN32
                        changeText(FOREGROUND_INTENSITY | COMMON_LVB_UNDERSCORE | FOREGROUND_RED | FOREGROUND_GREEN);
                        cout << date_oras->getCityName();
                        resetText();

                        cout << " --> ";
#elif __linux__
                        cout << "\033[4m"
                             << "\033[1m" << date_oras->denumire_oras << "\033[0m"
                             << " --> ";
#endif
                    }

                if (ID == minimumRouteTSP[i] && i != 1)
                {
                    cout << date_oras->getCityName();
                    if (i < contor_traseu_TSP)
                        cout << " --> ";
                    break;
                }
            }
        }
    }
    cout << "\n";
    underline(190, false);

    cout << "\n";
    for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
    {
        int ID = stoi(date_oras->getCityID());
        if (ID == minimumRouteTSP[1])
        {
            cout << setw(5) << " "
                 << "+-----------------------+\n"
                 << setw(8) << " [" << date_oras->getCityID() << "] " << date_oras->getCityName() << " - START\n"
                 << setw(5) << " "
                 << "+-----------------------+";
            break;
        }
    }

#ifdef _WIN32
    changeText(FOREGROUND_INTENSITY);
    cout << setw(100) << " "
         << "Distanta parcursa: " << distanta_parcursa;
    resetText();
    cout << "km | ";
    changeText(FOREGROUND_INTENSITY);
    cout << "Durata de calatorie: " << durata_parcursa;
    resetText();
    cout << "min\n";

#elif __linux__
    cout << setw(100) << " "
         << "\033[1m"
         << "Distanta parcursa: "
         << "\033[0m" << distanta_parcursa << "km | "
         << "\033[1m"
         << "Durata de calatorie: "
         << "\033[0m" << durata_parcursa << "min\n";
#endif

    cout << "\n\n"
         << setw(5) << " "
         << "ID" << setw(7) << " "
         << "Den.Produs" << setw(15) << " "
         << "Cnt.transport_totala\n";
    underline(65, true);

    for (SUPPLY::SUPPLY_NODE *supply_data = supply.getHead(); supply_data != nullptr; supply_data = supply_data->next)
    {
        int ID_AP = stoi(supply_data->getID());
        cout << setw(5 + 1) << " [" << ID_AP << "] " << setw(5) << " ";
        for (PRODUCT::PRODUCT_NODE *date_produs = product.getHead(); date_produs != nullptr; date_produs = date_produs->next)
        {
            int ID_P = stoi(date_produs->getProductID());
            if (ID_P == ID_AP)
            {
                cout << date_produs->getProductName() << setw(cmax_denumire_produse - strlen(date_produs->getProductName()) + 5) << " ";
                break;
            }
        }
        cout << supply_data->getQuantity() << " buc.\n";
    }
    underline(65, true);

    cout << "\n\n";
    underline(190, false);

    cout << setw(5) << " "
         << "[1] PREV" << setw(80) << " -" << pagina << "- " << setw(80) << " "
         << "[2] NEXT\n";

    underline(190, false);

    distanta_parcursa += adjacencyMatrix[minimumRouteTSP[1]][minimumRouteTSP[2]].distance;
    durata_parcursa += adjacencyMatrix[minimumRouteTSP[1]][minimumRouteTSP[2]].duration;
}

void pagina_finala_TSP(void)
{
    ifstream get_contor("utils/contor_TSP_log.txt");
    if (!get_contor.is_open())
    {
        cerr << setw(5) << " "
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

    traseu_completat = true;
    logCounter++;

    ofstream file("utils/contor_TSP_log.txt");
    if (!file.is_open())
    {
        cerr << setw(5) << " "
             << "Failed to open file!\n";
        _getch();
    }
    else
        file << logCounter;

    file.close();

    if (!depot.refreshData())
        cerr << setw(5) << " "
             << "Could not update the database!\n";
    else if (fetchTables() == EXIT_FAILURE)
        _getch();

    ofstream log_out;
    log_out.open("logs/TSP_log.txt", std::ios::app);

    if (!log_out.is_open())
    {
        cerr << setw(5) << " "
             << "Failed to open TSP log!\n";
        _getch();
    }
    else
    {
        string s(500, '=');
        log_out << "LOG [" << logCounter << "]\n";

        for (unsigned int i = 1; i <= contor_traseu_TSP; i++)
        {
            for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
            {
                int ID = stoi(date_oras->getCityID());
                if (ID == minimumRouteTSP[i])
                {
                    log_out << date_oras->getCityName();
                    if (i < contor_traseu_TSP)
                        log_out << " --> ";
                    break;
                }
            }
        }

        log_out << "\n"
                << s << "\n"
                << "Distanta parcursa: " << cost_minim_TSP << "km\n"
                << "Cantitate totala transportata: " << cantitate_totala_aprovizionata << "BUC.\n"
                << "Cost total: " << cost_aprovizionare_total << "RON\nEND-LOG\n"
                << s << "\n\n\n";

        log_out.close();
    }

    clear_screen();

    cout << "\n";
    underline(50, true);

    cout << setw(5) << " "
         << "-> Aprovizionare completa!\n"
         << setw(5) << " "
         << "-> Baza de date reinprostpatata!\n"
         << setw(5) << " "
         << "-> Mai multe detalii in fisierul TSP_log.txt!\n";

    underline(50, true);

    cout << setw(5) << " "
         << "Apasati 'ENTER' pentru a va intoarce...";

    _getch();
}

void pagina_stanga_TSP(void)
{
    clear_screen();

    if (pagina - 1 > 1)
    {
        pagina--;

        distanta_parcursa -= adjacencyMatrix[minimumRouteTSP[pagina]][minimumRouteTSP[pagina + 1]].distance;
        durata_parcursa -= adjacencyMatrix[minimumRouteTSP[pagina]][minimumRouteTSP[pagina + 1]].duration;

        cout << "\n";
        if (!minimumRouteTSP.empty())
        {
            cout << setw(5) << " "
                 << "Lungime traseu: " << cost_minim_TSP << " | "
                 << "Durata traseu: " << durata_minima_TSP << "min\n"
                 << setw(5) << " ";

            for (unsigned int i = 1; i <= contor_traseu_TSP; i++)
            {
                for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
                {
                    int ID = stoi(date_oras->getCityID());

                    if (ID == minimumRouteTSP[pagina] && i == pagina)
                    {
#ifdef _WIN32
                        changeText(FOREGROUND_INTENSITY | COMMON_LVB_UNDERSCORE | FOREGROUND_RED | FOREGROUND_GREEN);

                        cout << date_oras->getCityName();

                        resetText();
#elif __linux__
                        cout << "\033[4m"
                             << "\033[1m" << date_oras->denumire_oras << "\033[0m"
                             << " --> ";
#endif
                        if (i < contor_traseu_TSP)
                            cout << " --> ";
                        break;
                    }

                    if (ID == minimumRouteTSP[i] && i != pagina)
                    {
                        cout << date_oras->getCityName();
                        if (i < contor_traseu_TSP)
                            cout << " --> ";
                        break;
                    }
                }
            }
        }
        cout << "\n";
        underline(190, false);
        cout << "\n";

        for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
        {
            int ID = stoi(date_oras->getCityID());
            if (ID == minimumRouteTSP[pagina])
            {
                cout << setw(5) << " "
                     << "+-----------------------+\n"
                     << setw(8) << " [" << date_oras->getCityID() << "] " << date_oras->getCityName() << "\n"
                     << setw(5) << " "
                     << "+-----------------------+";
                break;
            }
        }

#ifdef _WIN32
        changeText(FOREGROUND_INTENSITY);
        cout << setw(100) << " "
             << "Distanta parcursa: " << distanta_parcursa;
        resetText();
        cout << "km | ";
        changeText(FOREGROUND_INTENSITY);
        cout << "Durata de calatorie: " << durata_parcursa;
        resetText();
        cout << "min\n";

#elif __linux__
        cout << setw(100) << " "
             << "\033[1m"
             << "Distanta parcursa: "
             << "\033[0m" << distanta_parcursa << "km | "
             << "\033[1m"
             << "Durata de calatorie: "
             << "\033[0m" << durata_parcursa << "min\n";
#endif

        cout << "\n\n"
             << setw(5) << " "
             << "ID" << setw(10) << " "
             << "Den.Produs" << setw(15) << " "
             << "Cnt.transport/Cnt.ramasa\n";
        underline(70, true);

        if (centralDepos[minimumRouteTSP[pagina]] == false)
        {
            if (limitedStockCities[minimumRouteTSP[pagina]] == false)
            {
#ifdef _WIN32
                changeText(FOREGROUND_INTENSITY);

                cout << setw(5) << " "
                     << "Depozitul a fost aprovizionat!" << endl;

                resetText();
#elif __linux__
                cout << setw(5) << " "
                     << "\033[1m"
                     << "Depozitul a fost aprovizionat!\n"
                     << "\033[0m";
#endif
            }
        }
        else
        {
#ifdef _WIN32
            changeText(FOREGROUND_INTENSITY);

            cout << setw(5) << " "
                 << "Depozit centralizat!!" << endl;

            resetText();
#elif __linux__
            cout << setw(5) << " "
                 << "\033[1m"
                 << "Depozit centralizat!\n"
                 << "\033[0m";
#endif
        }

        underline(70, true);

        cout << "\n\n";
        underline(190, false);

        cout << setw(5) << " "
             << "[1] PREV" << setw(80) << " -" << pagina << "- " << setw(80) << " "
             << "[2] NEXT\n";

        underline(190, false);
    }
    else
    {
        pagina = 1;
        pagina_principala_TSP();
    }
}

void pagina_dreapta_TSP(void)
{
    clear_screen();

    if (pagina < contor_traseu_TSP)
    {
        if (pagina > 2)
        {
            distanta_parcursa += adjacencyMatrix[minimumRouteTSP[pagina]][minimumRouteTSP[pagina + 1]].distance;
            durata_parcursa += adjacencyMatrix[minimumRouteTSP[pagina]][minimumRouteTSP[pagina + 1]].duration;
        }

        pagina++;

        cout << "\n";
        if (!minimumRouteTSP.empty())
        {
            cout << setw(5) << " "
                 << "Lungime traseu: " << cost_minim_TSP << "km | "
                 << "Durata traseu: " << durata_minima_TSP << "min\n"
                 << setw(5) << " ";

            for (unsigned int i = 1; i <= contor_traseu_TSP; i++)
            {
                for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
                {
                    int ID = stoi(date_oras->getCityID());

                    if (ID == minimumRouteTSP[pagina] && i == pagina)
                    {
#ifdef _WIN32
                        changeText(FOREGROUND_INTENSITY | COMMON_LVB_UNDERSCORE | FOREGROUND_RED | FOREGROUND_GREEN);

                        cout << date_oras->getCityName();

                        resetText();
#elif __linux__
                        cout << "\033[4m"
                             << "\033[1m" << date_oras->denumire_oras << "\033[0m"
                             << " --> ";
#endif
                        if (i < contor_traseu_TSP)
                            cout << " --> ";
                        break;
                    }

                    if (ID == minimumRouteTSP[i] && i != pagina)
                    {
                        cout << date_oras->getCityName();
                        if (i < contor_traseu_TSP)
                            cout << " --> ";
                        break;
                    }
                }
            }
        }
        cout << "\n";
        underline(190, false);
        cout << "\n";

        for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
        {
            int ID = stoi(date_oras->getCityID());
            if (ID == minimumRouteTSP[pagina])
            {
                cout << setw(5) << " "
                     << "+-----------------------+\n"
                     << setw(8) << " [" << date_oras->getCityID() << "] " << date_oras->getCityName() << "\n"
                     << setw(5) << " "
                     << "+-----------------------+";
                break;
            }
        }

#ifdef _WIN32
        changeText(FOREGROUND_INTENSITY);
        cout << setw(100) << " "
             << "Distanta parcursa: " << distanta_parcursa;
        resetText();
        cout << "km | ";
        changeText(FOREGROUND_INTENSITY);
        cout << "Durata de calatorie: " << durata_parcursa;
        resetText();
        cout << "min\n";

#elif __linux__
        cout << setw(100) << " "
             << "\033[1m"
             << "Distanta parcursa: "
             << "\033[0m" << distanta_parcursa << "km | "
             << "\033[1m"
             << "Durata de calatorie: "
             << "\033[0m" << durata_parcursa << "min\n";
#endif

        cout << "\n\n"
             << setw(5) << " "
             << "ID" << setw(10) << " "
             << "Den.Produs" << setw(15) << " "
             << "Cnt.transport/Cnt.ramasa\n";
        underline(70, true);

        if (centralDepos[minimumRouteTSP[pagina]] == false)
        {
            if (limitedStockCities[minimumRouteTSP[pagina]])
            {
                for (PRODUCT::PRODUCT_NODE *date_produs = product.getHead(); date_produs != nullptr; date_produs = date_produs->next)
                {
                    int ID_PRODUS = stoi(date_produs->getProductID());
                    double cantitate_necesara = 0.0;

                    for (DEPOT::DEPOT_NODE *date_depozit = depot.getHead(); date_depozit != nullptr; date_depozit = date_depozit->next)
                    {
                        int ID_PRODUS_DEPOZIT = stoi(date_depozit->getProductID()), ID_DEPOZIT = stoi(date_depozit->getCityID());
                        if (ID_DEPOZIT == minimumRouteTSP[pagina] && !isolatedVertex[ID_DEPOZIT])
                        {
                            if (ID_PRODUS_DEPOZIT == ID_PRODUS)
                            {
                                cantitate_necesara += date_depozit->getProductQuantity();
                                cantitate_necesara = VAL_STOC_MAXIM - cantitate_necesara;
                                cost_aprovizionare_total += cantitate_necesara * date_produs->getProductPrice();
                                cantitate_totala_aprovizionata += cantitate_necesara;
                                date_depozit->updateQuantity(VAL_STOC_MAXIM);

                                cout << setw(5 + 1) << " [" << date_depozit->getProductID() << "] " << setw(8) << " " << date_produs->getProductName()
                                     << setw(cmax_denumire_produse - strlen(date_produs->getProductName()) + 5) << " " << cantitate_necesara << " buc. /";

                                for (SUPPLY::SUPPLY_NODE *supply_data = supply.getHead(); supply_data != nullptr; supply_data = supply_data->next)
                                {
                                    int ID_PRODUS_APROVIZIONARE = stoi(supply_data->getID());
                                    if (ID_PRODUS_APROVIZIONARE == ID_PRODUS)
                                    {
                                        cout << supply_data->getQuantity() << " buc.";
                                        int currentQuantity = supply_data->getQuantity();
                                        supply_data->updateQuantity(currentQuantity - cantitate_necesara);
                                        break;
                                    }
                                }
                                cout << "\n";
                            }
                        }
                    }
                }
            }
            else
            {
#ifdef _WIN32
                changeText(FOREGROUND_INTENSITY);

                cout << setw(5) << " "
                     << "Depozitul a fost aprovizionat!" << endl;

                resetText();
#elif __linux__
                cout << setw(5) << " "
                     << "\033[1m"
                     << "Depozitul a fost aprovizionat!\n"
                     << "\033[0m";
#endif
            }
        }
        else
        {
#ifdef _WIN32
            changeText(FOREGROUND_INTENSITY);

            cout << setw(5) << " "
                 << "Depozit centralizat!!" << endl;

            resetText();
#elif __linux__
            cout << setw(5) << " "
                 << "\033[1m"
                 << "Depozit centralizat!\n"
                 << "\033[0m";
#endif
        }

        underline(70, true);

        cout << "\n\n";
        underline(190, false);

        cout << setw(5) << " "
             << "[1] PREV" << setw(80) << " -" << pagina << "- " << setw(80) << " "
             << "[2] NEXT\n";

        underline(190, false);

        limitedStockCities[minimumRouteTSP[pagina]] = false;

        if (pagina - 1 == 1)
        {
            distanta_parcursa += adjacencyMatrix[minimumRouteTSP[pagina]][minimumRouteTSP[pagina + 1]].distance;
            durata_parcursa += adjacencyMatrix[minimumRouteTSP[pagina]][minimumRouteTSP[pagina + 1]].duration;
        }
    }
    else
        pagina_finala_TSP();
}

void parcurgere_traseu_TSP(void)
{
    pagina = 1;
    cout << "\n";

    if (!traseu_completat)
    {
        if (minimumRouteTSP[1] == -1)
            TSP();
        else
        {
            clear_screen();
#ifdef _WIN32
            changeText(FOREGROUND_INTENSITY);

            cout << setw(5) << " "
                 << "Lungime traseu: " << cost_minim_TSP << "km | "
                 << " Durata traseu: " << durata_minima_TSP << endl
                 << setw(5) << " ";

            resetText();
#elif __linux__
            cout << setw(5) << " "
                 << "\033[1m"
                 << "Lungime traseu: " << cost_minim_TSP << "km | "
                 << "Durata traseu: " << durata_minima_TSP << "\n"
                 << setw(5) << " "
                 << "\033[0m";
#endif
            for (unsigned int i = 1; i <= contor_traseu_TSP; i++)
            {
                for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
                {
                    int ID = stoi(date_oras->getCityID());
                    if (ID == minimumRouteTSP[i])
                    {
                        cout << date_oras->getCityName();
                        if (i < contor_traseu_TSP)
                            cout << " --> ";
                        break;
                    }
                }
            }

            cout << "\n";
            underline(190, false);
        }
    }
    else
    {
        clear_screen();

#ifdef _WIN32
        changeText(FOREGROUND_INTENSITY);

        cout << "\n\n"
             << setw(5) << " "
             << "Nu exista depozite cu stoc limitat/epuizat...";

        resetText();
#elif __linux__
        cout << "\n\n"
             << setw(5) << " "
             << "\033[1m"
             << "Nu exista depozite cu stoc limitat/epuizat..."
             << "\033[0m";
#endif
        minimumRouteTSP.clear();
        _getch();
        return;
    }

    char *input = (char *)malloc(MAXL * sizeof(char) + 1);

    cout << setw(5) << " "
         << "Scrie '0' pentru a te intoarce...\n\n";

    cout << setw(5) << " "
         << "[S] Start: ";
    cin >> input;

    if (_strcasecmp_(input, "0") == 0)
    {
        free(input);
        return;
    }
    else if (_strcasecmp_(input, "s") == 0)
    {
        unsigned int MENIU;

        pagina_principala_TSP();

        do
        {
            cout << "\n"
                 << setw(5) << " "
                 << "Introduceti numarul meniului: ";
            cin >> MENIU;

            switch (MENIU)
            {
            case 1:
                pagina_stanga_TSP();
                break;
            case 2:
                pagina_dreapta_TSP();
                break;

            default:
                break;
            }
        } while (MENIU != 0 && traseu_completat == false);
    }
    free(input);
}
#pragma endregion

void afisare_detalii_SpeedyGo(void)
{
    cout << "\n";
    underline(100, true);

    cout << "\033[3m"
         << setw(5) << " "
         << "Bun venit in Consola MySQL. Introduceti o interogare SQL (sau 'exit' pentru a incheia)\n"
         << "\033[0m";

    underline(100, true);

    sql::Statement *stmt = con->createStatement();
    sql::ResultSet *res = stmt->executeQuery("SHOW TABLES");

    cout << setw(5) << " "
         << "+" << string(22, '-') << "+" << endl;
    cout << setw(5) << " "
         << "| " << std::left << setw(20) << "SpeedyGo - Tabele"
         << " |" << endl;
    cout << setw(5) << " "
         << "|" << string(22, '-') << "|" << endl;

    while (res->next())
    {
        cout << setw(5) << " "
             << "| " << setw(20) << std::left << res->getString(1) << " |" << endl;
    }

    cout << std::right;

    cout << setw(5) << " "
         << "+" << string(22, '-') << "+" << endl;

    underline(100, true);

    delete stmt;
    delete res;
}

void consola_mysql(void)
{
    clear_screen();

    if (buffer)
    {
        buffer = false;
        cin.ignore(9999, '\n');
    }

    afisare_detalii_SpeedyGo();

    string interogare;

    while (true)
    {
        cout << "\n"
             << setw(5) << " "
             << "mysql> ";

        getline(cin, interogare);

        if (interogare == "exit")
        {
            buffer = true;
            break;
        }
        else if (interogare == "clear")
        {
            clear_screen();
            afisare_detalii_SpeedyGo();
        }
        else
        {
            try
            {
                sql::Statement *stmt = con->createStatement();
                sql::ResultSet *res = stmt->executeQuery(interogare);

                int cnt_coloane = res->getMetaData()->getColumnCount();
                vector<int> coloane(cnt_coloane, 0);

                while (res->next())
                {
                    for (unsigned int i = 1; i <= cnt_coloane; i++)
                    {
                        int crt_width = res->getString(i).length();
                        if (crt_width > coloane[i - 1])
                        {
                            coloane[i - 1] = crt_width;
                        }
                    }
                }

                cout << "\n";
                for (unsigned int i = 1; i <= cnt_coloane; i++)
                    cout << setw(5) << " " << setw(coloane[i - 1] + 5) << res->getMetaData()->getColumnName(i) << " ";
                cout << "\n";

                underline(100, true);

                res->beforeFirst();

                while (res->next())
                {
                    for (unsigned int i = 1; i <= cnt_coloane; i++)
                        cout << setw(5) << " " << setw(coloane[i - 1] + 5) << res->getString(i) << " ";
                    cout << "\n";
                }

                underline(100, true);

                city.~CITY();
                city.head_city = nullptr;
                city.tail_city = nullptr;

                product.~PRODUCT();
                product.head_product = nullptr;
                product.tail_product = nullptr;

                depot.~DEPOT();
                depot.head_depot = nullptr;
                depot.tail_depot = nullptr;

                if (fetchTables() == EXIT_FAILURE)
                    _getch();

                delete stmt;
                delete res;
            }
            catch (sql::SQLException &e)
            {
                cout << setw(5) << " "
                     << "Error code: " << e.getErrorCode() << "\n";
                cout << setw(5) << " "
                     << "Error message: " << e.what() << "\n";
                cout << setw(5) << " "
                     << "SQLState: " << e.getSQLState() << "\n";
            }
        }
    }

    interogare.clear();
}

void sortare_tip_depozit(void)
{
    clear_screen();

    afisare_date_tabel_oras();

    char *input = (char *)malloc((MAXL + 1) * sizeof(char));

#ifdef _WIN32
    changeText(FOREGROUND_INTENSITY);
    cout << setw(5) << " "
         << "Scrieti '0' pentru a iesi\n\n";
    resetText();
    cout << setw(5) << " "
         << "Introduceti tipul: ";
#elif __linux__
    cout << setw(5) << " "
         << "\033[3m"
         << "Scrieti '0' pentru a iesi\n\n"
         << "\033[0m" << setw(5) << " "
         << "Introduceti tipul: ";
#endif

    cin >> input;

    if (_strcasecmp_(input, "0") == 0)
    {
        free(input);
        return;
    }
    else
    {
        clear_screen();

        cout << "\n\n"
             << setw(5) << " "
             << "+---------------+\n"
             << setw(5) << " "
             << "| TABEL-DEPOT |\n"
             << setw(5) << " "
             << "+---------------+\n\n";

        cout << setw(5) << " "
             << "ID_Oras"
             << setw(5) << " "
             << "Denumire_Oras"
             << setw(5) << " "
             << "Tip_Depozit"
             << setw(5) << " "
             << "Latitudine"
             << setw(5) << " "
             << "Longitudine\n";
        underline(80, true);

        for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
            if (_strcasecmp_(date_oras->getCityType(), input) == 0)
            {
                cout << setw(5 + 1) << " [" << date_oras->getCityID() << "]" << setw(cmax_ID_Oras - strlen(date_oras->getCityID()) + 8)
                     << " " << date_oras->getCityName() << setw(cmax_denumire_orase - strlen(date_oras->getCityName()) + 4)
                     << " " << date_oras->getCityType() << setw(11 - strlen(date_oras->getCityType()) + 5)
                     << " " << fixed << setprecision(2) << date_oras->getLatitude();
#ifdef _WIN32
                cout << "\370";
#elif __linux__
                cout << "\u00B0";
#endif
                cout << setw(cmax_lat_oras - to_string(round(date_oras->getLatitude())).length() + 13)
                     << " " << date_oras->getLongitude();
#ifdef _WIN32
                cout << "\370" << endl;
#elif __linux__
                cout << "\u00B0\n";
#endif
            }

        underline(80, true);

        cout << "\n\n"
             << setw(5) << " "
             << "Apasa 'ENTER' pentru a te intoarce...";

        free(input);
        _getch();
        sortare_tip_depozit();
    }
}

void sortare_depozit_alfabetic(const int tip_sortare)
{
    clear_screen();

    cout << "\n\n"
         << setw(5) << " "
         << "+---------------+\n"
         << setw(5) << " "
         << "| TABEL-DEPOT |\n"
         << setw(5) << " "
         << "+---------------+\n\n";

    cout << setw(5) << " "
         << "ID_Oras"
         << setw(5) << " "
         << "Denumire_Oras"
         << setw(5) << " "
         << "Tip_Depozit"
         << setw(5) << " "
         << "Latitudine"
         << setw(5) << " "
         << "Longitudine\n";
    underline(80, true);

    bool sort = true;

    CITY::CITY_NODE *ptr;
    CITY::CITY_NODE *l_ptr = nullptr;

    if (city.getHead() == nullptr)
        return;

    do
    {
        sort = true;
        ptr = city.getHead();

        while (ptr->next != l_ptr)
        {
            if (tip_sortare == 1)
            {
                if (strcmp(ptr->getCityName(), ptr->next->getCityName()) > 0)
                {
                    ptr->CITY::CITY_NODE::swapData(*(ptr->next));
                    sort = false;
                }
            }
            else if (tip_sortare == 2)
            {
                if (strcmp(ptr->getCityName(), ptr->next->getCityName()) < 0)
                {
                    ptr->CITY::CITY_NODE::swapData(*(ptr->next));
                    sort = false;
                }
            }

            ptr = ptr->next;
        }
        l_ptr = ptr;
    } while (!sort);
}

void cautare_oras_ID(void)
{
    clear_screen();

    afisare_date_tabel_oras();

    char *I_ID = (char *)malloc(MAXL * sizeof(char) + 1);
    bool gasit = false;

#ifdef _WIN32
    changeText(FOREGROUND_INTENSITY);
    cout << setw(5) << " "
         << "Scrieti 'exit' pentru a iesi\n\n";
    resetText();
    cout << setw(5) << " "
         << "Introduceti ID-ul: ";
#elif __linux__
    cout << setw(5) << " "
         << "\033[3m"
         << "Scrieti 'exit' pentru a iesi\n\n"
         << "\033[0m" << setw(5) << " "
         << "Introduceti ID-ul: ";
#endif

    cin >> I_ID;

    if (_strcasecmp_(I_ID, "exit") == 0)
    {
        free(I_ID);
        return;
    }

    clear_screen();

    cout << "\n\n"
         << setw(5) << " "
         << "+---------------+\n"
         << setw(5) << " "
         << "| TABEL-DEPOT |\n"
         << setw(5) << " "
         << "+---------------+\n\n";

    cout << setw(5) << " "
         << "ID_Oras"
         << setw(5) << " "
         << "Denumire_Oras"
         << setw(5) << " "
         << "Tip_Depozit"
         << setw(5) << " "
         << "Latitudine"
         << setw(5) << " "
         << "Longitudine\n";
    underline(80, true);

    for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
    {
        if (_strcasecmp_(date_oras->getCityID(), I_ID) == 0)
        {
            gasit = true;

            cout << setw(5 + 1) << " [" << date_oras->getCityID() << "]" << setw(cmax_ID_Oras - strlen(date_oras->getCityID()) + 8)
                 << " " << date_oras->getCityName() << setw(cmax_denumire_orase - strlen(date_oras->getCityName()) + 4)
                 << " " << date_oras->getCityType() << setw(11 - strlen(date_oras->getCityType()) + 5)
                 << " " << fixed << setprecision(2) << date_oras->getLatitude();
#ifdef _WIN32
            cout << "\370";
#elif __linux__
            cout << "\u00B0";
#endif
            cout << setw(cmax_lat_oras - to_string(round(date_oras->getLatitude())).length() + 13)
                 << " " << date_oras->getLongitude();
#ifdef _WIN32
            cout << "\370" << endl;
#elif __linux__
            cout << "\u00B0\n";
#endif

            break;
        }
    }

    underline(80, true);

    if (!gasit)
    {
        clear_screen();

        cout << "\n"
             << setw(5) << " "
             << "ID-ul introdus nu este valid...";

        free(I_ID);
        _getch();
        cautare_oras_ID();
    }
    else
    {
        cout << "\n"
             << setw(5) << " "
             << "Apasa 'ENTER' pentru a te intoarce...";

        _getch();
        free(I_ID);
        cautare_oras_ID();
    }
}

void cautare_depozit_denumire(void)
{
    clear_screen();

    afisare_date_tabel_oras();

    char *I_Denumire = (char *)malloc(MAXL * sizeof(char) + 1);
    bool gasit = false;

#ifdef _WIN32
    changeText(FOREGROUND_INTENSITY);
    cout << setw(5) << " "
         << "Scrieti '0' pentru a iesi\n\n";
    resetText();
    cout << setw(5) << " "
         << "Introduceti tipul: ";
#elif __linux__
    cout << setw(5) << " "
         << "\033[3m"
         << "Scrieti '0' pentru a iesi\n\n"
         << "\033[0m" << setw(5) << " "
         << "Introduceti tipul: ";
#endif

    cin.get();
    cin.get(I_Denumire, MAXL);

    if (_strcasecmp_(I_Denumire, "0") == 0)
    {
        free(I_Denumire);
        return;
    }

    clear_screen();

    cout << "\n\n"
         << setw(5) << " "
         << "+---------------+\n"
         << setw(5) << " "
         << "| TABEL-DEPOT |\n"
         << setw(5) << " "
         << "+---------------+\n\n";

    cout << setw(5) << " "
         << "ID_Oras"
         << setw(5) << " "
         << "Denumire_Oras"
         << setw(5) << " "
         << "Tip_Depozit"
         << setw(5) << " "
         << "Latitudine"
         << setw(5) << " "
         << "Longitudine\n";
    underline(80, true);

    for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
    {
        if (_strcasecmp_(date_oras->getCityName(), I_Denumire) == 0)
        {
            gasit = true;

            cout << setw(5 + 1) << " [" << date_oras->getCityID() << "]" << setw(cmax_ID_Oras - strlen(date_oras->getCityID()) + 8)
                 << " " << date_oras->getCityName() << setw(cmax_denumire_orase - strlen(date_oras->getCityName()) + 4)
                 << " " << date_oras->getCityType() << setw(11 - strlen(date_oras->getCityType()) + 5)
                 << " " << fixed << setprecision(2) << date_oras->getLatitude();
#ifdef _WIN32
            cout << "\370";
#elif __linux__
            cout << "\u00B0";
#endif
            cout << setw(cmax_lat_oras - to_string(round(date_oras->getLatitude())).length() + 13)
                 << " " << date_oras->getLongitude();
#ifdef _WIN32
            cout << "\370" << endl;
#elif __linux__
            cout << "\u00B0\n";
#endif
            break;
        }
    }

    underline(80, true);

    if (!gasit)
    {
        clear_screen();

        cout << "\n\n"
             << setw(5) << " "
             << "Numele introdus nu este valid...";

        free(I_Denumire);
        _getch();
        cautare_depozit_denumire();
    }
    else
    {
        cout << "\n"
             << setw(5) << " "
             << "Apasa 'ENTER' pentru a te intoarce...";

        _getch();
        free(I_Denumire);
        cautare_depozit_denumire();
    }
}