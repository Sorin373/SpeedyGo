#include <iostream>
#include <fstream>
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

        cout << setw(5) << " " << "[1] Aprovizioare stoc Romania\n"
             << setw(5) << " " << "[2] Vizualizare stoc Romania\n"
             << setw(5) << " " << "[0] EXIT\n";

        underline(45);
        cout << setw(5) << " " << "Introduceti numarul meniului: ";

        cin >> MENIU;

        switch (MENIU)
        {
        case 1:
            verificare_rute();
            getch();
            break;
        case 2:
            vizualizare_status_stoc();
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

        default:
            break;
        }

    } while (MENIU != 0);

    return EXIT_SUCCESS;
}