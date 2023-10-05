#include "../include/declarations.hpp"

#include <iostream>
#include <iomanip>
#include <string>

ADJACENCY_MATRIX_INITIALIZER adjacency_matrix_init;
DEPOT depot;
CITY city;
PRODUCT product;
ROUTE route;
SUPPLY supply;

#ifdef _WIN32
HANDLE hConsole;
CONSOLE_SCREEN_BUFFER_INFO csbi;
WORD originalAttributes;
#endif

std::vector<std::vector<GRAF_NEORIENTAT>> adjacencyMatrix(N, std::vector<GRAF_NEORIENTAT>(N, {0.0, 0})); // matricea de adiacenta ce contine distantele dintre noduri
std::vector<bool> centralDepos(N, false);                                                                // stocare ID depozite centralizate
std::vector<bool> limitedStockCities(adjacencyMatrix.size(), false);                                     // stocare ID depozite cu stoc limitat
std::vector<bool> isolatedVertex(adjacencyMatrix.size(), false);                                         // stocare ID depozite izolate
std::vector<bool> oneEdgeVertex(adjacencyMatrix.size(), false);                                          // stocare ID depozite o conexiune unica cu graful
std::vector<int> stack(adjacencyMatrix.size() * adjacencyMatrix.size());                                 // stiva utilizata in algoritmii de backtracking pt. det. traseului optim
std::vector<int> minimumRouteTSP(adjacencyMatrix.size() * (adjacencyMatrix.size() - 1) / 2);             // stocarea traseului optim TSP (travel salesman problem)

long long unsigned int logCounter;

int VERTEX_COUNT, nr_componente, contor_depozite_centralizate, nr_maxim_orase_parcurse = -1, contor_orase_stoc_limitat,
                                                                     contor_stiva, contor_traseu_TSP, pagina = 1, contor_orase, ERROR_CNT;

bool trasee = false, traseu_completat = false, buffer = true, use_API = false;
double cost_minim_TSP = INT_MAX, durata_minima_TSP = INT_MAX, distanta_parcursa, durata_parcursa,
       cost_aprovizionare_total, cantitate_totala_aprovizionata;

int cmax_denumire_produse, cmax_denumire_orase, cmax_categorie_produse, cmax_pret_produse, cmax_ID_Oras, cmax_lat_oras,
    cmax_ID_produs, cmax_cantitate_produs;

char *denumire_depozit_nou = (char *)malloc(MAXL * sizeof(char) + 1);

int _strcasecmp_(const char *str1, const char *str2)
{
    return STRCASECMP(str1, str2);
}

void underline(const unsigned int vWidth, const bool bSetw)
{
    if (bSetw)
        std::cout << std::setw(5 - 2) << " ";
    char fillLine;
    fillLine = std::cout.fill('_');
    std::cout.width(vWidth);
    std::cout << '_' << "\n";
    std::cout.fill(fillLine);
    std::cout << "\n";
}

#ifdef _WIN32
void changeText(WORD attributes)
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    SetConsoleTextAttribute(hConsole, attributes);
}

void resetText()
{
    SetConsoleTextAttribute(hConsole, originalAttributes);
}
#endif

#ifdef __linux__
char _getch(void)
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    printf("%c\n", buf);
    return buf;
}
#endif

void clear_screen(void)
{
#ifdef _WIN32
    system("CLS");
#elif __linux__
    if (system("clear"))
    {
    };
#endif
}