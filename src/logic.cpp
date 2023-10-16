#include "../include/logic.hpp"
#include "../include/declarations.hpp"
#include "../include/database.hpp"
#include "../include/GoogleMatrixAPI.hpp"
#include "../include/haversine.hpp"

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>

bool validateMysqlCredentials(void)
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
              << std::setw(10) << " "
              << "CONEXIUNE LA BAZA DE DATE\n"
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

bool _ADJACENCY_MATRIX_INIT_(void)
{
    if (_GOOGLE_MATRIX_API_INIT_() == EXIT_FAILURE)
    {
        std::cout << std::setw(5) << " "
                  << "-- Initialization of the Google API service could not be completed!\n";
        ERROR_CNT++;

        if (_HAVERSINE_INIT_("utils/legaturi.txt") == EXIT_FAILURE)
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
        double distanta = date_gps->getDistance();

        strcpy(city1, date_gps->getStart());
        strcpy(city2, date_gps->getDestination());

        for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
            if (_strcasecmp_(date_oras->getCityName(), city1) == 0)
            {
                city1_ID = std::stoi(date_oras->getCityID());
                break;
            }

        for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
            if (_strcasecmp_(date_oras->getCityName(), city2) == 0)
            {
                city2_ID = std::stoi(date_oras->getCityID());
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
        if (strlen(date_oras->getCityName()) > maxCityNameLength)
            maxCityNameLength = strlen(date_oras->getCityName());

        char temp[MAXL];
        sprintf(temp, "%f", date_oras->getLatitude());

        for (unsigned int i = 0; i < strlen(temp); i++)
            if (temp[i] == '.')
                temp[i] = '\0';

        if (strlen(temp) > maxCityLatitudeLength)
            maxCityLatitudeLength = strlen(temp);
    }

    for (PRODUCT::PRODUCT_NODE *date_produs = product.getHead(); date_produs != nullptr; date_produs = date_produs->next)
    {
        if (strlen(date_produs->getProductName()) > maxProductNameLength)
            maxProductNameLength = strlen(date_produs->getProductName());

        if (strlen(date_produs->getProductCategory()) > maxProductCategoryLength)
            maxProductCategoryLength = strlen(date_produs->getProductCategory());

        char *pret = (char *)malloc(MAXL * sizeof(char) + 1);
        sprintf(pret, "%f", date_produs->getProductPrice());

        for (unsigned int i = 0; i < strlen(pret); i++)
            if (pret[i] == '.')
                pret[i] = '\0';

        if (strlen(pret) > maxProductPriceLength)
            maxProductPriceLength = strlen(pret);

        if (strlen(date_produs->getProductID()) > maxProductIDLength)
            maxProductIDLength = strlen(date_produs->getProductID());

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

    std::cout << "\n\n"
              << std::setw(5) << " "
              << "ID_Produs" << std::setw(4) << " "
              << "Denumire_Produs" << std::setw(8) << " "
              << "Nr.Buc\n";
    underline(55, true);

    for (PRODUCT::PRODUCT_NODE *date_produs = product.getHead(); date_produs != nullptr; date_produs = date_produs->next)
    {
        int ID_PRODUS = std::stoi(date_produs->getProductID());
        for (DEPOT::DEPOT_NODE *date_depozit = depot.getHead(); date_depozit != nullptr; date_depozit = date_depozit->next)
        {
            int ID_PRODUS_DEPOZIT = std::stoi(date_depozit->getProductID()), ID_DEPOZIT = std::stoi(date_depozit->getCityID());
            if (ID_PRODUS_DEPOZIT == ID_PRODUS && ID_DEPOZIT == ID_Depozit)
            {
                if (date_depozit->getProductQuantity() < MINIMUM_STOCK_VALUE)
                    std::cout << std::setw(5 + 1) << " [" << date_produs->getProductID() << "]" << std::setw(maxProductIDLength - strlen(date_produs->getProductID()) + 9)
                              << " " << date_produs->getProductName()
                              << std::setw(maxProductNameLength - strlen(date_produs->getProductName()) + 3) << " " << date_depozit->getProductQuantity()
                              << std::setw(maxProductQuantityLength - std::to_string(round(date_produs->getProductPrice())).length() + 5) << " "
                              << "buc.\n";
            }
        }
    }

    underline(55, true);

    std::cout << std::setw(5) << " "
              << "Apasa 'ENTER' pentru a te intoarce...";
}

void depozite_conectate(int ID_Depozit)
{
    std::cout << "\n";

    std::vector<bool> temp_depozite(VERTEX_COUNT, false);
    CITY::CITY_NODE *date_oras = city.getHead();
    char *t_denumire = (char *)malloc(MAXL * sizeof(char) + 1);

    while (date_oras != nullptr)
    {
        int _ID = std::stoi(date_oras->getCityID());
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
                int _ID = std::stoi(date_oras->getCityID());
                if (_ID == i && _strcasecmp_(t_denumire, date_oras->getCityName()) != 0)
                {
                    std::cout << std::setw(5) << " " << t_denumire << " -> " << date_oras->getCityName()
                              << std::setw(maxCityNameLength - strlen(date_oras->getCityName()) + 5)
                              << " (" << adjacencyMatrix[ID_Depozit][i].distance << "km | " << adjacencyMatrix[ID_Depozit][i].duration << "min)\n";
                    break;
                }
                date_oras = date_oras->next;
            }
        }

    std::cout << "\n"
              << std::setw(5) << " "
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
            int ID = std::stoi(date_oras->getCityID());
            centralDepos[ID] = true;
        }
        date_oras = date_oras->next;
    }
}

