#ifndef DECLARATIONS
#define DECLARATIONS

#pragma region libraries
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <limits.h>
#include <string.h>
#include <nlohmann/json.hpp>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <mysql_error.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <curl/curl.h>

#ifdef WINDOWS
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

#include "cross_platform_compatibility.hpp"
#pragma endregion

#define MAX_SIZE 32
#define MAXL 256
#define VAL_STOC_MINIM 5
#define VAL_STOC_MAXIM 50

constexpr int N = 1001;

struct HTTP_RESPONSE
{
    string body;
    long status_cod;
};

typedef struct GRAF
{
    double distanta;
    int durata;
} GRAF_NEORIENTAT;

#pragma region classes

class TRASEE_GPS
{
public:
    struct NOD_TRASEE_GPS
    {
        double distanta = 0.0;
        int durata = 0;
        char *start = nullptr;
        char *destinatie = nullptr;
        NOD_TRASEE_GPS *prev = nullptr;
        NOD_TRASEE_GPS *next = nullptr;

        NOD_TRASEE_GPS(double vDistanta, int vDurata, char *vStart, char *vDestinatie)
        {
            this->distanta = vDistanta;
            this->durata = vDurata;
            this->start = strdup(vStart);
            this->destinatie = strdup(vDestinatie);
            prev = nullptr;
            next = nullptr;
        }

        ~NOD_TRASEE_GPS()
        {
            free(start);
            free(destinatie);
        }
    };

    NOD_TRASEE_GPS *head_gps = nullptr;
    NOD_TRASEE_GPS *tail_gps = nullptr;

public:
    NOD_TRASEE_GPS *getHead()
    {
        return head_gps;
    }

    NOD_TRASEE_GPS *getTail()
    {
        return tail_gps;
    }

    void introducere_date_trasee_gps(double vDistanta, int vDurata, char *vStart, char *vDestinatie)
    {
        NOD_TRASEE_GPS *newnod = new NOD_TRASEE_GPS(vDistanta, vDurata, vStart, vDestinatie);

        if (head_gps == nullptr)
        {
            head_gps = newnod;
            tail_gps = newnod;
        }
        else
        {
            tail_gps->next = newnod;
            newnod->prev = tail_gps;
            tail_gps = newnod;
        }
    }

    ~TRASEE_GPS()
    {
        NOD_TRASEE_GPS *ptr = head_gps;

        while (ptr != nullptr)
        {
            NOD_TRASEE_GPS *temp = ptr;
            ptr = ptr->next;
            delete temp;
        }
    }
};

class AUTENTIFICARE
{
private:
    struct NOD_AUTENTIFICARE
    {
        char *host_name = nullptr;
        char *username = nullptr;
        char *parola = nullptr;

        NOD_AUTENTIFICARE(char *host_name, char *username, char *parola)
        {
            this->host_name = strdup(host_name);
            this->username = strdup(username);
            this->parola = strdup(parola);
        }

        ~NOD_AUTENTIFICARE()
        {
            free(host_name);
            free(username);
            free(parola);
        }
    };

    NOD_AUTENTIFICARE *data = nullptr;

public:
    void introducere_date(char *vHost_name, char *vUsername, char *vParola)
    {
        data = new NOD_AUTENTIFICARE(vHost_name, vUsername, vParola);
    }

    NOD_AUTENTIFICARE *get_nod()
    {
        return data;
    }

    ~AUTENTIFICARE()
    {
        delete data;
    }
};

class DEPOZIT
{
public:
    struct NOD_DEPOZIT
    {
        char *ID_Produs = nullptr;
        char *ID_Oras = nullptr;
        double Cantitate_Produs = 0.0;
        NOD_DEPOZIT *prev = nullptr;
        NOD_DEPOZIT *next = nullptr;

        NOD_DEPOZIT(char *vID_Produs, char *vID_Oras, double vCantitate_Produs)
        {
            this->ID_Produs = strdup(vID_Produs);
            this->ID_Oras = strdup(vID_Oras);
            this->Cantitate_Produs = vCantitate_Produs;
            prev = nullptr;
            next = nullptr;
        }

        ~NOD_DEPOZIT()
        {
            free(ID_Produs);
            free(ID_Oras);
        }
    };

    NOD_DEPOZIT *head_depozit = nullptr;
    NOD_DEPOZIT *tail_depozit = nullptr;

public:
    NOD_DEPOZIT *getHead()
    {
        return head_depozit;
    }

    NOD_DEPOZIT *getTail()
    {
        return tail_depozit;
    }

