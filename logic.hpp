#ifndef LOGIC
#define LOGIC

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <limits.h>
#include <string.h>
#include "declarations.hpp"
#include <nlohmann/json.hpp>

using namespace std;
using namespace nlohmann;

bool autentificare_cont()
{
    clear_screen();

    char *_HN = (char *)malloc(MAXL * sizeof(char) + 1),
         *_UN = (char *)malloc(MAXL * sizeof(char) + 1),
         *_P = (char *)malloc(MAXL * sizeof(char) + 1);

    cout << "\n\n"
         << setw(10) << " "
         << "CONECTARE BAZA DE DATE\n";
    underline(40);

    cout << setw(4) << " "
         << "SERVER NAME: ";
    cin >> _HN;
    cout << setw(4) << " "
         << "USERNAME: ";
    cin >> _UN;
    cout << setw(4) << " "
         << "PASSWORD: ";
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
    free(_P);
    ;

    return EXIT_SUCCESS;
}

bool _init_()
{
    ifstream file_json("distante_orase.json");
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
            return EXIT_FAILURE;
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

void afisare_date_tabel_oras()
{
    clear_screen();

    cout << "\n\n";
    cout << setw(5) << " "
         << "┌──────────────┐\n";
    cout << setw(6) << " "
         << "TABEL-DEPOZIT\n";
    cout << setw(5) << " "
         << "└──────────────┘\n\n";

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
    underline(80);

    int cmax = -1;
    for (ORAS::NOD_ORAS *date_oras = oras.getHead(); date_oras != nullptr; date_oras = date_oras->next)
    {
        if (strlen(date_oras->denumire_oras) > cmax)
            cmax = strlen(date_oras->denumire_oras);
    }

    for (ORAS::NOD_ORAS *date_oras = oras.getHead(); date_oras != nullptr; date_oras = date_oras->next)
    {
        cout << setw(7) << " " << date_oras->ID_Oras << setw(10) << " " << date_oras->denumire_oras
             << setw(cmax - strlen(date_oras->denumire_oras) + 20) << " " << date_oras->tip_depozit
             << setw(11 - strlen(date_oras->tip_depozit) + 4) << " " << date_oras->latitudine << "\u00B0" << setw(8)
             << " " << date_oras->longitudine << "\u00B0\n";
    }

    underline(80);
}

void afisare_date_tabel_depozit()
{
    DEPOZIT::NOD_DEPOZIT *ptr = depozit.getHead();
    while (ptr != nullptr)
    {
        if (ptr->Cantitate_Produs == 0)
        {
            cout << "ID_Produs: " << ptr->ID_Produs << ", ";
            cout << "Cantitate_Produs: " << ptr->Cantitate_Produs << ", ";
            cout << "ID_oras: " << ptr->ID_Oras << endl;
        }
        ptr = ptr->next;
    }
}

void afisare_date_tabel_produs()
{
    clear_screen();

    cout << "\n\n";
    cout << setw(5) << " "
         << "┌──────────────┐\n";
    cout << setw(6) << " "
         << "TABEL-PRODUS\n";
    cout << setw(5) << " "
         << "└──────────────┘\n\n";

    cout << setw(5) << " "
         << "ID_Produs"
         << setw(10) << " "
         << "Denumire_Produs"
         << setw(35) << " "
         << "Categorie_Produs"
         << setw(13) << " "
         << "Pret_Produs\n";

    underline(115);

    int cmax_dp = 0, cmax_cp = 0;

    for (DETALII_PRODUS::NOD_DETALII_PRODUS *date_produs = produs.getHead(); date_produs != nullptr; date_produs = date_produs->next)
    {
        if (strlen(date_produs->Denumire_Produs) > cmax_dp)
            cmax_dp = strlen(date_produs->Denumire_Produs);
        if (strlen(date_produs->Categorie_Produs) > cmax_cp)
            cmax_cp = strlen(date_produs->Categorie_Produs);
    }

    for (DETALII_PRODUS::NOD_DETALII_PRODUS *date_produs = produs.getHead(); date_produs != nullptr; date_produs = date_produs->next)
    {
        cout << setw(7) << " " << date_produs->ID_Produs << setw(15) << " " << date_produs->Denumire_Produs
             << setw(cmax_dp - strlen(date_produs->Denumire_Produs) + 10) << " " << date_produs->Categorie_Produs
             << setw(cmax_cp - strlen(date_produs->Categorie_Produs) + 5) << " " << date_produs->pret_produs << "\n";
    }

    underline(115);
}

void cautareDepozit()
{
    char *ID = (char *)malloc(MAXL * sizeof(char));
    DEPOZIT::NOD_DEPOZIT *ptr = depozit.getHead();

    cin >> ID;
    vizualizare_status_stoc();
    cout << endl;
    while (ptr != nullptr)
    {
        if (strcasecmp(ID, ptr->ID_Oras) == 0)
        {
            cout << "ID_Produs: " << ptr->ID_Produs << ", ";
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

void cautare_produse_ID(const int ID_Depozit)
{
    clear_screen();

    DETALII_PRODUS::NOD_DETALII_PRODUS *date_produs = produs.getHead();
    int cmax = 0;
    while (date_produs != nullptr)
    {
        if (strlen(date_produs->Denumire_Produs) > cmax)
            cmax = strlen(date_produs->Denumire_Produs);
        date_produs = date_produs->next;
    }

    cout << "\n\n"
         << setw(5) << " "
         << "ID_Produs" << setw(5) << " "
         << "Denumire_Produs" << setw(10) << " "
         << "Nr.Produse\n";
    underline(55);

    DEPOZIT::NOD_DEPOZIT *date_depozit = depozit.getHead();
    while (date_depozit != nullptr)
    {
        int tID_Depozit = stoi(date_depozit->ID_Oras);
        if (tID_Depozit == ID_Depozit)
            if (date_depozit->Cantitate_Produs < 100)
            {
                date_produs = produs.getHead();
                int tID_Produs = stoi(date_depozit->ID_Produs);
                while (date_produs != nullptr)
                {
                    int tsID_Produs = stoi(date_produs->ID_Produs);
                    if (tsID_Produs == tID_Produs)
                        cout << setw(7) << " [" << date_produs->ID_Produs << "]" << setw(8) << " " << date_produs->Denumire_Produs
                             << setw(cmax - strlen(date_produs->Denumire_Produs) - 11) << " ";
                    date_produs = date_produs->next;
                }
                cout << date_depozit->Cantitate_Produs << "\n";
            }
        date_depozit = date_depozit->next;
    }
    underline(55);
}

void depozite_conectate(int ID_Depozit)
{
    cout << "\n";
    vector<bool> temp_depozite(N, 100);
    ORAS::NOD_ORAS *date_oras = oras.getHead();
    char *t_denumire = (char *)malloc(MAXL * sizeof(char) + 1);
    int cmax = 0;

    while (date_oras != nullptr)
    {
        int _ID = stoi(date_oras->ID_Oras);
        if (_ID == ID_Depozit)
            strcpy(t_denumire, date_oras->denumire_oras);
        if (strlen(date_oras->denumire_oras) > cmax)
            cmax = strlen(date_oras->denumire_oras);
        date_oras = date_oras->next;
    }

    int contor = 0;
    for (unsigned int i = 0; i < matrice_drum.size(); i++)
        if (matrice_drum[ID_Depozit][i] == 1)
            temp_depozite[i] = true;

    for (unsigned int i = 0; i < matrice_drum.size(); i++)
        if (temp_depozite[i] == true)
        {
            date_oras = oras.getHead();
            while (date_oras != nullptr)
            {
                int _ID = stoi(date_oras->ID_Oras);
                if (_ID == i && strcasecmp(t_denumire, date_oras->denumire_oras) != 0)
                {
                    cout << setw(5) << " " << t_denumire << " -> " << date_oras->denumire_oras << setw(cmax - strlen(date_oras->denumire_oras) + 5)
                         << " " << matrice_drum[ID_Depozit][i] << "km\n";
                    break;
                }
                date_oras = date_oras->next;
            }
        }
}

void determinare_tip_depozit()
{
    ORAS::NOD_ORAS *date_oras = oras.getHead();
    while (date_oras != nullptr)
    {
        if (strcasecmp(date_oras->tip_depozit, "centralizat") == 0)
        {
            int ID = stoi(date_oras->ID_Oras);
            depozite_centralizate[ID] = true;
        }
        date_oras = date_oras->next;
    }
}

void cautare_orase_stoc_limitat()
{
    ORAS::NOD_ORAS *date_oras = oras.getHead();
    while (date_oras != nullptr)
    {
        int _ID_Oras = stoi(date_oras->ID_Oras);

        DEPOZIT::NOD_DEPOZIT *date_depozit = depozit.getHead();
        while (date_depozit != nullptr)
        {
            int _ID_Depozit = stoi(date_depozit->ID_Oras);
            if (_ID_Depozit == _ID_Oras && depozite_centralizate[_ID_Depozit] == false)
                if (date_depozit->Cantitate_Produs < VAL_STOC_MINIM)
                {
                    orase_stoc_limitat[_ID_Depozit] = true;
                    contor_orase_stoc_limitat++;
                    break;
                }
            date_depozit = date_depozit->next;
        }
        date_oras = date_oras->next;
    }
}

void cautare_orase_izolate()
{
    for (unsigned int i = 0; i < matrice_drum.size(); i++)
    {
        bool izolat = true;
        for (unsigned int j = 0; j < matrice_drum.size() && izolat; j++)
            if (matrice_drum[i][j] > 0)
                izolat = false;
        if (izolat)
            orase_izolate[i] = true;
    }
}

void vizualizare_status_stoc()
{
    clear_screen();

    cout << "\n\n"
         << setw(5) << " "
         << "Orase cu stocuri insuficiente:\n";
    underline(100);

    DEPOZIT::NOD_DEPOZIT *date_depozit = depozit.getHead();
    while (date_depozit != nullptr)
    {
        if (date_depozit->Cantitate_Produs < VAL_STOC_MINIM)
        {
            int _ID = stoi(date_depozit->ID_Oras);
            if (depozite_centralizate[_ID] == false)
                matrice_drum[_ID][_ID] = 1;
        }
        date_depozit = date_depozit->next;
    }

    ORAS::NOD_ORAS *date_oras = oras.getHead();
    int cmax = 0, contor_linii = 0;

    while (date_oras != nullptr)
    {
        if (strlen(date_oras->denumire_oras) > cmax)
            cmax = strlen(date_oras->denumire_oras);
        date_oras = date_oras->next;
    }

    for (unsigned int i = 0; i < matrice_drum.size(); i++)
        if (matrice_drum[i][i] == true)
        {
            date_oras = oras.getHead();
            while (date_oras != nullptr)
            {
                int _ID = stoi(date_oras->ID_Oras);
                if (_ID == i)
                {
                    cout << setw(5) << " [" << date_oras->ID_Oras << "] " << date_oras->denumire_oras
                         << setw(cmax - strlen(date_oras->denumire_oras) + 5) << " ";
                    contor_linii++;
                    if (contor_linii % 3 == 0)
                        cout << "\n";
                }
                date_oras = date_oras->next;
            }
        }

    cout << "\n";
    underline(100);

    char *t_ID_Oras = (char *)malloc(MAXL * sizeof(char) + 1);
    cout << setw(5) << " "
         << "Introduceti ID-ul orasului: ";
    cin >> t_ID_Oras;

    if (strcasecmp(t_ID_Oras, "exit") == 0)
        return;
    else
    {
        int _ID_Oras = stoi(t_ID_Oras);
        date_oras = oras.getHead();
        while (date_oras != nullptr)
        {
            int t_ID = stoi(date_oras->ID_Oras);
            if (t_ID == _ID_Oras && orase_stoc_limitat[t_ID] == true)
            {
                unsigned int sMENIU;

                do
                {
                    clear_screen();

                    cout << "\n\n"
                         << setw(5 + 1) << " [" << date_oras->ID_Oras << "] " << date_oras->denumire_oras
                         << " | Tip depozit: " << date_oras->tip_depozit << "\n";

                    underline(50);

                    cout << setw(5) << " "
                         << "[1] Vizualizare produse cu stoc limitat\n"
                         << setw(5) << " "
                         << "[2] Vizualizare conexiuni cu alte depozite\n"
                         << setw(5) << " "
                         << "[0] Inapoi\n";

                    underline(50);

                    cout << setw(5) << " "
                         << "Introduceti numarul meniului: ";
                    cin >> sMENIU;

                    switch (sMENIU)
                    {
                    case 1:
                        cautare_produse_ID(_ID_Oras);
                        getch();
                        break;
                    case 2:
                        depozite_conectate(_ID_Oras);
                        getch();
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

void creare_solutie_distanta(int start, vector<double> &distanta, vector<int> &distanta_minima, bool afisare, bool creare_trasee)
{
    int contor = 0;
    vector<bool> temp(dimensiune_matrice, false);
    temp.assign(orase_stoc_limitat.begin(), orase_stoc_limitat.end());

    for (unsigned int i = 0; i < dimensiune_matrice; i++)
    {
        contor = 0;
        if (i != start)
        {
            if (afisare)
                cout << "Shortest distance from " << start << " to " << i << " is " << distanta[i] << ". traseu: ";
            vector<int> traseu;
            int nod = i;

            while (nod != -1)
            {
                traseu.push_back(nod);
                nod = distanta_minima[nod];
            }

            int dimensiune_vector_traseu = traseu.size();

            reverse(traseu.begin(), traseu.end());

            for (unsigned int j = 0; j < dimensiune_vector_traseu; j++)
            {
                if (orase_stoc_limitat[traseu[j]])
                {
                    contor++;
                    temp[traseu[j]] = false;
                }
            }

            if (creare_trasee)
                _traseu.inserareDateTraseu(start, i, distanta[i], traseu);

            if (afisare)
                for (unsigned int j = 0; j < traseu.size(); j++)
                    cout << traseu[j] << " ";

            if (contor > nr_maxim_orase_parcurse)
            {
                nr_maxim_orase_parcurse = contor;
                _verificare_orase_parcurse.assign(traseu.begin(), traseu.end());
            }

            if (afisare)
                cout << "\n";
        }
    }
}

void dijkstra(int start, vector<double> &distanta, vector<int> &distanta_minima)
{
    vector<bool> visited(dimensiune_matrice, false);
    distanta[start] = 0.0;

    for (unsigned int i = 0; i < dimensiune_matrice; i++)
    {
        int min_index = 0;
        double min_dist = numeric_limits<double>::infinity();

        for (unsigned int j = 0; j < dimensiune_matrice; j++)
            if (!visited[j] && distanta[j] < min_dist)
            {
                min_index = j;
                min_dist = distanta[j];
            }

        visited[min_index] = true;

        for (unsigned int j = 0; j < dimensiune_matrice; j++)
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

void afisare_depozite_centralizare()
{
    cout << "\n"
         << setw(5) << " "
         << "Depozite centralizate\n";
    underline(40);

    for (unsigned int i = 0; i < matrice_drum.size(); i++)
    {
        if (depozite_centralizate[i])
            for (ORAS::NOD_ORAS *date_oras = oras.getHead(); date_oras != nullptr; date_oras = date_oras->next)
            {
                int ID = stoi(date_oras->ID_Oras);
                if (ID == i)
                {
                    cout << setw(5 + 1) << " [" << ID << "] " << date_oras->denumire_oras << endl;
                    break;
                }
            }
    }

    underline(40);
}

void afisare_trasee_optime(const int _ID)
{
    bool gasit = false;

    if (orase_stoc_limitat[_ID] == false)
    {
        cout << "\n"
             << setw(5) << " "
             << "Nu exista traseul cu acest ID!";
        return;
    }

    for (TRASEU::NOD_TRASEU *date_traseu = _traseu.getHead(); date_traseu != nullptr; date_traseu = date_traseu->next)
    {
        if (date_traseu->destinatie == _ID && orase_izolate[_ID] == false)
        {
            gasit = true;
            cout << "\n"
                 << setw(5) << " "
                 << "Distanta: ";
            cout << date_traseu->distanta << "km\n"
                 << setw(5) << " ";

            for (unsigned int i = 0; i < date_traseu->traseu.size(); i++)
                for (ORAS::NOD_ORAS *date_oras = oras.getHead(); date_oras != nullptr; date_oras = date_oras->next)
                {
                    int ID = stoi(date_oras->ID_Oras);
                        
                    if (ID == date_traseu->traseu[i])
                    {
                        cout << date_oras->denumire_oras;
                        // corectare afisare '->'
                        if (i != date_traseu->traseu.size() - 1)
                            cout << " -> ";
                        break;
                    }
                }
            break;
        }
        else
        {
            cout << "\n" << setw(5) << " " << "Depozit izolat!";
            return;
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

    underline(45);

    char *oras_start = (char *)malloc(MAXL * sizeof(char) + 1);

    for (ORAS::NOD_ORAS *date_oras = oras.getHead(); date_oras != nullptr; date_oras = date_oras->next)
    {
        int ID = stoi(date_oras->ID_Oras);
        if (ID == vStart)
        {
            strcpy(oras_start, date_oras->denumire_oras);
            break;
        }
    }

    for (TRASEU::NOD_TRASEU *date_traseu = _traseu.getHead(); date_traseu != nullptr; date_traseu = date_traseu->next)
    {
        if (date_traseu->start == vStart)
        {
            for (ORAS::NOD_ORAS *date_oras = oras.getHead(); date_oras != nullptr; date_oras = date_oras->next)
            {
                int ID = stoi(date_oras->ID_Oras);
                if (ID == date_traseu->destinatie && orase_stoc_limitat[date_traseu->destinatie])
                {
                    cout << setw(5 + 1) << " [" << date_oras->ID_Oras << "] ";
                    cout << oras_start << " -> " << date_oras->denumire_oras << endl;
                    break;
                }
            }
        }
    }

    underline(45);

    char *_ID = (char *)malloc(MAXL * sizeof(char) + 1);
    cout << setw(5) << " "
         << "Introduceti ID-ul corespunzator: ";
    cin >> _ID;
    if (strcasecmp(_ID, "exit") == 0)
        return;
    else
    {
        int _ID_temp = stoi(_ID);
        afisare_trasee_optime(_ID_temp);
        getch();
        afisare_optiuni_trasee_optime(vStart);
    }
}

void sistem_aprovizionare_independent()
{
    if (!trasee)
    {
        for (unsigned int i = 0; i < matrice_drum.size(); i++)
        {
            vector<int> distanta_minima(N, -1);
            vector<double> distanta(N, numeric_limits<double>::infinity());

            if (depozite_centralizate[i])
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
    if (strcasecmp(_ID, "exit") == 0)
        return;
    else
    {
        int _ID_temp = stoi(_ID);
        afisare_optiuni_trasee_optime(_ID_temp);
        sistem_aprovizionare_independent();
    }
}

void BFS(int start)
{
    vector<unsigned int> v(N, 0);
    v[0] = start;
    int pi = 1, ps = 1;
    depozite_vizitate[start] = nr_componente;

    while (ps <= pi)
    {
        start = v[ps];
        for (unsigned int i = 0; i < matrice_drum.size(); i++)
        {
            if (matrice_drum[start][i] != 0 && depozite_vizitate[i] == 0)
            {
                pi++;
                v[pi] = i;
                depozite_vizitate[i] = nr_componente;
            }
        }
        ps++;
    }
}

void afisare_depozite_izolate()
{
    clear_screen();

    cout << "\n\n";
    cout << setw(5) << " "
         << "┌────────────────┐\n";
    cout << setw(6) << " "
         << "DEPOZITE IZOLATE\n";
    cout << setw(5) << " "
         << "└────────────────┘\n\n";

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

    underline(75);

    int cmax = -1;
    for (ORAS::NOD_ORAS *date_oras = oras.getHead(); date_oras != nullptr; date_oras = date_oras->next)
    {
        if (strlen(date_oras->denumire_oras) > cmax)
            cmax = strlen(date_oras->denumire_oras);
    }

    bool gasit = false;
    for (unsigned int i = 0; i < orase_izolate.size(); i++)
        if (orase_izolate[i] == true)
        {
            gasit = true;
            for (ORAS::NOD_ORAS *date_oras = oras.getHead(); date_oras != nullptr; date_oras = date_oras->next)
            {
                int ID = stoi(date_oras->ID_Oras);
                if (ID == i)
                {
                    cout << setw(7) << " " << date_oras->ID_Oras << setw(12) << " " << date_oras->denumire_oras
                         << setw(cmax - strlen(date_oras->denumire_oras) + 18) << " " << date_oras->tip_depozit
                         << setw(11 - strlen(date_oras->tip_depozit) + 4) << " " << date_oras->latitudine << "\u00B0" << setw(8)
                         << " " << date_oras->longitudine << "\u00B0\n";
                    break;
                }
            }
        }

    underline(75);

    if (!gasit)
    {
        cout << setw(5) << " "
             << "Nu exista depozite izolate!";
        return;
    }
}

void afisare_depozite_unic_drum()
{
    clear_screen();

    cout << "\n\n";
    cout << setw(5) << " "
         << "┌─────────────────────────────┐\n";
    cout << setw(6) << " "
         << "DEPOZITE CU O UNICA CONEXIUNE\n";
    cout << setw(5) << " "
         << "└─────────────────────────────┘\n\n";

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

    underline(75);

    bool gasit = false;
    for (unsigned int i = 0; i < matrice_drum.size(); i++)
    {
        int contor = 0;
        for (unsigned int j = 0; j < matrice_drum.size(); j++)
            if (matrice_drum[i][j] != 0)
            {
                contor++;
                if (contor >= 2)
                    break;
            }
        if (contor == 1)
        {
            gasit = true;
            orase_conexiune_unica[i] = true;
        }
    }

    if (gasit)
    {
        int cmax = -1;
        for (ORAS::NOD_ORAS *date_oras = oras.getHead(); date_oras != nullptr; date_oras = date_oras->next)
        {
            if (strlen(date_oras->denumire_oras) > cmax)
                cmax = strlen(date_oras->denumire_oras);
        }

        for (unsigned int i = 0; i < orase_conexiune_unica.size(); i++)
        {
            if (orase_conexiune_unica[i])
            {
                for (ORAS::NOD_ORAS *date_oras = oras.getHead(); date_oras != nullptr; date_oras = date_oras->next)
                {
                    int ID = stoi(date_oras->ID_Oras);
                    if (ID == i)
                    {
                        cout << setw(7) << " " << date_oras->ID_Oras << setw(10) << " " << date_oras->denumire_oras
                             << setw(cmax - strlen(date_oras->denumire_oras) + 20) << " " << date_oras->tip_depozit
                             << setw(11 - strlen(date_oras->tip_depozit) + 4) << " " << date_oras->latitudine << "\u00B0" << setw(8)
                             << " " << date_oras->longitudine << "\u00B0\n";
                        break;
                    }
                }
            }
        }
        underline(75);
    }
    else
    {
        cout << setw(5) << " "
             << "Nu exista depozite cu o unica conexiune!";
        return;
    }
}

#endif