void cautare_orase_stoc_limitat(void)
{
    limited_stock_cities_count = 0;
    CITY::CITY_NODE *date_oras = city.getHead();
    while (date_oras != nullptr)
    {
        int _ID_Oras = std::stoi(date_oras->getCityID());

        DEPOT::DEPOT_NODE *date_depozit = depot.getHead();
        while (date_depozit != nullptr)
        {
            int _ID_Depozit = std::stoi(date_depozit->getCityID());
            if (_ID_Depozit == _ID_Oras && centralDepos[_ID_Depozit] == false)
                if (date_depozit->getProductQuantity() < MINIMUM_STOCK_VALUE)
                {
                    limitedStockCities[_ID_Depozit] = true;
                    limited_stock_cities_count++;
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

    if (limited_stock_cities_count == 0)
    {
        std::cout << "\n"
                  << std::setw(5) << " "
                  << "Nu sunt depozite care necesita aprovizionare...";
        _getch();
        return;
    }

    clear_screen();

    std::cout << "\n\n"
              << std::setw(5) << " "
              << "+-----------------------+\n"
              << std::setw(6) << " "
              << "Orase cu stocuri reduse\n"
              << std::setw(5) << " "
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
                int _ID = std::stoi(date_oras->getCityID());
                if (_ID == i)
                    std::cout << std::setw(5 + 1) << " [" << date_oras->getCityID() << "] " << date_oras->getCityName()
                              << std::setw(maxCityNameLength - strlen(date_oras->getCityName()) + 5) << " ";
                date_oras = date_oras->next;
            }
            std::cout << "\n";
        }

    underline(40, true);

#ifdef _WIN32
    changeText(FOREGROUND_INTENSITY);
    std::cout << std::setw(5) << " "
              << "Scrieti 'exit' pentru a iesi\n\n";
    resetText();
#elif __linux__
    std::cout << std::setw(5) << " "
              << "\033[3m"
              << "Scrieti 'exit' pentru a iesi\n\n"
              << "\033[0m";
#endif

    char *t_ID_Oras = (char *)malloc(MAXL * sizeof(char) + 1);
    std::cout << std::setw(5) << " "
              << "Introduceti ID-ul orasului: ";
    std::cin >> t_ID_Oras;

    if (_strcasecmp_(t_ID_Oras, "exit") == 0)
    {
        free(t_ID_Oras);
        return;
    }
    else
    {
        int _ID_Oras = std::stoi(t_ID_Oras);
        free(t_ID_Oras);
        date_oras = city.getHead();

        while (date_oras != nullptr)
        {
            int t_ID = std::stoi(date_oras->getCityID());
            if (t_ID == _ID_Oras && limitedStockCities[t_ID] == true)
            {
                unsigned int sMENIU;

                do
                {
                    clear_screen();

                    std::cout << "\n\n"
                              << std::setw(5) << " "
                              << "+--------------------------------------------+\n"
                              << std::setw(7) << " " << date_oras->getCityName() << " | Tip depot: " << date_oras->getCityName() << "\n"
                              << std::setw(5) << " "
                              << "+--------------------------------------------+\n";

                    underline(50, true);

                    std::cout << std::setw(5) << " "
                              << "[1] Vizualizare produse cu stoc limitat\n"
                              << std::setw(5) << " "
                              << "[2] Vizualizare conexiuni cu alte depozite\n"
                              << std::setw(5) << " "
                              << "[0] Inapoi\n";

                    underline(50, true);

                    std::cout << std::setw(5) << " "
                              << "Introduceti numarul meniului: ";
                    std::cin >> sMENIU;

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

void creare_solutie_distanta(int start, std::vector<double> &distanta, std::vector<int> &distanta_minima, bool afisare, bool creare_trasee)
{
    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
    {
        if (i != start)
        {
            if (afisare)
                std::cout << "Cea mai scurta distanta de la " << start << " la " << i << " este: " << distanta[i] << " : traseu: ";

            std::vector<int> traseu;

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
                    std::cout << traseu[j] << " ";

            if (afisare)
                std::cout << "\n";
        }
    }
}

void dijkstra(int start, std::vector<double> &distanta, std::vector<int> &distanta_minima)
{
    std::vector<bool> visited(VERTEX_COUNT, false);
    distanta[start] = 0.0;

    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
    {
        int min_index = 0;
        double min_dist = std::numeric_limits<double>::infinity();

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
    std::cout << "\n"
              << std::setw(5) << " "
              << "Depozite centralizate\n";
    underline(40, true);

    for (unsigned int i = 0; i < VERTEX_COUNT; i++)
    {
        if (centralDepos[i])
            for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
            {
                int ID = std::stoi(date_oras->getCityID());
                if (ID == i)
                {
                    std::cout << std::setw(5 + 1) << " [" << ID << "] " << date_oras->getCityName() << "\n";
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
        std::cout << "\n"
                  << std::setw(5) << " "
                  << "Nu exista traseul cu acest ID. Depozit izolat...";
        return;
    }

    for (ROUTE::ROUTE_NODE *date_traseu = route.getHead(); date_traseu != nullptr; date_traseu = date_traseu->next)
    {
        if (date_traseu->getDestination() == _ID && date_traseu->getStart() == vStart)
        {
            gasit = true;
            std::cout << "\n"
                      << std::setw(5) << " "
                      << "Distanta: ";
            std::cout << date_traseu->getDistance() << "km\n"
                      << std::setw(5) << " ";

            const std::vector<int> &traseu = date_traseu->getRoute();

            for (unsigned int i = 0; i < traseu.size(); i++)
                for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
                {
                    int ID = std::stoi(date_oras->getCityID());

                    if (ID == traseu[i])
                    {
                        std::cout << date_oras->getCityName();
                        // corectare afisare '->'
                        if (i != traseu.size() - 1)
                            std::cout << " --> ";
                        break;
                    }
                }
            break;
        }
    }

    if (!gasit)
    {
        std::cout << "\n"
                  << std::setw(5) << " "
                  << "Nu exista traseul cu acest ID!";
        return;
    }
}

void afisare_optiuni_trasee_optime(const int vStart)
{
    clear_screen();

    std::cout << "\n\n"
              << std::setw(5) << " "
              << "SCRIE 'exit' PENTRU A TE INTOARCE...\n";

    underline(45, true);

    char *oras_start = (char *)malloc(MAXL * sizeof(char) + 1);

    for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
    {
        int ID = std::stoi(date_oras->getCityID());
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
                int ID = std::stoi(date_oras->getCityID());
                if (ID == date_traseu->getDestination())
                {
                    std::cout << std::setw(5 + 1) << " [" << date_oras->getCityID() << "] ";
                    std::cout << oras_start << " -> " << date_oras->getCityName() << "\n";
                    break;
                }
            }
        }
    }

    underline(45, true);

    char *_ID = (char *)malloc(MAXL * sizeof(char) + 1);
    std::cout << std::setw(5) << " "
              << "Introduceti ID-ul corespunzator: ";
    std::cin >> _ID;
    if (_strcasecmp_(_ID, "exit") == 0)
    {
        free(_ID);
        free(oras_start);
        return;
    }
    else
    {
        free(oras_start);

        int _ID_temp = std::stoi(_ID);

        free(_ID);

        afisare_trasee_optime(_ID_temp, vStart);
        _getch();
        afisare_optiuni_trasee_optime(vStart);
    }
}

void sistem_aprovizionare_independent(void)
{
    if (!dijkstraRoutesCalculated)
    {
        for (unsigned int i = 0; i < VERTEX_COUNT; i++)
        {
            std::vector<int> distanta_minima(VERTEX_COUNT, -1);
            std::vector<double> distanta(VERTEX_COUNT, std::numeric_limits<double>::infinity());

            if (centralDepos[i])
            {
                dijkstra(i, distanta, distanta_minima);
                if (!dijkstraRoutesCalculated)
                    creare_solutie_distanta(i, distanta, distanta_minima, false, true);
                else
                {
                    dijkstraRoutesCalculated = true;
                    creare_solutie_distanta(i, distanta, distanta_minima, false, false);
                }
            }
            distanta_minima.clear();
            distanta.clear();
        }
        dijkstraRoutesCalculated = true;
    }

    clear_screen();

    std::cout << "\n\n"
              << std::setw(5) << " "
              << "SCRIE 'exit' PENTRU A TE INTOARCE...\n";

    afisare_depozite_centralizare();

    char *_ID = (char *)malloc(MAXL * sizeof(char) + 1);
    std::cout << std::setw(5) << " "
              << "Introduceti ID-ul corespunzator: ";
    std::cin >> _ID;
    if (_strcasecmp_(_ID, "exit") == 0)
    {
        free(_ID);
        return;
    }
    else
    {
        int _ID_temp = std::stoi(_ID);

        if (centralDepos[_ID_temp])
        {
            free(_ID);
            afisare_optiuni_trasee_optime(_ID_temp);
            sistem_aprovizionare_independent();
        }
        else
        {
            std::cout << "\n"
                      << std::setw(5) << " "
                      << "Nu exista depozitul central cu acest ID...";
            free(_ID);
            _getch();
            sistem_aprovizionare_independent();
        }
    }
}

void afisare_depozite_izolate(void)
{
    clear_screen();

    std::cout << "\n\n"
              << std::setw(5) << " "
              << "+------------------+\n"
              << std::setw(5) << " "
              << "| DEPOZITE IZOLATE |\n"
              << std::setw(5) << " "
              << "+------------------+\n\n";

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
                int ID = std::stoi(date_oras->getCityID());
                if (ID == i)
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
                    break;
                }
            }
        }

    underline(75, true);

    if (!gasit)
    {
        std::cout << std::setw(5) << " "
                  << "Nu exista depozite izolate...";
        return;
    }
    else
        std::cout << "\n"
                  << std::setw(5) << " "
                  << " Apasati 'ENTER' pentru a iesi...";
}

void afisare_depozite_unic_drum(void)
{
    clear_screen();

    std::cout << "\n\n";
    std::cout << std::setw(5) << " "
              << "+-------------------------------+\n";
    std::cout << std::setw(5) << " "
              << "| DEPOZITE CU O UNICA CONEXIUNE |\n";
    std::cout << std::setw(5) << " "
              << "+-------------------------------+\n\n";

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
                    int ID = std::stoi(date_oras->getCityID());
                    if (ID == i)
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
                        break;
                    }
                }
            }
        }
        underline(75, true);

        std::cout << "\n"
                  << std::setw(5) << " "
                  << " Apasati 'ENTER' pentru a iesi...";
    }
    else
    {
        std::cout << std::setw(5) << " "
                  << "Nu exista depozite cu o unica conexiune...";
        return;
    }
}