    void inserareDateDepozit(char *vID_Produs, char *vID_Oras, double vCantitate_Produs)
    {
        NOD_DEPOZIT *newnod = new NOD_DEPOZIT(vID_Produs, vID_Oras, vCantitate_Produs);

        if (head_depozit == nullptr)
        {
            head_depozit = newnod;
            tail_depozit = newnod;
        }
        else
        {
            tail_depozit->next = newnod;
            newnod->prev = tail_depozit;
            tail_depozit = newnod;
        }
    }

    ~DEPOZIT()
    {
        NOD_DEPOZIT *ptr = head_depozit;

        while (ptr != nullptr)
        {
            NOD_DEPOZIT *temp = ptr;
            ptr = ptr->next;
            delete temp;
        }
    }
};

class DETALII_PRODUS
{
public:
    struct NOD_DETALII_PRODUS
    {
        char *ID_Produs = nullptr;
        char *Denumire_Produs = nullptr;
        char *Categorie_Produs = nullptr;
        double pret_produs = 0.0;
        NOD_DETALII_PRODUS *next = nullptr;
        NOD_DETALII_PRODUS *prev = nullptr;

        NOD_DETALII_PRODUS(char *vID_Produs, char *vDenumire_Produs, char *vCategorie_Produs, double vPret_Produs)
        {
            this->ID_Produs = strdup(vID_Produs);
            this->Denumire_Produs = strdup(vDenumire_Produs);
            this->Categorie_Produs = strdup(vCategorie_Produs);
            this->pret_produs = vPret_Produs;
            next = nullptr;
            prev = nullptr;
        }

        ~NOD_DETALII_PRODUS()
        {
            free(ID_Produs);
            free(Denumire_Produs);
            free(Categorie_Produs);
        }
    };

    NOD_DETALII_PRODUS *head_produs = nullptr;
    NOD_DETALII_PRODUS *tail_produs = nullptr;

public:
    NOD_DETALII_PRODUS *getHead()
    {
        return head_produs;
    }

    NOD_DETALII_PRODUS *getTail()
    {
        return tail_produs;
    }

    void inserareDateProdus(char *vID_Produs, char *vDenumire_Produs, char *vCategorie_Produs, double vPret_Produs)
    {
        NOD_DETALII_PRODUS *newnod = new NOD_DETALII_PRODUS(vID_Produs, vDenumire_Produs, vCategorie_Produs, vPret_Produs);

        if (head_produs == nullptr)
        {
            head_produs = newnod;
            tail_produs = newnod;
        }
        else
        {
            tail_produs->next = newnod;
            newnod->prev = tail_produs;
            tail_produs = newnod;
        }
    }

    ~DETALII_PRODUS()
    {
        NOD_DETALII_PRODUS *ptr = head_produs;

        while (ptr != nullptr)
        {
            NOD_DETALII_PRODUS *temp = ptr;
            ptr = ptr->next;
            delete temp;
        }
    }
};

class ORAS
{
public:
    struct NOD_ORAS
    {
        char *ID_Oras = nullptr;
        char *denumire_oras = nullptr;
        char *tip_depozit = nullptr;
        double latitudine = 0.0;
        double longitudine = 0.0;
        NOD_ORAS *prev = nullptr;
        NOD_ORAS *next = nullptr;

        NOD_ORAS(char *vID_Oras, char *vDenumire_Oras, char *vTip_Depozit, double vLatitudine, double vLongitudine)
        {
            this->ID_Oras = strdup(vID_Oras);
            this->denumire_oras = strdup(vDenumire_Oras);
            this->tip_depozit = strdup(vTip_Depozit);
            this->latitudine = vLatitudine;
            this->longitudine = vLongitudine;
            next = nullptr;
            prev = nullptr;
        }

        ~NOD_ORAS()
        {
            free(ID_Oras);
            free(denumire_oras);
            free(tip_depozit);
        }
    };

    NOD_ORAS *head_oras = nullptr;
    NOD_ORAS *tail_oras = nullptr;

public:
    NOD_ORAS *getHead()
    {
        return head_oras;
    }

    NOD_ORAS *getTail()
    {
        return tail_oras;
    }

    void insearareDateOrase(char *vID_Oras, char *vDenumire_Oras, char *vTip_Depozit, double vLatitudine, double vLongitudine)
    {
        NOD_ORAS *newnod = new NOD_ORAS(vID_Oras, vDenumire_Oras, vTip_Depozit, vLatitudine, vLongitudine);

        if (head_oras == nullptr)
        {
            head_oras = newnod;
            tail_oras = newnod;
        }
        else
        {
            tail_oras->next = newnod;
            newnod->prev = tail_oras;
            tail_oras = newnod;
        }
    }

