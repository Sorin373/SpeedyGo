#ifndef DECLARATIONS
#define DECLARATIONS

#include <vector>
#include <limits.h>

constexpr int MAXL = 256, N = 45;
constexpr unsigned int MAXN = 1000;

class NOD_DEPOZIT
{
public:
    char *ID_Produs;
    char *ID_Depozit;
    char *ID_Oras;

    double Cantitate_Produs;
    NOD_DEPOZIT *prev_d;
    NOD_DEPOZIT *next_d;

public:
    NOD_DEPOZIT()
    {
        ID_Produs = (char *)malloc(MAXL * sizeof(char) + 1);
        ID_Depozit = (char *)malloc(MAXL * sizeof(char) + 1);
        ID_Oras = (char *)malloc(MAXL * sizeof(char) + 1);
    }

    ~NOD_DEPOZIT()
    {
        free(ID_Produs);
        free(ID_Depozit);
        free(ID_Oras);
    }
};

class NOD_DETALII_PRODUS
{
public:
    char *ID_Produs;
    char *Denumire_Produs;
    char *Categorie_Produs;
    double pret_produs;
    NOD_DETALII_PRODUS *prev_p;
    NOD_DETALII_PRODUS *next_p;

public:
    NOD_DETALII_PRODUS()
    {
        ID_Produs = (char *)malloc(MAXL * sizeof(char) + 1);
        Denumire_Produs = (char *)malloc(MAXL * sizeof(char) + 1);
        Categorie_Produs = (char *)malloc(MAXL * sizeof(char) + 1);
    }

    ~NOD_DETALII_PRODUS()
    {
        free(ID_Produs);
        free(Denumire_Produs);
        free(Categorie_Produs);
    }
};

class NOD_ORASE
{
public:
    char *ID_Oras;
    char *denumire_oras;
    char *tip_depozit;
    double latitudine;
    double longitudine;
    NOD_ORASE *prev_o;
    NOD_ORASE *next_o;
public:
    NOD_ORASE()
    {
        ID_Oras = (char *)malloc(MAXL * sizeof(char) + 1);
        denumire_oras = (char *)malloc(MAXL * sizeof(char) + 1);
        tip_depozit = (char *)malloc(MAXL * sizeof(char) + 1);
    }
    ~NOD_ORASE()
    {
        free(ID_Oras);
        free(denumire_oras);
        free(tip_depozit);
    }
};

NOD_DEPOZIT *head_depozit = nullptr, *tail_depozit = nullptr;
NOD_DETALII_PRODUS *head_produs = nullptr, *tail_produs = nullptr;
NOD_ORASE *head_oras = nullptr, *tail_oras = nullptr;

std::vector<std::vector<double>> matrice_drum(N, std::vector<double>(N, 0.0));
std::vector<bool> depozite_centralizate(N, false);
unsigned int contor_depozite_centralizate = 1;

bool _init_();

void inserareDateDepozit(char *vID_Produs, char *vID_Depozit, char *vID_Oras, double vCantitate_Produs);

void inserareDateProduse(char *vID_Produs, char *vDenumire_Produs, char *vCategorie_Produs, double vPret_Produs);

void insearareDateOrase(char *vID_Oras, char *vDenumire_Oras, char *vTip_Depozit, double vLatitudine, double vLongitudine);

void afisareDateDepozit();

void afisareDateProdus();

void afisareDateOrase();

void statisticaStoc();

void afisareSolutieDistanta(int start, std::vector<double> &distanta, std::vector<int> &distanta_minima);

void dijkstra(int start, std::vector<double> &distanta, std::vector<int> &distanta_minima);

void determinareStartAprovizionare();

#endif