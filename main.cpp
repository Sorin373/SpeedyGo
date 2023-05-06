#include <iostream>
#include "database.hpp"
#include "declarations.hpp"
#include "compatibilityFunctions.hpp"
#include "logic.hpp"

using namespace std;

int main()
{
    if (autentificare_cont() == EXIT_FAILURE)
        return EXIT_FAILURE;
    else if (_init_() == EXIT_FAILURE)
        return EXIT_FAILURE;

    unsigned int MENIU;

    do
    {
        clear_screen();

        cout << "\n\n" << setw(20) << " " << "MENIU\n";
        underline(45);

        cout << setw(5) << " " << "[1] Refacere stoc la nivel de tara\n";
        cout << "\n\n";
        cout << setw(5) << " " << "Introduce numarul pentru";
        cin >> MENIU;

        switch (MENIU)
        {
        case 1:
            //vizualizare_date();
            getch();
            break;
        case 2:
            statisticaStoc();
            getch();
            break;
        case 3:
            afisareDateDepozit();
            getch();
            break;
        case 4:
            afisareDateOrase();
            getch();
            break;
        case 5:
            sistem_aprovizionare();
            getch();
            break;
        }

    } while (MENIU != 0);

    matrice_drum.clear();
    depozite_centralizate.clear();

    return EXIT_SUCCESS;
}