    ~ORAS()
    {
        NOD_ORAS *ptr = head_oras;

        while (ptr != nullptr)
        {
            NOD_ORAS *temp = ptr;
            ptr = ptr->next;
            delete temp;
        }
    }
};

class TRASEU
{
public:
    struct NOD_TRASEU
    {
        int start = 0, destinatie = 0;
        double distanta = 0.0;
        std::vector<int> traseu;
        NOD_TRASEU *prev = nullptr;
        NOD_TRASEU *next = nullptr;

        NOD_TRASEU(int vStart, int vDestinatie, double vDistanta, std::vector<int> &traseu)
        {
            this->start = vStart;
            this->destinatie = vDestinatie;
            this->distanta = vDistanta;
            this->traseu.assign(traseu.begin(), traseu.end());
        }

        ~NOD_TRASEU()
        {
            traseu.clear();
        }
    };

    NOD_TRASEU *head_traseu = nullptr;
    NOD_TRASEU *tail_traseu = nullptr;

public:
    NOD_TRASEU *getHead()
    {
        return head_traseu;
    }

    NOD_TRASEU *getTail()
    {
        return tail_traseu;
    }

    void inserareDateTraseu(int vStart, int vDestinatie, double vDistanta, std::vector<int> &traseu)
    {
        NOD_TRASEU *newnod = new NOD_TRASEU(vStart, vDestinatie, vDistanta, traseu);

        if (head_traseu == nullptr)
        {
            head_traseu = newnod;
            tail_traseu = newnod;
        }
        else
        {
            tail_traseu->next = newnod;
            newnod->prev = tail_traseu;
            tail_traseu = newnod;
        }
    }

    ~TRASEU()
    {
        NOD_TRASEU *ptr = head_traseu;

        while (ptr != nullptr)
        {
            NOD_TRASEU *temp = ptr;
            ptr = ptr->next;
            delete temp;
        }
    }
};

class APROVIZIONARE
{
public:
    struct NOD_APROVIZIONARE
    {
        char *ID_Produs;
        int cantitate_totala_necesara = 0;
        NOD_APROVIZIONARE *next = nullptr;
        NOD_APROVIZIONARE *prev = nullptr;

        NOD_APROVIZIONARE(char *vID_Produs, int vCantitate_totala_necesara)
        {
            this->ID_Produs = strdup(vID_Produs);
            this->cantitate_totala_necesara = vCantitate_totala_necesara;
            this->next = nullptr;
            this->prev = nullptr;
        }

        ~NOD_APROVIZIONARE()
        {
            free(ID_Produs);
        }
    };

    NOD_APROVIZIONARE *head_aprovizionare = nullptr;
    NOD_APROVIZIONARE *tail_aprovizionare = nullptr;

public:
    NOD_APROVIZIONARE *getHead()
    {
        return head_aprovizionare;
    }

    NOD_APROVIZIONARE *getTail()
    {
        return tail_aprovizionare;
    }

    void inserareDateAprovizionare(char *vID_Produs, int vCantitate_totala_necesara)
    {
        NOD_APROVIZIONARE *newnod = new NOD_APROVIZIONARE(vID_Produs, vCantitate_totala_necesara);

        if (head_aprovizionare == nullptr)
        {
            head_aprovizionare = newnod;
            tail_aprovizionare = newnod;
        }
        else
        {
            tail_aprovizionare->next = newnod;
            newnod->prev = tail_aprovizionare;
            tail_aprovizionare = newnod;
        }
    }

    ~APROVIZIONARE()
    {
        NOD_APROVIZIONARE *ptr = head_aprovizionare;

        while (ptr != nullptr)
        {
            NOD_APROVIZIONARE *temp = ptr;
            ptr = ptr->next;
            delete temp;
        }
    }
};
#pragma endregion

TRASEE_GPS trasee_gps;
DEPOZIT depozit;
ORAS oras;
DETALII_PRODUS produs;
AUTENTIFICARE autentificare;
TRASEU _traseu;
APROVIZIONARE aprovizionare;

