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

void autentificare()
{

}

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

            ORAS::NOD_ORAS *p = oras.getHead();
            bool exit = false;
            while (p != nullptr && !exit)
            {
                if (strcasecmp(p->denumire_oras, oras1) == 0)
                {
                    exit = true;
                    ID_Oras1 = stoi(p->ID_Oras);
                }
                p = p->next;
            }

            exit = false;
            p = oras.getHead();

            while (p != nullptr && !exit)
            {
                if (strcasecmp(p->denumire_oras, oras2) == 0)
                {
                    exit = true;
                    ID_Oras2 = stoi(p->ID_Oras);
                }
                p = p->next;
            }
            
            matrice_drum[ID_Oras1][ID_Oras2] = matrice_drum[ID_Oras2][ID_Oras1] = i.value();

            free(pereche_orase);
            free(oras1);
            free(oras2);
        }
        return true;
    }
}

void afisareDateOrase()
{
    ORAS::NOD_ORAS *ptr = oras.getHead();
    while (ptr != nullptr)
    {
        cout << "ID_Oras: " << ptr->ID_Oras << " ";
        cout << "Denumire: " << ptr->denumire_oras << " ";
        cout << "Tip: " << ptr->tip_depozit << endl;
        ptr = ptr->next;
    }
}

void afisareDateDepozit()
{
    DEPOZIT::NOD_DEPOZIT *ptr = depozit.getHead();
    while (ptr != nullptr)
    {
        cout << "ID_Produs: " << ptr->ID_Produs << ", ";
        cout << "ID_Depozit: " << ptr->ID_Depozit << ", ";
        cout << "Cantitate_Produs: " << ptr->Cantitate_Produs << ", ";
        cout << "ID_oras: " << ptr->ID_Oras << endl;
        ptr = ptr->next;
    }
}

void afisareDateProdus()
{
    DETALII_PRODUS::NOD_DETALII_PRODUS *ptr = produs.getHead();
    while (ptr != nullptr)
    {
        cout << "ID_Produs: " << ptr->ID_Produs << " ";
        cout << "Nume_Produs: " << ptr->Denumire_Produs << endl;
        ptr = ptr->next;
    }
}


void cautareDepozit()
{
    char *ID = (char *)malloc(MAXL * sizeof(char));
    DEPOZIT::NOD_DEPOZIT *ptr = depozit.getHead();

    cin >> ID;
    statisticaStoc();
    cout << endl;
    while (ptr != nullptr)
    {
        if (strcasecmp(ID, ptr->ID_Depozit) == 0)
        {
            cout << "ID_Produs: " << ptr->ID_Produs << ", ";
            cout << "ID_Depozit: " << ptr->ID_Depozit << ", ";
            cout << "Cantitate_Produs: " << ptr->Cantitate_Produs << ", ";
            cout << "ID_oras: " << ptr->ID_Oras << endl;
        }
        ptr = ptr->next;
    }
    free(ID);
}


void bsort()
{
    DEPOZIT::NOD_DEPOZIT *lptr = nullptr;
    if (depozit.getHead() == nullptr)
        return;
    bool vsort = true;
    do
    {
        vsort = true;
        DEPOZIT::NOD_DEPOZIT *ptr = depozit.getHead();
        while (ptr->next != nullptr)
        {
            int ID1 = stoi(ptr->ID_Produs);
            int ID2 = stoi(ptr->next->ID_Produs);
            if (ID1 > ID2)
            {
                swap(ptr->ID_Produs, ptr->next->ID_Produs);
                vsort = false;
            }
            ptr = ptr->next;
        }
        lptr = ptr;
    } while (!vsort);
    DEPOZIT::NOD_DEPOZIT *ptr = depozit.getHead();
    while (ptr != nullptr)
    {
        cout << "ID_Produs: " << ptr->ID_Produs << ", ";
        cout << "ID_Depozit: " << ptr->ID_Depozit << ", ";
        cout << "Cantitate_Produs: " << ptr->Cantitate_Produs << ", ";
        cout << "ID_oras: " << ptr->ID_Oras << endl;
        ptr = ptr->next;
    }
}


void statisticaStoc()
{
    DEPOZIT::NOD_DEPOZIT *ptr = depozit.getHead();
    while (ptr != nullptr)
    {
        if (ptr->Cantitate_Produs < 50)
        {
            int tempIdOras = stoi(ptr->ID_Oras);
            if (tempIdOras > 5)
                matrice_drum[tempIdOras][tempIdOras] = 1;
        }
        ptr = ptr->next;
    }

    /*
    for (unsigned int i = 1; i <= matrice_drum.size() - 1; i++)
        if (matrice_drum[i][i] == true)
        {
            ptr = depozit.getHead();
            while (ptr != nullptr)
            {
                string c = to_string(i);
                if (strcasecmp(c.c_str(), ptr->ID_Oras) == 0 && ptr->Cantitate_Produs < 50)
                {
                    cout << ptr->ID_Depozit << " ";
                    cout << ptr->ID_Produs << " ";
                    cout << ptr->Cantitate_Produs << endl;
                }
                ptr = ptr->next;
            }
        }*/
}

void afisareSolutieDistanta(int start, vector<double> &distanta, vector<int> &distanta_minima)
{
    for (unsigned int i = 1; i <= N - 1; i++)
    {
        if (i != start && matrice_drum[i][i] == true)
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

void sistem_aprovizionare()
{
    ORAS::NOD_ORAS *ptr = oras.getHead();
    while (ptr != nullptr)
    {
        if (strcasecmp(ptr->tip_depozit, "centralizat") == 0)
        {   
            int ID = stoi(ptr->ID_Oras);
            depozite_centralizate[ID] = true;
            contor_depozite_centralizate++;
        }
        ptr = ptr->next;
    }

    statisticaStoc();

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