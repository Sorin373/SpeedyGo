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

#define MAX_SIZE 32

using namespace std;
using namespace nlohmann;

bool autentificare_cont()
{
    clear_screen();

    char *_HN = (char *)malloc(MAXL * sizeof(char) + 1),
         *_UN = (char *)malloc(MAXL * sizeof(char) + 1),
         *_P = (char *)malloc(MAXL * sizeof(char) + 1);
         
    cout << "\n\n" << setw(10) << " " << "CONECTARE BAZA DE DATE\n";
    underline(40);

    cout << setw(4) << " " << "SERVER NAME: ";
    cin >> _HN;
    cout << setw(4) << " " << "USERNAME: ";
    cin >> _UN;
    cout << setw(4) << " " << "PASSWORD: ";
    cin >> _P;

    if (strlen(_HN) > MAX_SIZE || strlen(_UN) > MAX_SIZE || strlen(_P) > MAX_SIZE)
        return EXIT_FAILURE;

    autentificare.introducere_date(_HN, _UN, _P);

    clear_screen();

    if (accesareDate() == EXIT_FAILURE)
    {
        getch();
        autentificare_cont();
    }
        
    free(_HN);
    free(_UN);
    free(_P);;

    return EXIT_SUCCESS;
}

bool _init_()
{
    ifstream file_json("distance_matrix.json");
    if (!file_json.is_open())
    {
        cerr << "Eroare\n";
        return EXIT_FAILURE;
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
            return EXIT_SUCCESS;
        }
        
        for (json::iterator i = data.begin(); i != data.end(); i++)
        {
            char *pereche_orase = (char *)malloc(MAXL * sizeof(char) + 1);
            strcpy(pereche_orase, i.key().c_str());
            char *ptr = strtok(pereche_orase, "_");
            char *oras1 = (char *)malloc(MAXL * sizeof(char) + 1);
            char *oras2 = (char *)malloc(MAXL * sizeof(char) + 1);
            int ID_Oras1 = 0, ID_Oras2 = 0;
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
        return EXIT_SUCCESS;
    }
}