std::vector<std::vector<GRAF_NEORIENTAT>> matrice_drum(N, std::vector<GRAF_NEORIENTAT>(N, {0.0, 0})); // matricea de adiacenta ce contine distantele dintre noduri
std::vector<bool> depozite_centralizate(N, false);                                                    // stocare ID depozite centralizate
std::vector<bool> orase_stoc_limitat(matrice_drum.size(), false);                                     // stocare ID depozite cu stoc limitat
std::vector<bool> orase_izolate(matrice_drum.size(), false);                                          // stocare ID depozite izolate
std::vector<bool> orase_conexiune_unica(matrice_drum.size(), false);                                  // stocare ID depozite o conexiune unica cu graful
std::vector<int> stiva(matrice_drum.size() * matrice_drum.size());                                    // stiva utilizata in algoritmii de backtracking pt. det. traseului optim
std::vector<int> traseu_minim_TSP(matrice_drum.size() * (matrice_drum.size() - 1) / 2);               // stocarea traseului optim TSP (travel salesman problem)

long long unsigned int contor_log; // contorizeaza log-urile dintr-o singura rulare a programului
int contor_noduri_graf;            // se foloseste acest contor, deoarece functia size() nu returneaza nr. corect

int nr_componente, contor_depozite_centralizate, nr_maxim_orase_parcurse = -1, contor_orase_stoc_limitat, contor_stiva, contor_traseu_TSP, pagina = 1,
                                                 contor_orase;
bool trasee = false, traseu_completat = false, buffer = true;
double cost_minim_TSP = INT_MAX, durata_minima_TSP = INT_MAX, distanta_parcursa, durata_parcursa, cost_aprovizionare_total, cantitate_totala_aprovizionata;

int cmax_denumire_produse, cmax_denumire_orase, cmax_categorie_produse, cmax_pret_produse;

#pragma region functions_init
void mascare_text_on(void);

void mascare_text_off(void);

string _GET_API_KEY_(const string &config_file_path);

size_t _response_data_(void *content, size_t size, size_t nmemb, string *buffer);

HTTP_RESPONSE _http_request_(const string &url);

bool _GPS_UPDATE_DATA_(void);

bool load_data(const char *path);

double calculare_distante(const char *oras_1, const char *oras_2, const double lat_1,
                          const double long_1, const double lat_2, const double long_2);

double toRadians(const double degrees);

bool start(void);

#ifdef __linux__
char getch(void);
#endif

void clear_screen(void);

void sleepcp(const int ms);

void underline(const unsigned int vWidth, const bool bSetw);

bool accesareDate(void);

bool autentificare_cont(void);

bool _init_(void);

void afisare_date_tabel_depozit(void);

void afisare_date_tabel_produs(void);

void afisare_date_tabel_oras(void);

void vizualizare_status_stoc(void);

void creare_solutie_distanta(int start, std::vector<double> &distanta, std::vector<int> &distanta_minima, bool afisare, bool creare_trasee);

void dijkstra(int start, std::vector<double> &distanta, std::vector<int> &distanta_minima);

void sistem_aprovizionare_independent(void);

void sortare_date_depozit(void);

void sortare_date_produs(const int tip_sortare);

void sortare_date_oras(const int tip_sortare);

void nr_max_caractere_den(void);

bool verificare_orase_stoc_limitat(void);

void cautare_produse_ID_stoc_limitat(const int ID_Depozit);

void depozite_conectate(const int ID_Depozit);

void determinare_tip_depozit(void);

void cautare_orase_stoc_limitat(void);

void cautare_orase_izolate(void);

void afisare_depozite_centralizare(void);

void afisare_trasee_optime(const int _ID, const int vStart);

void afisare_optiuni_trasee_optime(const int vStart);

void afisare_depozite_izolate(void);

void afisare_depozite_unic_drum(void);

void init_stiva_hc(void);

bool succesor_hc(void);

bool solutie_hc(void);

bool valid_hc(void);

void determinare_ciclu_hc_minim(void);

void back_hc(void);

void init_stiva_ac(void);

bool succesor_ac(void);

bool solutie_ac(void);

bool valid_ac(void);

void determinare_traseu_minim(void);

void back_ac(void);

void TSP(void);

void produse_transport_TSP(void);

void pagina_principala_TSP(void);

void pagina_stanga_TSP(void);

void pagina_dreapta_TSP(void);

void pagina_finala_TSP(void);

void parcurgere_traseu_TSP(void);

void afisare_detalii_SpeedyGo(sql::Connection *con);

void consola_mysql(void);

void sortare_tip_depozit(void);

void sortare_depozit_alfabetic(const int tip_sortare);

void cautare_oras_ID(void);

void sortare_categorie_produs(void);

void sortare_produs_alfabetic(const int tip_sortare);

void cautare_produs_ID(void);

void cautare_produs_denumire(void);

void free_memory(void);

bool update_database(void);

void adaugare_depozit(void);

void stergere_depozit(void);
#pragma endregion

#endif