void hamiltonianGraph::init_stiva(void)
{
    stack[stackCounter] = -1;
}

bool hamiltonianGraph::succesor(void)
{
    if (stack[stackCounter] < VERTEX_COUNT - 1)
    {
        stack[stackCounter]++;
        return true;
    }
    return false;
}

bool hamiltonianGraph::solutie(void)
{
    if (stackCounter == limited_stock_cities_count)
        return true;
    return false;
}

bool hamiltonianGraph::valid(void)
{
    for (unsigned int i = 1; i < stackCounter; i++)
        if (stack[stackCounter] == stack[i])
            return false;

    if (stackCounter > 1)
        if (adjacencyMatrix[stack[stackCounter]][stack[stackCounter - 1]].distance == 0)
            return false;

    if (stackCounter > 1)
        if (centralDepos[stack[1]] == false)
            return false;

    return true;
}

void hamiltonianGraph::determinare_ciclu_hc_minim(void)
{
    double suma_dist = 0.0;
    int suma_durata = 0;

    for (int i = 1; i <= stackCounter; i++)
    {
        suma_dist += adjacencyMatrix[stack[i]][stack[i + 1]].distance;
        suma_durata += adjacencyMatrix[stack[i]][stack[i + 1]].duration;
    }

    if (suma_dist < minimumDistanceCostTSP || (suma_dist == minimumDistanceCostTSP && suma_durata < minimumDurationCostTSP))
    {
        minimumDistanceCostTSP = suma_dist;
        minimumDurationCostTSP = suma_durata;

        for (int i = 1; i <= stackCounter; i++)
        {
            minimumRouteTSP[i] = stack[i];
            TSP_RouteCounter = stackCounter;
        }
    }
}