void afisareDateOrase()
{
    cout << "ID_Oras" << " " << "Denumire_Oras" << " " << "Tip_Depozit\n";
    underline(70);

    ORAS::NOD_ORAS *ptr = oras.getHead();
    while (ptr != nullptr)
    {
        cout << ptr->ID_Oras << " " << ptr->denumire_oras << " " << ptr->tip_depozit << "\n";
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

void sortare_date_depozit()
{
    bool vsort = true;

    DEPOZIT::NOD_DEPOZIT *l_date_depozit = nullptr;
    if (depozit.getHead() == nullptr)
        return;
    do
    {
        vsort = true;
        DEPOZIT::NOD_DEPOZIT *ptr = depozit.getHead();
        while (ptr->next != nullptr)
        {
            int _ID1 = stoi(ptr->ID_Produs), _ID2 = stoi(ptr->next->ID_Produs);
            if (_ID1 > _ID2)
            {
                swap(ptr->ID_Produs, ptr->next->ID_Produs);
                swap(ptr->ID_Depozit, ptr->next->ID_Depozit);
                swap(ptr->ID_Oras, ptr->next->ID_Oras);
                swap(ptr->Cantitate_Produs, ptr->next->Cantitate_Produs);
                vsort = false;
            }
            ptr = ptr->next;
        }
        l_date_depozit = ptr;
    } while (!vsort);
}

void sortare_date_produs()
{
    bool vsort = true;

    DETALII_PRODUS::NOD_DETALII_PRODUS *l_date_produs = nullptr;
    if (produs.getHead() == nullptr)
        return;
    do
    {
        vsort = true;
        DETALII_PRODUS::NOD_DETALII_PRODUS *ptr = produs.getHead();
        while (ptr->next != nullptr)
        {
            int _ID1 = stoi(ptr->ID_Produs), _ID2 = stoi(ptr->next->ID_Produs);
            if (_ID1 > _ID2)
            {
                swap(ptr->ID_Produs, ptr->next->ID_Produs);
                swap(ptr->Categorie_Produs, ptr->next->Categorie_Produs);
                swap(ptr->Denumire_Produs, ptr->next->Denumire_Produs);
                swap(ptr->pret_produs, ptr->next->pret_produs);
                vsort = false;
            }
            ptr = ptr->next;
        }
        l_date_produs = ptr;
    } while (!vsort);
}

void sortare_date_oras()
{
    bool vsort = true;

    ORAS::NOD_ORAS *l_date_oras = nullptr;
    if (oras.getHead() == nullptr)
        return;
    do
    {
        vsort = true;
        ORAS::NOD_ORAS *ptr = oras.getHead();
        while (ptr->next != nullptr)
        {
            int _ID1 = stoi(ptr->ID_Oras), _ID2 = stoi(ptr->next->ID_Oras);
            if (_ID1 > _ID2)
            {
                swap(ptr->ID_Oras, ptr->next->ID_Oras);
                swap(ptr->denumire_oras, ptr->next->denumire_oras);
                swap(ptr->latitudine, ptr->next->latitudine);
                swap(ptr->longitudine, ptr->next->longitudine);
                vsort = false;
            }
            ptr = ptr->next;
        }
        l_date_oras = ptr;
    } while (!vsort);
}

void statisticaStoc()
{
    clear_screen();
    DEPOZIT::NOD_DEPOZIT *date_depozit = depozit.getHead();
    while (date_depozit != nullptr)
    {
        if (date_depozit->Cantitate_Produs < 50)
        {
            int tempIdOras = stoi(date_depozit->ID_Oras);
            if (tempIdOras > 5)
                matrice_drum[tempIdOras][tempIdOras] = 1;
        }
        date_depozit = date_depozit->next;
    }

    for (unsigned int i = 1; i <= matrice_drum.size() - 1; i++)
        if (matrice_drum[i][i] == true)
        {
            date_depozit = depozit.getHead();
            while (date_depozit != nullptr)
            {
                string c = to_string(i);

                if (strcasecmp(c.c_str(), date_depozit->ID_Oras) == 0 && date_depozit->Cantitate_Produs < 50)
                {
                    cout << date_depozit->ID_Depozit << " ";

                    ORAS::NOD_ORAS *date_oras = oras.getHead();

                    while (date_oras != nullptr)
                    {
                        if (strcasecmp(date_depozit->ID_Depozit, date_oras->ID_Oras) == 0)
                        {
                            cout << date_oras->denumire_oras << " ";
                            break;
                        }
                        date_oras = date_oras->next;
                    }
                    
                    cout << date_depozit->ID_Produs << " ";
                    cout << date_depozit->Cantitate_Produs << endl;
                }
                date_depozit = date_depozit->next;
            }
        }
    getch();
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

/*
void vizualizare_date()
{
    unsigned int MENIU;

    do
    {
        clear_screen();
        afisareDateOrase();
        underline(70);

        cout << "Selecteaza un oras: ";
        cin >> MENIU;

        unsigned int sMENIU;

        do 
        {
            clear_screen();
            ORAS::NOD_ORAS *date_oras = oras.getHead();
            while (date_oras != nullptr)
            {
                int ID = stoi(date_oras->ID_Oras);
                if (ID == MENIU)
                {
                    cout << date_oras->ID_Oras << " " << date_oras->denumire_oras << " " << date_oras->tip_depozit << "\n";
                    break;
                }
                date_oras = date_oras->next;
            }

            DEPOZIT::NOD_DEPOZIT *date_depozit = depozit.getHead();
            while (date_depozit != nullptr)
            {
                int ID = stoi(date_depozit->ID_Oras);
                if (ID == MENIU)
                {
                    cout << date_depozit->ID_Produs << " ";
                    DETALII_PRODUS::NOD_DETALII_PRODUS *date_produs = produs.getHead();
                    while (date_produs != nullptr)
                    {
                        if (strcasecmp(date_depozit->ID_Produs, date_produs->ID_Produs) == 0)
                        {
                            cout << date_produs->Denumire_Produs << " " << date_produs->Categorie_Produs << " " 
                                 << date_produs->pret_produs << " ";
                            break;
                        }
                        date_produs = date_produs->next;
                    }
                    cout << date_depozit->Cantitate_Produs << "\n";
                }
                date_depozit = date_depozit->next;
            }

            cin >> sMENIU;
            switch (sMENIU)
            {
                case 1:
                    break;
                case 2:
                    break;
            }
        } while (sMENIU != 0);

    } while (MENIU != 0);
}*/

#endif