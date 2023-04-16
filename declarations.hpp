#ifndef DECLARATIONS
#define DECLARATIONS

#include <vector>
#include <limits>

constexpr int MAXL = 256, N = 45;
constexpr unsigned int MAXN = 1000;

class NOD_PRODUS_DEPOZIT
{
public:
    char *ID_Produs;
    char *ID_Depozit;
    double Cantitate_Produs;
    NOD_PRODUS_DEPOZIT *prev_d;
    NOD_PRODUS_DEPOZIT *next_d;

public:
    NOD_PRODUS_DEPOZIT()
    {
        ID_Produs = (char *)malloc(MAXL * sizeof(char) + 1);
        ID_Depozit = (char *)malloc(MAXL * sizeof(char) + 1);
    }

    ~NOD_PRODUS_DEPOZIT()
    {
        free(ID_Produs);
        free(ID_Depozit);
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

class NOD_PRODUS_DEPOZIT_LOCAL
{
public:
    char *ID_Produs;
    char *ID_Oras;
    double cantitate_produs;
    NOD_PRODUS_DEPOZIT_LOCAL *prev_l;
    NOD_PRODUS_DEPOZIT_LOCAL *next_l;

public:
    NOD_PRODUS_DEPOZIT_LOCAL()
    {
        ID_Produs = (char *)malloc(MAXL * sizeof(char) + 1);
        ID_Oras = (char *)malloc(MAXL * sizeof(char)) + 1;
    }

    ~NOD_PRODUS_DEPOZIT_LOCAL()
    {
        free(ID_Produs);
        free(ID_Oras);
    }
};

class NOD_ORASE
{
public:
    char *ID_Oras;
    char *denumire_oras;
    double latitudine;
    double longitudine;
    NOD_ORASE *prev_o;
    NOD_ORASE *next_o;
public:
    NOD_ORASE()
    {
        ID_Oras = (char *)malloc(MAXL * sizeof(char) + 1);
        denumire_oras = (char *)malloc(MAXL * sizeof(char) + 1);
    }
    ~NOD_ORASE()
    {
        free(ID_Oras);
        free(denumire_oras);
    }
};

NOD_PRODUS_DEPOZIT *head_depozit = nullptr, *tail_depozit = nullptr;
NOD_DETALII_PRODUS *head_produs = nullptr, *tail_produs = nullptr;
NOD_PRODUS_DEPOZIT_LOCAL *head_local = nullptr, *tail_local = nullptr;
NOD_ORASE *head_oras = nullptr, *tail_oras = nullptr;

std::vector<std::vector<double>> matrice_drum(N, std::vector<double>(N, 0.0));

//double distantaCalc(double lat1, double lon1, double lat2, double lon2);

bool _init_();

void inserareDateDepozit(char *vID_Produs, char *vID_Depozit, double vCantitate_Produs);

void inserareDateProduse(char *vID_Produs, char *vDenumire_Produs, char *vCategorie_Produs, double vPret_Produs);

void inserareDateProduseLocal(char *vID_Oras, char *vID_Produs, double vCantitate_Produs_Local);

void insearareDateOrase(char *vID_Oras, char *vDenumire_Oras, double vLatitudine, double vLongitudine);

void afisareDateLocal();

void afisareDateDepozit();

void afisareDateProdus();

void statisticaStoc();

void afisareSolutieDistanta(int start, std::vector<double> &distanta, std::vector<int> &distanta_minima);

void dijkstra(int start, std::vector<double> &distanta, std::vector<int> &distanta_minima);

void determinareStartAprovizionare();

#endif