void hamiltonianGraph::back(void)
{
    stackCounter = 1;
    hamiltonianGraph::init_stiva();
    while (stackCounter > 0)
    {
        int vSuccesor, vValid;
        do
        {
            vSuccesor = hamiltonianGraph::succesor();
            if (vSuccesor == 1)
                vValid = hamiltonianGraph::valid();
        } while (vSuccesor == 1 && vValid == 0);
        if (vSuccesor == 1)
        {
            if (hamiltonianGraph::solutie() == 1)
                hamiltonianGraph::determinare_ciclu_hc_minim();
            else
            {
                stackCounter++;
                hamiltonianGraph::init_stiva();
            }
        }
        else
            stackCounter--;
    }
}

void acyclicGraph::init_stiva(void)
{
    stack[stackCounter] = -1;
}

bool acyclicGraph::succesor(void)
{
    if (stack[stackCounter] < VERTEX_COUNT - 1)
    {
        stack[stackCounter]++;
        return true;
    }
    return false;
}

bool acyclicGraph::solutie(void)
{
    if (stackCounter == VERTEX_COUNT + 1)
        return true;
    return false;
}

bool acyclicGraph::valid(void)
{
    if (stackCounter == VERTEX_COUNT + 1)
    {
        for (unsigned int i = 0; i < VERTEX_COUNT; i++)
        {
            bool gasit = false;
            if (limitedStockCities[i] == true && !isolatedVertex[i])
            {
                for (unsigned int j = 1; j <= stackCounter; j++)
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

    if (stackCounter > 1)
        if (adjacencyMatrix[stack[stackCounter]][stack[stackCounter - 1]].distance == 0)
            return false;

    if (stackCounter > 1)
        if (centralDepos[stack[1]] == false)
            return false;

    return true;
}

void acyclicGraph::determinare_traseu_minim(void)
{
    double suma_dist = 0.0;
    int suma_durata = 0;

    for (int i = 1; i < stackCounter; i++)
    {
        suma_dist += adjacencyMatrix[stack[i]][stack[i + 1]].distance;
        suma_durata += adjacencyMatrix[stack[i]][stack[i + 1]].duration;
    }

    if (suma_dist < minimumDistanceCostTSP || (suma_dist == minimumDistanceCostTSP && suma_durata < minimumDurationCostTSP))
    {
        minimumDistanceCostTSP = suma_dist;
        minimumDurationCostTSP = suma_durata;

        for (int i = 1; i <= stackCounter; i++)
        {
            minimumRouteTSP[i] = stack[i];
            TSP_RouteCounter = stackCounter;
        }
    }
}

void acyclicGraph::back(void)
{
    int vSuccesor, vValid;
    stackCounter = 1;
    acyclicGraph::init_stiva();
    while (stackCounter > 0)
    {
        do
        {
            vSuccesor = acyclicGraph::succesor();
            if (vSuccesor == 1)
                vValid = acyclicGraph::valid();
        } while (vSuccesor == 1 && vValid == 0);
        if (vSuccesor == 1)
            if (acyclicGraph::solutie() == 1)
                acyclicGraph::determinare_traseu_minim();
            else
            {
                stackCounter++;
                acyclicGraph::init_stiva();
            }
        else
            stackCounter--;
    }
}

#pragma region TSP
void TSP(void)
{
#ifdef _WIN32

    changeText(FOREGROUND_INTENSITY | FOREGROUND_BLUE);

    std::cout << std::setw(5) << " "
              << "Se calculeaza traseul cel mai optim...\n";

    resetText();
#elif __linux__
    std::cout << "\033[1;34m" << std::setw(5) << " "
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
        hamiltonianGraph::back();
        clear_screen();
        std::cout << "\n";

        if (!minimumRouteTSP.empty())
        {
#ifdef _WIN32
            changeText(FOREGROUND_INTENSITY);

            std::cout << std::setw(5) << " "
                      << "Lungime traseu: " << minimumDistanceCostTSP << "km | "
                      << " Durata traseu: " << minimumDurationCostTSP << "min"
                      << "\n"
                      << std::setw(5) << " ";

            resetText();
#elif __linux__
            std::cout << std::setw(5) << " "
                      << "\033[1m"
                      << "Lungime traseu: " << minimumDistanceCostTSP << "km | "
                      << "Durata traseu: " << minimumDurationCostTSP << "min\n"
                      << std::setw(5) << " "
                      << "\033[0m";
#endif

            for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
            {
                for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
                {
                    int ID = std::stoi(date_oras->getCityID());
                    if (ID == minimumRouteTSP[i])
                    {
                        std::cout << date_oras->getCityName();
                        if (i < TSP_RouteCounter)
                            std::cout << " --> ";
                        break;
                    }
                }
            }

            std::cout << "\n";
            underline(190, false);
        }
        else
        {
            minimumRouteTSP.clear();
            stack.clear();

            acyclicGraph::back();
            clear_screen();
            std::cout << "\n";

            if (!minimumRouteTSP.empty())
            {
#ifdef _WIN32
                changeText(FOREGROUND_INTENSITY);

                std::cout << std::setw(5) << " "
                          << "Lungime traseu: " << minimumDistanceCostTSP << "km | "
                          << " Durata traseu: " << minimumDurationCostTSP << "min"
                          << "\n"
                          << std::setw(5) << " ";

                resetText();
#elif __linux__
                std::cout << std::setw(5) << " "
                          << "\033[1m"
                          << "Lungime traseu: " << minimumDistanceCostTSP << "km | "
                          << "Durata traseu: " << minimumDurationCostTSP << "min\n"
                          << std::setw(5) << " "
                          << "\033[0m";
#endif

                for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
                {
                    for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
                    {
                        int ID = std::stoi(date_oras->getCityID());
                        if (ID == minimumRouteTSP[i])
                        {
                            std::cout << date_oras->getCityName();
                            if (i < TSP_RouteCounter)
                                std::cout << " --> ";
                            break;
                        }
                    }
                }
                std::cout << "\n";
                underline(190, false);
            }
            else
                std::cout << "Toate depozitele sunt izolate!";
        }
    }
    else
    {
        acyclicGraph::back();
        clear_screen();

        std::cout << "\n";
        if (!minimumRouteTSP.empty())
        {
#ifdef _WIN32
            changeText(FOREGROUND_INTENSITY);

            std::cout << std::setw(5) << " "
                      << "Lungime traseu: " << minimumDistanceCostTSP << "km | "
                      << " Durata traseu: " << minimumDurationCostTSP << "min"
                      << "\n"
                      << std::setw(5) << " ";

            resetText();
#elif __linux__
            std::cout << std::setw(5) << " "
                      << "\033[1m"
                      << "Lungime traseu: " << minimumDistanceCostTSP << "km | "
                      << "Durata traseu: " << minimumDurationCostTSP << "min\n"
                      << std::setw(5) << " "
                      << "\033[0m";
#endif

            for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
            {
                for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
                {
                    int ID = std::stoi(date_oras->getCityID());
                    if (ID == minimumRouteTSP[i])
                    {
                        std::cout << date_oras->getCityName();
                        if (i < TSP_RouteCounter)
                            std::cout << " --> ";
                        break;
                    }
                }
            }
            std::cout << "\n";
            underline(190, false);
        }
        else
            std::cout << "\n"
                      << std::setw(5) << " "
                      << "Toate depozitele sunt izolate!";
    }
}

void produse_transport_TSP(void)
{
    for (PRODUCT::PRODUCT_NODE *date_produs = product.getHead(); date_produs != nullptr; date_produs = date_produs->next)
    {
        int cantitate = 0;
        int ID_PRODUS = std::stoi(date_produs->getProductID());
        for (DEPOT::DEPOT_NODE *date_depozit = depot.getHead(); date_depozit != nullptr; date_depozit = date_depozit->next)
        {
            int ID_DEPOZIT = std::stoi(date_depozit->getCityID());
            if (limitedStockCities[ID_DEPOZIT] == true && isolatedVertex[ID_DEPOZIT] == false && centralDepos[ID_DEPOZIT] == false)
            {
                int ID_PRODUS_DEPOZIT = std::stoi(date_depozit->getProductID());
                if (ID_PRODUS == ID_PRODUS_DEPOZIT)
                    cantitate += MAXIMUM_STOCK_VALUE - date_depozit->getProductQuantity();
            }
        }
        supply.getData(date_produs->getProductID(), cantitate);
    }
}

void pagina_principala_TSP(void)
{
    clear_screen();

    traveledDistanceTSP = 0.0;
    elapsedDurationTSP = 0;

    std::cout << "\n";
    if (!minimumRouteTSP.empty())
    {
        std::cout << std::setw(5) << " "
                  << "Lungime traseu: " << minimumDistanceCostTSP << "km | "
                  << "Durata traseu: " << minimumDurationCostTSP << "min\n"
                  << std::setw(5) << " ";
        for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
        {
            for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
            {
                int ID = std::stoi(date_oras->getCityID());

                if (i == 1)
                    if (ID == minimumRouteTSP[1])
                    {
#ifdef _WIN32
                        changeText(FOREGROUND_INTENSITY | COMMON_LVB_UNDERSCORE | FOREGROUND_RED | FOREGROUND_GREEN);
                        std::cout << date_oras->getCityName();
                        resetText();

                        std::cout << " --> ";
#elif __linux__
                        std::cout << "\033[4m"
                                  << "\033[1m" << date_oras->denumire_oras << "\033[0m"
                                  << " --> ";
#endif
                    }

                if (ID == minimumRouteTSP[i] && i != 1)
                {
                    std::cout << date_oras->getCityName();
                    if (i < TSP_RouteCounter)
                        std::cout << " --> ";
                    break;
                }
            }
        }
    }
    std::cout << "\n";
    underline(190, false);

    std::cout << "\n";
    for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
    {
        int ID = std::stoi(date_oras->getCityID());
        if (ID == minimumRouteTSP[1])
        {
            std::cout << std::setw(5) << " "
                      << "+-----------------------+\n"
                      << std::setw(8) << " [" << date_oras->getCityID() << "] " << date_oras->getCityName() << " - START\n"
                      << std::setw(5) << " "
                      << "+-----------------------+";
            break;
        }
    }

#ifdef _WIN32
    changeText(FOREGROUND_INTENSITY);
    std::cout << std::setw(100) << " "
              << "Distanta parcursa: " << traveledDistanceTSP;
    resetText();
    std::cout << "km | ";
    changeText(FOREGROUND_INTENSITY);
    std::cout << "Durata de calatorie: " << elapsedDurationTSP;
    resetText();
    std::cout << "min\n";

#elif __linux__
    std::cout << std::setw(100) << " "
              << "\033[1m"
              << "Distanta parcursa: "
              << "\033[0m" << traveledDistanceTSP << "km | "
              << "\033[1m"
              << "Durata de calatorie: "
              << "\033[0m" << elapsedDurationTSP << "min\n";
#endif

    std::cout << "\n\n"
              << std::setw(5) << " "
              << "ID" << std::setw(7) << " "
              << "Den.Produs" << std::setw(15) << " "
              << "Cnt.transport_totala\n";
    underline(65, true);

    for (SUPPLY::SUPPLY_NODE *supply_data = supply.getHead(); supply_data != nullptr; supply_data = supply_data->next)
    {
        int ID_AP = std::stoi(supply_data->getID());
        std::cout << std::setw(5 + 1) << " [" << ID_AP << "] " << std::setw(5) << " ";
        for (PRODUCT::PRODUCT_NODE *date_produs = product.getHead(); date_produs != nullptr; date_produs = date_produs->next)
        {
            int ID_P = std::stoi(date_produs->getProductID());
            if (ID_P == ID_AP)
            {
                std::cout << date_produs->getProductName() << std::setw(maxProductNameLength - strlen(date_produs->getProductName()) + 5) << " ";
                break;
            }
        }
        std::cout << supply_data->getQuantity() << " buc.\n";
    }
    underline(65, true);

    std::cout << "\n\n";
    underline(190, false);

    std::cout << std::setw(5) << " "
              << "[1] PREV" << std::setw(80) << " -" << consolePage << "- " << std::setw(80) << " "
              << "[2] NEXT\n";

    underline(190, false);

    traveledDistanceTSP += adjacencyMatrix[minimumRouteTSP[1]][minimumRouteTSP[2]].distance;
    elapsedDurationTSP += adjacencyMatrix[minimumRouteTSP[1]][minimumRouteTSP[2]].duration;
}

void pagina_finala_TSP(void)
{
    std::ifstream get_contor("utils/contor_TSP_log.txt");
    if (!get_contor.is_open())
    {
        std::cerr << std::setw(5) << " "
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

    TSP_RoutesCompleted = true;
    logCounter++;

    std::ofstream file("utils/contor_TSP_log.txt");
    if (!file.is_open())
    {
        std::cerr << std::setw(5) << " "
                  << "Failed to open file!\n";
        _getch();
    }
    else
        file << logCounter;

    file.close();

    if (!depot.refreshData())
        std::cerr << std::setw(5) << " "
                  << "Could not update the database!\n";
    else if (fetchTables() == EXIT_FAILURE)
        _getch();

    std::ofstream log_out;
    log_out.open("logs/TSP_log.txt", std::ios::app);

    if (!log_out.is_open())
    {
        std::cerr << std::setw(5) << " "
                  << "Failed to open TSP log!\n";
        _getch();
    }
    else
    {
        std::string s(500, '=');
        log_out << "LOG [" << logCounter << "]\n";

        for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
        {
            for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
            {
                int ID = std::stoi(date_oras->getCityID());
                if (ID == minimumRouteTSP[i])
                {
                    log_out << date_oras->getCityName();
                    if (i < TSP_RouteCounter)
                        log_out << " --> ";
                    break;
                }
            }
        }

        log_out << "\n"
                << s << "\n"
                << "Distanta parcursa: " << minimumDistanceCostTSP << "km\n"
                << "Cantitate totala transportata: " << totalSuppliedQuantity << "BUC.\n"
                << "Cost total: " << totalSupplyCost << "RON\nEND-LOG\n"
                << s << "\n\n\n";

        log_out.close();
    }

    clear_screen();

    std::cout << "\n";
    underline(50, true);

    std::cout << std::setw(5) << " "
              << "-> Aprovizionare completa!\n"
              << std::setw(5) << " "
              << "-> Baza de date reinprostpatata!\n"
              << std::setw(5) << " "
              << "-> Mai multe detalii in fisierul TSP_log.txt!\n";

    underline(50, true);

    std::cout << std::setw(5) << " "
              << "Apasati 'ENTER' pentru a va intoarce...";

    _getch();
}

void pagina_stanga_TSP(void)
{
    clear_screen();

    if (consolePage - 1 > 1)
    {
        consolePage--;

        traveledDistanceTSP -= adjacencyMatrix[minimumRouteTSP[consolePage]][minimumRouteTSP[consolePage + 1]].distance;
        elapsedDurationTSP -= adjacencyMatrix[minimumRouteTSP[consolePage]][minimumRouteTSP[consolePage + 1]].duration;

        std::cout << "\n";
        if (!minimumRouteTSP.empty())
        {
            std::cout << std::setw(5) << " "
                      << "Lungime traseu: " << minimumDistanceCostTSP << " | "
                      << "Durata traseu: " << minimumDurationCostTSP << "min\n"
                      << std::setw(5) << " ";

            for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
            {
                for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
                {
                    int ID = std::stoi(date_oras->getCityID());

                    if (ID == minimumRouteTSP[consolePage] && i == consolePage)
                    {
#ifdef _WIN32
                        changeText(FOREGROUND_INTENSITY | COMMON_LVB_UNDERSCORE | FOREGROUND_RED | FOREGROUND_GREEN);

                        std::cout << date_oras->getCityName();

                        resetText();
#elif __linux__
                        std::cout << "\033[4m"
                                  << "\033[1m" << date_oras->denumire_oras << "\033[0m"
                                  << " --> ";
#endif
                        if (i < TSP_RouteCounter)
                            std::cout << " --> ";
                        break;
                    }

                    if (ID == minimumRouteTSP[i] && i != consolePage)
                    {
                        std::cout << date_oras->getCityName();
                        if (i < TSP_RouteCounter)
                            std::cout << " --> ";
                        break;
                    }
                }
            }
        }
        std::cout << "\n";
        underline(190, false);
        std::cout << "\n";

        for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
        {
            int ID = std::stoi(date_oras->getCityID());
            if (ID == minimumRouteTSP[consolePage])
            {
                std::cout << std::setw(5) << " "
                          << "+-----------------------+\n"
                          << std::setw(8) << " [" << date_oras->getCityID() << "] " << date_oras->getCityName() << "\n"
                          << std::setw(5) << " "
                          << "+-----------------------+";
                break;
            }
        }

#ifdef _WIN32
        changeText(FOREGROUND_INTENSITY);
        std::cout << std::setw(100) << " "
                  << "Distanta parcursa: " << traveledDistanceTSP;
        resetText();
        std::cout << "km | ";
        changeText(FOREGROUND_INTENSITY);
        std::cout << "Durata de calatorie: " << elapsedDurationTSP;
        resetText();
        std::cout << "min\n";

#elif __linux__
        std::cout << std::setw(100) << " "
                  << "\033[1m"
                  << "Distanta parcursa: "
                  << "\033[0m" << traveledDistanceTSP << "km | "
                  << "\033[1m"
                  << "Durata de calatorie: "
                  << "\033[0m" << elapsedDurationTSP << "min\n";
#endif

        std::cout << "\n\n"
                  << std::setw(5) << " "
                  << "ID" << std::setw(10) << " "
                  << "Den.Produs" << std::setw(15) << " "
                  << "Cnt.transport/Cnt.ramasa\n";
        underline(70, true);

        if (centralDepos[minimumRouteTSP[consolePage]] == false)
        {
            if (limitedStockCities[minimumRouteTSP[consolePage]] == false)
            {
#ifdef _WIN32
                changeText(FOREGROUND_INTENSITY);

                std::cout << std::setw(5) << " "
                          << "Depozitul a fost aprovizionat!"
                          << "\n";

                resetText();
#elif __linux__
                std::cout << std::setw(5) << " "
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

            std::cout << std::setw(5) << " "
                      << "Depozit centralizat!!"
                      << "\n";

            resetText();
#elif __linux__
            std::cout << std::setw(5) << " "
                      << "\033[1m"
                      << "Depozit centralizat!\n"
                      << "\033[0m";
#endif
        }

        underline(70, true);

        std::cout << "\n\n";
        underline(190, false);

        std::cout << std::setw(5) << " "
                  << "[1] PREV" << std::setw(80) << " -" << consolePage << "- " << std::setw(80) << " "
                  << "[2] NEXT\n";

        underline(190, false);
    }
    else
    {
        consolePage = 1;
        pagina_principala_TSP();
    }
}

void pagina_dreapta_TSP(void)
{
    clear_screen();

    if (consolePage < TSP_RouteCounter)
    {
        if (consolePage > 2)
        {
            traveledDistanceTSP += adjacencyMatrix[minimumRouteTSP[consolePage]][minimumRouteTSP[consolePage + 1]].distance;
            elapsedDurationTSP += adjacencyMatrix[minimumRouteTSP[consolePage]][minimumRouteTSP[consolePage + 1]].duration;
        }

        consolePage++;

        std::cout << "\n";
        if (!minimumRouteTSP.empty())
        {
            std::cout << std::setw(5) << " "
                      << "Lungime traseu: " << minimumDistanceCostTSP << "km | "
                      << "Durata traseu: " << minimumDurationCostTSP << "min\n"
                      << std::setw(5) << " ";

            for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
            {
                for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
                {
                    int ID = std::stoi(date_oras->getCityID());

                    if (ID == minimumRouteTSP[consolePage] && i == consolePage)
                    {
#ifdef _WIN32
                        changeText(FOREGROUND_INTENSITY | COMMON_LVB_UNDERSCORE | FOREGROUND_RED | FOREGROUND_GREEN);

                        std::cout << date_oras->getCityName();

                        resetText();
#elif __linux__
                        std::cout << "\033[4m"
                                  << "\033[1m" << date_oras->denumire_oras << "\033[0m"
                                  << " --> ";
#endif
                        if (i < TSP_RouteCounter)
                            std::cout << " --> ";
                        break;
                    }

                    if (ID == minimumRouteTSP[i] && i != consolePage)
                    {
                        std::cout << date_oras->getCityName();
                        if (i < TSP_RouteCounter)
                            std::cout << " --> ";
                        break;
                    }
                }
            }
        }
        std::cout << "\n";
        underline(190, false);
        std::cout << "\n";

        for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
        {
            int ID = std::stoi(date_oras->getCityID());
            if (ID == minimumRouteTSP[consolePage])
            {
                std::cout << std::setw(5) << " "
                          << "+-----------------------+\n"
                          << std::setw(8) << " [" << date_oras->getCityID() << "] " << date_oras->getCityName() << "\n"
                          << std::setw(5) << " "
                          << "+-----------------------+";
                break;
            }
        }

#ifdef _WIN32
        changeText(FOREGROUND_INTENSITY);
        std::cout << std::setw(100) << " "
                  << "Distanta parcursa: " << traveledDistanceTSP;
        resetText();
        std::cout << "km | ";
        changeText(FOREGROUND_INTENSITY);
        std::cout << "Durata de calatorie: " << elapsedDurationTSP;
        resetText();
        std::cout << "min\n";

#elif __linux__
        std::cout << std::setw(100) << " "
                  << "\033[1m"
                  << "Distanta parcursa: "
                  << "\033[0m" << traveledDistanceTSP << "km | "
                  << "\033[1m"
                  << "Durata de calatorie: "
                  << "\033[0m" << elapsedDurationTSP << "min\n";
#endif

        std::cout << "\n\n"
                  << std::setw(5) << " "
                  << "ID" << std::setw(10) << " "
                  << "Den.Produs" << std::setw(15) << " "
                  << "Cnt.transport/Cnt.ramasa\n";
        underline(70, true);

        if (centralDepos[minimumRouteTSP[consolePage]] == false)
        {
            if (limitedStockCities[minimumRouteTSP[consolePage]])
            {
                for (PRODUCT::PRODUCT_NODE *date_produs = product.getHead(); date_produs != nullptr; date_produs = date_produs->next)
                {
                    int ID_PRODUS = std::stoi(date_produs->getProductID());
                    double cantitate_necesara = 0.0;

                    for (DEPOT::DEPOT_NODE *date_depozit = depot.getHead(); date_depozit != nullptr; date_depozit = date_depozit->next)
                    {
                        int ID_PRODUS_DEPOZIT = std::stoi(date_depozit->getProductID()), ID_DEPOZIT = std::stoi(date_depozit->getCityID());
                        if (ID_DEPOZIT == minimumRouteTSP[consolePage] && !isolatedVertex[ID_DEPOZIT])
                        {
                            if (ID_PRODUS_DEPOZIT == ID_PRODUS)
                            {
                                cantitate_necesara += date_depozit->getProductQuantity();
                                cantitate_necesara = MAXIMUM_STOCK_VALUE - cantitate_necesara;
                                totalSupplyCost += cantitate_necesara * date_produs->getProductPrice();
                                totalSuppliedQuantity += cantitate_necesara;
                                date_depozit->updateQuantity(MAXIMUM_STOCK_VALUE);

                                std::cout << std::setw(5 + 1) << " [" << date_depozit->getProductID() << "] " << std::setw(8) << " " << date_produs->getProductName()
                                          << std::setw(maxProductNameLength - strlen(date_produs->getProductName()) + 5) << " " << cantitate_necesara << " buc. /";

                                for (SUPPLY::SUPPLY_NODE *supply_data = supply.getHead(); supply_data != nullptr; supply_data = supply_data->next)
                                {
                                    int ID_PRODUS_APROVIZIONARE = std::stoi(supply_data->getID());
                                    if (ID_PRODUS_APROVIZIONARE == ID_PRODUS)
                                    {
                                        std::cout << supply_data->getQuantity() << " buc.";
                                        int currentQuantity = supply_data->getQuantity();
                                        supply_data->updateQuantity(currentQuantity - cantitate_necesara);
                                        break;
                                    }
                                }
                                std::cout << "\n";
                            }
                        }
                    }
                }
            }
            else
            {
#ifdef _WIN32
                changeText(FOREGROUND_INTENSITY);

                std::cout << std::setw(5) << " "
                          << "Depozitul a fost aprovizionat!"
                          << "\n";

                resetText();
#elif __linux__
                std::cout << std::setw(5) << " "
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

            std::cout << std::setw(5) << " "
                      << "Depozit centralizat!!"
                      << "\n";

            resetText();
#elif __linux__
            std::cout << std::setw(5) << " "
                      << "\033[1m"
                      << "Depozit centralizat!\n"
                      << "\033[0m";
#endif
        }

        underline(70, true);

        std::cout << "\n\n";
        underline(190, false);

        std::cout << std::setw(5) << " "
                  << "[1] PREV" << std::setw(80) << " -" << consolePage << "- " << std::setw(80) << " "
                  << "[2] NEXT\n";

        underline(190, false);

        limitedStockCities[minimumRouteTSP[consolePage]] = false;

        if (consolePage - 1 == 1)
        {
            traveledDistanceTSP += adjacencyMatrix[minimumRouteTSP[consolePage]][minimumRouteTSP[consolePage + 1]].distance;
            elapsedDurationTSP += adjacencyMatrix[minimumRouteTSP[consolePage]][minimumRouteTSP[consolePage + 1]].duration;
        }
    }
    else
        pagina_finala_TSP();
}

void parcurgere_traseu_TSP(void)
{
    consolePage = 1;
    std::cout << "\n";

    if (!TSP_RoutesCompleted)
    {
        if (minimumRouteTSP[1] == -1)
            TSP();
        else
        {
            clear_screen();
#ifdef _WIN32
            changeText(FOREGROUND_INTENSITY);

            std::cout << std::setw(5) << " "
                      << "Lungime traseu: " << minimumDistanceCostTSP << "km | "
                      << " Durata traseu: " << minimumDurationCostTSP << "\n"
                      << std::setw(5) << " ";

            resetText();
#elif __linux__
            std::cout << std::setw(5) << " "
                      << "\033[1m"
                      << "Lungime traseu: " << minimumDistanceCostTSP << "km | "
                      << "Durata traseu: " << minimumDurationCostTSP << "\n"
                      << std::setw(5) << " "
                      << "\033[0m";
#endif
            for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
            {
                for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
                {
                    int ID = std::stoi(date_oras->getCityID());
                    if (ID == minimumRouteTSP[i])
                    {
                        std::cout << date_oras->getCityName();
                        if (i < TSP_RouteCounter)
                            std::cout << " --> ";
                        break;
                    }
                }
            }

            std::cout << "\n";
            underline(190, false);
        }
    }
    else
    {
        clear_screen();

#ifdef _WIN32
        changeText(FOREGROUND_INTENSITY);

        std::cout << "\n\n"
                  << std::setw(5) << " "
                  << "Nu exista depozite cu stoc limitat/epuizat...";

        resetText();
#elif __linux__
        std::cout << "\n\n"
                  << std::setw(5) << " "
                  << "\033[1m"
                  << "Nu exista depozite cu stoc limitat/epuizat..."
                  << "\033[0m";
#endif
        minimumRouteTSP.clear();
        _getch();
        return;
    }

    char *input = (char *)malloc(MAXL * sizeof(char) + 1);

    std::cout << std::setw(5) << " "
              << "Scrie '0' pentru a te intoarce...\n\n";

    std::cout << std::setw(5) << " "
              << "[S] Start: ";
    std::cin >> input;

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
            std::cout << "\n"
                      << std::setw(5) << " "
                      << "Introduceti numarul meniului: ";
            std::cin >> MENIU;

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
        } while (MENIU != 0 && TSP_RoutesCompleted == false);
    }
    free(input);
}
#pragma endregion

void afisare_detalii_SpeedyGo(void)
{
    std::cout << "\n";
    underline(100, true);

    std::cout << "\033[3m"
              << std::setw(5) << " "
              << "Bun venit in Consola MySQL. Introduceti o interogare SQL (sau 'exit' pentru a incheia)\n"
              << "\033[0m";

    underline(100, true);

    sql::Statement *stmt = con->createStatement();
    sql::ResultSet *res = stmt->executeQuery("SHOW TABLES");

    std::cout << std::setw(5) << " "
              << "+" << std::string(22, '-') << "+"
              << "\n";
    std::cout << std::setw(5) << " "
              << "| " << std::left << std::setw(20) << "SpeedyGo - Tabele"
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

void consola_mysql(void)
{
    clear_screen();

    if (buffer)
    {
        buffer = false;
        std::cin.ignore(9999, '\n');
    }

    afisare_detalii_SpeedyGo();

    std::string interogare;

    while (true)
    {
        std::cout << "\n"
                  << std::setw(5) << " "
                  << "mysql> ";

        std::getline(std::cin, interogare);

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
                std::vector<int> coloane(cnt_coloane, 0);

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
                std::cout << std::setw(5) << " "
                          << "Error code: " << e.getErrorCode() << "\n";
                std::cout << std::setw(5) << " "
                          << "Error message: " << e.what() << "\n";
                std::cout << std::setw(5) << " "
                          << "SQLState: " << e.getSQLState() << "\n";
            }
        }
    }

    interogare.clear();
}