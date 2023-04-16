#ifndef LOGIC
#define LOGIC

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <limits.h>
#include <string.h>
#include "declarations.hpp"
#include <nlohmann/json.hpp>

using namespace std;
using namespace nlohmann;

/*
double distantaCalc(double lat1, double lon1, double lat2, double lon2)
{
    constexpr double R = 6371.0;
    double lat_rad1 = M_PI * lat1 / 180.0,
           lon_rad1 = M_PI * lon1 / 180.0,
           lat_rad2 = M_PI * lat2 / 180.0,
           lon_rad2 = M_PI * lon2 / 180.0,
           dlon = lon_rad2 - lon_rad1,
           dlat = lat_rad2 - lat_rad1,
           a = pow(sin(dlat / 2), 2) + cos(lat_rad1) * cos(lat_rad2) * pow(sin(dlon / 2), 2),
           c = 2 * atan2(sqrt(a), sqrt(1 - a)),
           d = R * c;
    return d;
}
*/

bool _init_()
{
    ifstream file_json("distance_matrix.json");
    if (!file_json.is_open())
    {
        cerr << "Eroare\n";
        return false;
    }
    else
    {
        json data;
        try
        {
            file_json >> data;
        }
        catch (json::parse_error &e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }
        
        for (json::iterator i = data.begin(); i != data.end(); i++)
        {
            char *pereche_orase = (char *)malloc(MAXL * sizeof(char) + 1);
            strcpy(pereche_orase, i.key().c_str());
            char *ptr = strtok(pereche_orase, "_");
            char *oras1 = (char *)malloc(MAXL * sizeof(char) + 1);
            char *oras2 = (char *)malloc(MAXL * sizeof(char) + 1);
            int ID_Oras1, ID_Oras2;
            bool nume_oras = true;

            if (ptr != NULL)
            {
                strcpy(oras1, ptr);
                ptr = strtok(NULL, "_");
            }

            if (ptr != NULL)
                strcpy(oras2, ptr);

            NOD_ORASE *p = head_oras;
            bool exit = false;
            while (p != nullptr && !exit)
            {
                if (strcasecmp(p->denumire_oras, oras1) == 0)
                {
                    exit = true;
                    ID_Oras1 = stoi(p->ID_Oras);
                }
                p = p->next_o;
            }

            exit = false;
            p = head_oras;

            while (p != nullptr && !exit)
            {
                if (strcasecmp(p->denumire_oras, oras2) == 0)
                {
                    exit = true;
                    ID_Oras2 = stoi(p->ID_Oras);
                }
                p = p->next_o;
            }
            
            matrice_drum[ID_Oras1][ID_Oras2] = matrice_drum[ID_Oras2][ID_Oras1] = i.value();
        }
        return true;
    }
}

void inserareDateDepozit(char *vID_Produs, char *vID_Depozit, double vCantitate_Produs)
{
    NOD_PRODUS_DEPOZIT *newnod = (NOD_PRODUS_DEPOZIT *)malloc(sizeof(NOD_PRODUS_DEPOZIT));
    newnod->ID_Produs = strdup(vID_Produs);
    newnod->ID_Depozit = strdup(vID_Depozit);
    newnod->Cantitate_Produs = vCantitate_Produs;

    if (head_depozit == nullptr)
    {
        head_depozit = newnod;
        tail_depozit = newnod;
    }
    else
    {
        tail_depozit->next_d = newnod;
        newnod->prev_d = tail_depozit;
        tail_depozit = newnod;
    }
}

void inserareDateProduse(char *vID_Produs, char *vDenumire_Produs, char *vCategorie_Produs, double vPret_Produs)
{
    NOD_DETALII_PRODUS *newnod = (NOD_DETALII_PRODUS *)malloc(sizeof(NOD_DETALII_PRODUS));

    newnod->ID_Produs = strdup(vID_Produs);
    newnod->Denumire_Produs = strdup(vDenumire_Produs);
    newnod->Categorie_Produs = strdup(vCategorie_Produs);
    newnod->pret_produs = vPret_Produs;

    if (head_produs == nullptr)
    {
        head_produs = newnod;
        tail_produs = newnod;
    }
    else
    {
        tail_produs->next_p = newnod;
        newnod->prev_p = tail_produs;
        tail_produs = newnod;
    }
}

void inserareDateProduseLocal(char *vID_Oras, char *vID_Produs, double vCantitate_Produs_Local)
{
    NOD_PRODUS_DEPOZIT_LOCAL *newnod = (NOD_PRODUS_DEPOZIT_LOCAL *)malloc(sizeof(NOD_PRODUS_DEPOZIT_LOCAL));

    newnod->ID_Oras = strdup(vID_Oras);
    newnod->ID_Produs = strdup(vID_Produs);
    newnod->cantitate_produs = vCantitate_Produs_Local;

    if (head_local == nullptr)
    {
        head_local = newnod;
        tail_local = newnod;
    }
    else
    {
        tail_local->next_l = newnod;
        newnod->prev_l = tail_local;
        tail_local = newnod;
    }
}

