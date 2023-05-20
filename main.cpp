#include "declarations.hpp"
#include "logic.hpp"
#include "database.hpp"

using namespace std;

bool start(void)
{
    if (autentificare_cont() == EXIT_FAILURE)
        return EXIT_FAILURE;
    else if (_init_() == EXIT_FAILURE)
        return EXIT_FAILURE;

    determinare_tip_depozit();
    cautare_orase_stoc_limitat();
    cautare_orase_izolate();
    sortare_date_depozit();
    sortare_date_oras();
    sortare_date_produs();
    produse_transport_TSP();
    nr_max_caractere_den_produse();
    
    traseu_minim_TSP[1] = -1;

    return EXIT_SUCCESS;
}

int main(void)
{
    unsigned int MENIU;

    if (start() == EXIT_FAILURE)
        return EXIT_FAILURE;

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
        underline(60, true);

        cout << setw(5) << " "
             << "[1] Aprovizionarea stocurilor in depozitele din Romania\n"
             << setw(5) << " "
             << "[2] Analiza stoc Romania\n"
             << setw(5) << " "
             << "[3] Vizualizare baza de date\n"
             << setw(5) << " "
             << "[4] SQL Query Tool\n"
             << setw(5) << " "
             << "[0] EXIT\n";

        underline(60, true);
        cout << setw(5) << " "
             << "Introduceti numarul meniului: ";

        cin >> MENIU;

        switch (MENIU)
        {
        case 1:
            unsigned int MENIU_1;

            do
            {
                clear_screen();

                cout << "\n\n";
                cout << setw(19) << " "
                     << "┌───────────────────┐\n";
                cout << setw(20) << " "
                     << "MENIU APROVIZIONARE\n";
                cout << setw(19) << " "
                     << "└───────────────────┘\n";
                underline(50, true);

                cout << setw(5) << " "
                     << "[1] Trasee independente de aprovizionare\n"
                     << setw(5) << " "
                     << "[2] Traseu optim de aprovizionare\n"
                     << setw(5) << " "
                     << "[3] Depozite izolate\n"
                     << setw(5) << " "
                     << "[4] Depozite cu un singur drum conector\n"
                     << setw(5) << " "
                     << "[0] EXIT\n";
                underline(50, true);

                cout << setw(5) << " "
                     << "Introduceti numarul meniului: ";
                cin >> MENIU_1;

                switch (MENIU_1)
                {
                case 1:
                    sistem_aprovizionare_independent();
                    break;
                case 2:
                    parcurgere_traseu_TSP();
                    break;
                case 3:
                    afisare_depozite_izolate();
                    getch();
                    break;
                case 4:
                    afisare_depozite_unic_drum();
                    getch();
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
            unsigned int MENIU_3;

            do
            {
                clear_screen();

                cout << "\n\n";
                cout << setw(15) << " "
                     << "┌────────────┐\n";
                cout << setw(16) << " "
                     << "BAZA DE DATE\n";
                cout << setw(15) << " "
                     << "└────────────┘\n";

                underline(40, true);

                cout << setw(5) << " "
                     << "[1] Tabel depozite Ro\n"
                     << setw(5) << " "
                     << "[2] Tabel produse\n"
                     << setw(5) << " "
                     << "[0] EXIT\n";

                underline(40, true);

                cout << setw(5) << " "
                     << "Introduceti numarul meniului: ";
                cin >> MENIU_3;

                switch (MENIU_3)
                {
                case 1:
                {
                    unsigned int MENIU_3_1;

                    do
                    {
                        afisare_date_tabel_oras();
                        cin >> MENIU_3_1;
                    } while (MENIU_3_1 != 0);
                }
                    break;
                case 2:
                {
                    unsigned int MENIU_3_2;

                    do
                    {
                        afisare_date_tabel_produs();
                        cin >> MENIU_3_2;
                    } while (MENIU_3_2 != 0);
                }
                    break;

                default:
                    break;
                }
            } while (MENIU_3 != 0);

            break;
        case 4:
            consola_mysql();
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