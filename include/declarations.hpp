#pragma once
#ifndef DECLARATIONS
#define DECLARATIONS

#include "classes/aprovizionare.hpp"
#include "classes/autentificare.hpp"
#include "classes/depozit.hpp"
#include "classes/detalii_produs.hpp"
#include "classes/oras.hpp"
#include "classes/trasee_gps.hpp"
#include "classes/traseu.hpp"
#include <vector>
#include <cstring>
#include <limits.h>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_error.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#define STRCASECMP _stricmp
#elif __linux__
#define STRCASECMP strcasecmp
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <mysql_error.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#endif

#define M_PI 3.14159265358979323846
#define MAX_SIZE 32
#define MAXL 256
#define VAL_STOC_MINIM 5
#define VAL_STOC_MAXIM 50
#define EARTH_RADIUS_KM 6371.0

using std::vector;

constexpr int N = 1001;

struct GRAF
{
    double distanta;
    int durata;
};

using GRAF_NEORIENTAT = GRAF;

extern TRASEE_GPS trasee_gps;
extern DEPOZIT depozit;
extern ORAS oras;
extern DETALII_PRODUS produs;
extern TRASEU _traseu;
extern APROVIZIONARE aprovizionare;

#ifdef _WIN32
extern HANDLE hConsole;
extern CONSOLE_SCREEN_BUFFER_INFO csbi;
extern WORD originalAttributes;
#endif

extern vector<vector<GRAF_NEORIENTAT>> matrice_drum;
extern vector<bool> depozite_centralizate;
extern vector<bool> orase_stoc_limitat;
extern vector<bool> orase_izolate;
extern vector<bool> orase_conexiune_unica;
extern vector<int> stiva;
extern vector<int> traseu_minim_TSP;

extern long long unsigned int contor_log;

extern int contor_noduri_graf, nr_componente, contor_depozite_centralizate, nr_maxim_orase_parcurse, contor_orase_stoc_limitat,
    contor_stiva, contor_traseu_TSP, pagina, contor_orase, ERROR_CNT;

extern bool trasee, traseu_completat, buffer, use_API;

extern double cost_minim_TSP, durata_minima_TSP, distanta_parcursa, durata_parcursa,
    cost_aprovizionare_total, cantitate_totala_aprovizionata;

extern int cmax_denumire_produse, cmax_denumire_orase, cmax_categorie_produse, cmax_pret_produse, cmax_ID_Oras, cmax_lat_oras,
    cmax_ID_produs, cmax_cantitate_produs;

extern char *denumire_depozit_nou;

extern int _strcasecmp_(const char *str1, const char *str2);
extern void underline(const unsigned int vWidth, const bool bSetw);
#ifdef _WIN32
extern void changeText(WORD attributes);
extern void resetText();
#endif
#ifdef __linux__
extern char _getch(void);
#endif
extern void clear_screen(void);

#endif