void insearareDateOrase(char *vID_Oras, char *vDenumire_Oras, double vLatitudine, double vLongitudine)
{
    NOD_ORASE *newnod = (NOD_ORASE *)malloc(sizeof(NOD_ORASE));

    newnod->ID_Oras = strdup(vID_Oras);
    newnod->denumire_oras = strdup(vDenumire_Oras);
    newnod->latitudine = vLatitudine;
    newnod->longitudine = vLongitudine;

    if (head_oras == nullptr)
    {
        head_oras = newnod;
        tail_oras = newnod;
    }
    else
    {
        tail_oras->next_o = newnod;
        newnod->prev_o = tail_oras;
        tail_oras = newnod;
    }
}

void afisareDateOrase()
{
    NOD_ORASE *ptr;
    ptr = head_oras;
    while (ptr != nullptr)
    {
        cout << "ID_Oras: " << ptr->ID_Oras << " ";
        cout << "Denumire: " << ptr->denumire_oras << endl;
        ptr = ptr->next_o;
    }
}

void afisareDateLocal()
{
    NOD_PRODUS_DEPOZIT_LOCAL *ptr;
    ptr = head_local;
    while (ptr != nullptr)
    {
        cout << "ID_Oras: " << ptr->ID_Oras << " ";
        cout << "ID_Produs: " << ptr->ID_Produs << " ";
        cout << "Cantitate: " << ptr->cantitate_produs << endl;
        ptr = ptr->next_l;
    }
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

void statisticaStoc()
{
    NOD_PRODUS_DEPOZIT_LOCAL *ptr = head_local;
    while (ptr != nullptr)
    {
        if (ptr->cantitate_produs < 50)
        {
            int tempIdOras = stoi(ptr->ID_Oras);
            if (tempIdOras > 5)
                matrice_drum[tempIdOras][tempIdOras] = 1;
        }
        ptr = ptr->next_l;
    }

    /*
    for (unsigned int i = 1; i <= 20; i++)
    {
        for (unsigned int j = 1; j <= 5; j++)
            cout << matrice_aprovizionare[i][j] << " ";
        cout << endl;
    }

    for (unsigned int i = 1; i < matrice_drum.size(); i++)
    {
        for (unsigned int j = 1; j < matrice_drum.size(); j++)
            if (matrice_drum[i][j] == 1)
                cout << matrice_drum[i][j] << " ";
            else
                cout << 0 << " ";
        cout << endl;
    }
    */
}

void afisareSolutieDistanta(int start, vector<double> &distanta, vector<int> &distanta_minima)
{
    for (int i = 0; i < N - 1; i++)
    {
        if (i != start)
        {
            cout << "Shortest distance from " << start << " to " << i << " is " << distanta[i] << ". cale: ";
            vector<int> cale;
            int node = i;
            while (node != -1)
            {
                cale.push_back(node);
                node = distanta_minima[node];
            }
            reverse(cale.begin(), cale.end());
            for (int j = 0; j < cale.size(); j++)
            {
                cout << cale[j];
                if (j != cale.size() - 1)
                    cout << " -> ";
            }
            cout << endl;
        }
    }
}

void dijkstra(int start, vector<double> &distanta, vector<int> &distanta_minima)
{
    vector<bool> visited(matrice_drum.size(), false);
    distanta[start] = 0.0;

    for (int i = 1; i <= matrice_drum.size() - 2; i++)
    {
        int min_index = -1;
        double min_dist = numeric_limits<double>::infinity();

        for (int j = 1; j <= matrice_drum.size() - 2; j++)
        {
            if (!visited[j] && distanta[j] < min_dist)
            {
                min_index = j;
                min_dist = distanta[j];
            }
        }

        visited[min_index] = true;

        for (int j = 1; j <= matrice_drum.size() - 2; j++)
        {
            double new_dist = distanta[min_index] + matrice_drum[min_index][j];
            if (!visited[j] && matrice_drum[min_index][j] > 0 && new_dist < distanta[j])
            {
                distanta[j] = new_dist;
                distanta_minima[j] = min_index;
            }
        }
    }
}

void determinareStartAprovizionare()
{
    vector<int> distanta_minima(N, -1);
    vector<double> distanta(N, numeric_limits<double>::infinity());
    int start;
    cin >> start;
    dijkstra(start, distanta, distanta_minima);
    afisareSolutieDistanta(start, distanta, distanta_minima);

    distanta_minima.clear();
    distanta.clear();
}

#endif