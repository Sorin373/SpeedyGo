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

void inserareDateDepozit(char *vID_Produs, char *vID_Depozit, char *vID_Oras, double vCantitate_Produs)
{
    NOD_DEPOZIT *newnod = (NOD_DEPOZIT *)malloc(sizeof(NOD_DEPOZIT));
    newnod->ID_Produs = strdup(vID_Produs);
    newnod->ID_Depozit = strdup(vID_Depozit);
    newnod->ID_Oras = strdup(vID_Oras);
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

void insearareDateOrase(char *vID_Oras, char *vDenumire_Oras, char *vTip_Depozit, double vLatitudine, double vLongitudine)
{
    NOD_ORASE *newnod = (NOD_ORASE *)malloc(sizeof(NOD_ORASE));

    newnod->ID_Oras = strdup(vID_Oras);
    newnod->denumire_oras = strdup(vDenumire_Oras);
    newnod->tip_depozit = strdup(vTip_Depozit);
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
        cout << "Denumire: " << ptr->denumire_oras << " ";
        cout << "Tip: " << ptr->tip_depozit << endl;
        ptr = ptr->next_o;
    }
}

void afisareDateDepozit()
{
    NOD_DEPOZIT *ptr;
    ptr = head_depozit;
    while (ptr != nullptr)
    {
        cout << "ID_Produs: " << ptr->ID_Produs << ", ";
        cout << "ID_Depozit: " << ptr->ID_Depozit << ", ";
        cout << "Cantitate_Produs: " << ptr->Cantitate_Produs << ", ";
        cout << "ID_oras: " << ptr->ID_Oras << endl; 
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
    NOD_DEPOZIT *ptr = head_depozit;
    while (ptr != nullptr)
    {
        if (ptr->Cantitate_Produs < 50)
        {
            int tempIdOras = stoi(ptr->ID_Oras);
            if (tempIdOras > 5)
                matrice_drum[tempIdOras][tempIdOras] = 1;
        }
        ptr = ptr->next_d;
    }
}

void afisareSolutieDistanta(int start, vector<double> &distanta, vector<int> &distanta_minima)
{
    for (unsigned int i = 1; i <= N - 1; i++)
    {
        if (i != start)
        {
            cout << "Shortest distance from " << start << " to " << i << " is " << distanta[i] << ". traseu: ";
            vector<int> traseu;
            int nod = i;
            while (nod != -1)
            {
                traseu.push_back(nod);
                nod = distanta_minima[nod];
            }
            reverse(traseu.begin(), traseu.end());
            for (unsigned int j = 0; j < traseu.size(); j++)
            {
                cout << traseu[j];
                if (j != traseu.size() - 1)
                    cout << " -> ";
            }
            cout << endl;
        }
    }
}

void dijkstra(int start, vector<double> &distanta, vector<int> &distanta_minima)
{
    vector<bool> visited(matrice_drum.size() - 1, false);
    distanta[start] = 0.0;

    for (unsigned int i = 1; i <= matrice_drum.size() - 1; i++)
    {
        int min_index = -1;
        double min_dist = numeric_limits<double>::infinity();

        for (unsigned int j = 1; j <= matrice_drum.size() - 1; j++)
            if (!visited[j] && distanta[j] < min_dist)
            {
                min_index = j;
                min_dist = distanta[j];
            }

        visited[min_index] = true;

        for (unsigned int j = 1; j <= matrice_drum.size() - 1; j++)
        {
            double distanta_noua = distanta[min_index] + matrice_drum[min_index][j];
            if (!visited[j] && matrice_drum[min_index][j] > 0 && distanta_noua < distanta[j])
            {
                distanta[j] = distanta_noua;
                distanta_minima[j] = min_index;
            }
        }
    }
}

void determinareStartAprovizionare()
{
    NOD_ORASE *ptr = head_oras;
    while (ptr != nullptr)
    {
        if (strcasecmp(ptr->tip_depozit, "centralizat") == 0)
        {   
            int ID = stoi(ptr->ID_Oras);
            depozite_centralizate[ID] = true;
            contor_depozite_centralizate++;
        }
        ptr = ptr->next_o;
    }

    for (unsigned int i = 1; i <= contor_depozite_centralizate; i++)
    {
        vector<int> distanta_minima(N, -1);
        vector<double> distanta(N, numeric_limits<double>::infinity());
        if (depozite_centralizate[i])
        {
            dijkstra(i, distanta, distanta_minima);
            afisareSolutieDistanta(i, distanta, distanta_minima);
            getch();
            clear_screen();
        }
        distanta_minima.clear();
        distanta.clear();
    }
}

#endif