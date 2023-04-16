#ifndef LOGIC
#define LOGIC

#include <iostream>
#include <cmath>
#include <limits.h>
#include <string.h>
#include <vector>
#include "declarations.hpp"

using namespace std;

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

void _init_()
{
    NOD_ORASE *ptr = head_oras;
    while (ptr != nullptr)
    {
        int ID_Oras = stoi(ptr->ID_Oras);
        NOD_ORASE *tempPtr = head_oras;
        while (tempPtr != nullptr)
        {
            int sID_Oras = stoi(tempPtr->ID_Oras);
            if (ID_Oras != sID_Oras)
            {
                if (sID_Oras >= N)
                    matrice_drum.resize(sID_Oras + 1);
                if (matrice_drum[sID_Oras].size() <= ID_Oras)
                    matrice_drum[sID_Oras].resize(ID_Oras + 1);

                matrice_drum[ID_Oras][sID_Oras] = matrice_drum[sID_Oras][ID_Oras] = distantaCalc(ptr->latitudine, ptr->longitudine, tempPtr->latitudine, tempPtr->longitudine);
            }
            tempPtr = tempPtr->next_o;
        }
        ptr = ptr->next_o;
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

    cout << "\n\n\n\n\n";
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

int distantaMinima(vector<double> distanta, vector<bool> vizitat)
{
    NOD_ORASE *ptr = head_oras;

    double minim = INT_MAX;
    int minim_index;

    while (ptr != nullptr)
    {
        int ID_Oras = stoi(ptr->ID_Oras);
        if (vizitat[ID_Oras] == false && distanta[ID_Oras] <= minim)
        {
            minim = distanta[ID_Oras];
            minim_index = ID_Oras;
        }
        ptr = ptr->next_o;
    }
    return minim_index;
}

void afisareSolutieDistanta(vector<double> distanta)
{
    for (unsigned int i = 6; i <= matrice_drum[0].size(); i++)
        if (matrice_drum[i][i] > 0)
            cout << i << " " << distanta[i] << endl;
}

void dijkstra(int sursa)
{
    vector<double> distanta(N);
    vector<bool> vizitat(N);

    for (unsigned int i = 1; i <= 44; i++)
    {
        distanta[i] = INT_MAX;
        vizitat[i] = false;
    }

    distanta[sursa] = 0;

    for (unsigned int i = 1; i <= 44 - 1; i++)
    {
        int tempDM = distantaMinima(distanta, vizitat);
        vizitat[tempDM] = true;

        for (unsigned int j = 1; j <= 44; j++)
            if (!vizitat[j] && matrice_drum[i][j] && distanta[tempDM] != INT_MAX && distanta[tempDM] + matrice_drum[i][j] < distanta[j])
                distanta[j] = distanta[tempDM] + matrice_drum[i][j];
    }
    afisareSolutieDistanta(distanta);
}

void determinareStartAprovizionare()
{
    statisticaStoc();
    dijkstra(1);
}

/*
int detDepozitStart(const int tempID_Oras)
{

}

void determinareStartTraseu()
{

}

void determinareTraseu()
{

}
*/

#endif