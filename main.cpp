#include <iostream>
#include <fstream>
#include "database.hpp"
#include "declarations.hpp"
#include "compatibilityFunctions.hpp"
#include "logic.hpp"

using namespace std;

bool start()
{
    if (autentificare_cont() == EXIT_FAILURE)
        return EXIT_FAILURE;
    else if (_init_() == EXIT_FAILURE)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int main()
{
    unsigned int MENIU;
    
    if (start() == EXIT_FAILURE)
        return EXIT_FAILURE;

    determinare_tip_depozit();
    cautare_orase_stoc_limitat();
    cautare_orase_izolate();
    sortare_date_depozit();
    sortare_date_oras();
    sortare_date_produs();

    do
    {
        clear_screen();

        cout << "\n\n";
        cout << setw(30) << " "
             << "┌───────┐\n";
        cout << setw(32) << " "
             << "MENIU\n";
        cout << setw(30) << " "
             << "└───────┘\n";
        underline(60);

        cout << setw(5) << " " << "[1] Aprovizionarea stocurilor in depozitele din Romania\n"
             << setw(5) << " " << "[2] Vizualizare stoc Romania\n"
             << setw(5) << " " << "[0] EXIT\n";

        underline(60);
        cout << setw(5) << " " << "Introduceti numarul meniului: ";

        cin >> MENIU;

        switch (MENIU)
        {
        case 1:
            unsigned int MENIU_1;
            
            do
            {
                clear_screen();

                cout << "\n\n" << setw(20) << " " << "Meniu Aprovizionare\n";
                underline(50);

                cout << setw(5) << " " << "[1] Trasee independente de aprovizionare\n"
                     << setw(5) << " " << "[2] Traseu optim de aprovizionare\n"
                     << setw(5) << " " << "[3] Depozite izolate\n"
                     << setw(5) << " " << "[4] Depozite cu un singur drum conector\n"
                     << setw(5) << " " << "[0] EXIT\n";
                underline(50);

                cout  << setw(5) << " " << "Introduceti numarul meniului: ";
                cin >> MENIU_1;

                switch (MENIU_1)
                {
                case 1:
                    sistem_aprovizionare_independent();
                    break;
                case 2:
                    back();
                    cout << "Da";

                    getch();
                    break;
                case 3:
                    afisare_depozite_izolate();
                    getch();
                    break;
                case 4:
                    break;
                
                default:
                    break;
                }
            } while (MENIU_1 != 0);

            break;
        case 2:
            vizualizare_status_stoc();
            break;
        case 3:
            getch();
            break;
        case 4:
            getch();
            break;
        case 5:
            getch();
            break;

        default:
            break;
        }

    } while (MENIU != 0);

    return EXIT_SUCCESS;
}