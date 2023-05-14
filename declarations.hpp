#ifndef DECLARATIONS
#define DECLARATIONS

#include <vector>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 32
#define MAX_LENGTH 1000000
#define VAL_STOC_MINIM 1
#define VAL_STOC_MAXIM 1000

constexpr int MAXL = 256, N = 11;
constexpr unsigned int MAXN = 1000;

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
            ID_Produs = strdup(vID_Produs);
            ID_Oras = strdup(vID_Oras);
            Cantitate_Produs = vCantitate_Produs;
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
            ID_Produs = strdup(vID_Produs);
            Denumire_Produs = strdup(vDenumire_Produs);
            Categorie_Produs = strdup(vCategorie_Produs);
            pret_produs = vPret_Produs;
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
            ID_Oras = strdup(vID_Oras);
            denumire_oras = strdup(vDenumire_Oras);
            tip_depozit = strdup(vTip_Depozit);
            latitudine = vLatitudine;
            longitudine = vLongitudine;
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

DEPOZIT depozit;
ORAS oras;
DETALII_PRODUS produs;
AUTENTIFICARE autentificare;
TRASEU _traseu;

std::vector<std::vector<double>> matrice_drum(N, std::vector<double>(N, 0.0));
std::vector<bool> depozite_centralizate(N, false);
std::vector<unsigned int> depozite_vizitate(N, 0);
std::vector<bool> orase_stoc_limitat(matrice_drum.size(), false);
std::vector<int> _verificare_orase_parcurse(matrice_drum.size(), 0);
std::vector<bool> orase_izolate(matrice_drum.size(), false);

long long unsigned int dimensiune_matrice = matrice_drum.size();
int nr_componente,  contor_depozite_centralizate , nr_maxim_orase_parcurse = -1;
unsigned int contor_orase_stoc_limitat;
bool trasee = false;

bool start();

char getch();

void clear_screen();

void sleepcp(const int ms);

void underline(const unsigned int vWidth);

bool accesareDate();

bool autentificare_cont();

bool _init_();

void afisareDateDepozit();

void afisareDateProdus();

void afisareDateOrase();

void vizualizare_status_stoc();

void creare_solutie_distanta(int start, std::vector<double> &distanta, std::vector<int> &distanta_minima, bool afisare, bool creare_trasee);

void dijkstra(int start, std::vector<double> &distanta, std::vector<int> &distanta_minima);

void sistem_aprovizionare();

void sortare_date_depozit();

void sortare_date_produs();

void sortare_date_oras();

void BFS(int start);

void cautare_produse_ID(const int ID_Depozit);

void depozite_conectate(const int ID_Depozit);

void determinare_tip_depozit();

void cautare_orase_stoc_limitat();

void cautare_orase_izolate();

void afisare_trasee_minime();

void afisare_depozite_centralizare();

#endif