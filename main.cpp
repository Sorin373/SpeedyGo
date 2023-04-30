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
    else if (accesareDate() == EXIT_FAILURE || _init_() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if (true)
    {   
        unsigned int MENIU;

        do
        {
            clear_screen();

            cout << "MENIU | ADMINISTRATOR\n\n"
                 << "[1]\n" 
                 << "[2]\n"
                 << "[3]\n";

            cout << "Introduceti optiunea: ";
            cin >> MENIU;

            switch (MENIU)
            {
                case 1:
                    afisareDateDepozit();
                    getch();
                    break;
                case 2:
                    afisareDateOrase();
                    getch();
                    break;
                case 3:
                    afisareDateProdus();
                    getch();
                    break;
                case 4:
                    sistem_aprovizionare();
                    getch();
                    break;
                case 5:
                    getch();
                    break;
            }

        } while (MENIU != 0);

        //matrice_drum.clear();
        //depozite_centralizate.clear();

    }
    else
    {
        cerr << "Nu s-a putut autentifica contul!";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}