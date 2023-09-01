#include "../include/declarations.hpp"
#include "../include/logic.hpp"
#include "../include/GoogleMatrixAPI.hpp"
#include "../include/haversine.hpp"
#include "../include/database.hpp"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::setw;

bool start(void)
{
#ifdef _WIN32
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    originalAttributes = csbi.wAttributes;
#endif

    if (autentificare_cont() == EXIT_FAILURE)
    {
        cerr << "\n"
             << setw(5) << " "
             << "-- Authentication process did not complete!\n";
        ERROR_CNT++;
        return EXIT_FAILURE;
    }
    else
    {
        cout << setw(5) << " "
             << "-- Authentication successful!\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    if (_init_() == EXIT_FAILURE)
    {
        cerr << setw(5) << " "
             << "-- Application initialisation process did not complete!\n";
        ERROR_CNT++;
        return EXIT_FAILURE;
    }
    else if (ERROR_CNT == 1)
    {
        cout << setw(5) << " "
             << "-- All the necessary files were initialized with " << ERROR_CNT << " error!\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    else
    {
        cout << setw(5) << " "
             << "-- All the necessary files were initialized with " << ERROR_CNT << " errors!\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    cout << "\n"
         << setw(5) << " "
         << "-- Starting application configuration...\n\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    sortare_date_depozit();
    cout << setw(6) << " "
         << "--> Data sort complete (1)\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    sortare_date_oras(1);
    cout << setw(6) << " "
         << "--> Data sort complete (2)\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    sortare_date_produs(1);
    cout << setw(6) << " "
         << "--> Data sort complete (3)\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    determinare_tip_depozit();
    cout << setw(6) << " "
         << "--> Data configuration complete (1)\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    cautare_orase_stoc_limitat();
    cout << setw(6) << " "
         << "--> Data configuration complete (2)\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    cautare_orase_izolate();
    cout << setw(6) << " "
         << "--> Data configuration complete (3)\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    produse_transport_TSP();
    cout << setw(6) << " "
         << "--> Data configuration complete (4)\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    nr_max_caractere_den();
    cout << setw(6) << " "
         << "--> Data configuration complete (5)\n";

    traseu_minim_TSP[1] = -1;

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    return EXIT_SUCCESS;
}

void free_memory(void)
{
    matrice_drum.clear();
    depozite_centralizate.clear();
    orase_stoc_limitat.clear();
    orase_izolate.clear();
    orase_conexiune_unica.clear();
    stiva.clear();
    traseu_minim_TSP.clear();
    free(denumire_depozit_nou);
    AUTENTIFICARE::cleanup();
    delete con;
}

int main(int argc, char **argv)
{
    if (start() == EXIT_FAILURE)
    {
        cerr << setw(5) << "-- Can not start the application!\n";
        free_memory();
        return EXIT_FAILURE;
    }
    else if (ERROR_CNT == 1)
    {
        cout << "\n"
             << setw(5) << " "
             << "-- Application configuration completed with " << ERROR_CNT << " error!\n\n"
             << setw(5) << " "
             << "Press 'ENTER' to continue...";
        _getch();
    }
    else
    {
        cout << "\n"
             << setw(5) << " "
             << "-- Application configuration completed with " << ERROR_CNT << " errors!\n\n"
             << setw(5) << " "
             << "Press 'ENTER' to continue...";
        _getch();
    }


    unsigned int MENIU;

    do
    {
        clear_screen();

        cout << "\n\n"
             << setw(30) << " "
             << "+-------+\n"
             << setw(30) << " "
             << "| MENIU |\n"
             << setw(30) << " "
             << "+-------+\n";
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
             << "[5] Actualizare date\n"
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

                cout << "\n\n"
                     << setw(18) << " "
                     << "+--------------------+\n"
                     << setw(18) << " "
                     << "| GESTIONARE STOCURI |\n"
                     << setw(18) << " "
                     << "+--------------------+\n";
                underline(50, true);

                cout << setw(5) << " "
                     << "[1] Ruta optima a aprovizionarii depozitelor\n"
                     << setw(5) << " "
                     << "[2] Traseul optim de la un depozit la altul\n"
                     << setw(5) << " "
                     << "[3] Depozite izolate\n"
                     << setw(5) << " "
                     << "[4] Depozite cu o singura ruta de conexiune\n"
                     << setw(5) << " "
                     << "[0] EXIT\n";
                underline(50, true);

                cout << setw(5) << " "
                     << "Introduceti numarul meniului: ";
                cin >> MENIU_1;

                switch (MENIU_1)
                {
                case 1:
                    parcurgere_traseu_TSP();
                    break;
                case 2:
                    sistem_aprovizionare_independent();
                    break;
                case 3:
                    afisare_depozite_izolate();
                    _getch();
                    break;
                case 4:
                    afisare_depozite_unic_drum();
                    _getch();
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

                cout << "\n\n"
                     << setw(15) << " "
                     << "+--------------+\n"
                     << setw(15) << " "
                     << "| BAZA DE DATE |\n"
                     << setw(15) << " "
                     << "+--------------+\n";

                underline(40, true);

                cout << setw(5) << " "
                     << "[1] Tabel Depozite Ro\n"
                     << setw(5) << " "
                     << "[2] Tabel Produse\n"
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
                        clear_screen();

                        afisare_date_tabel_oras();

                        cout << setw(5) << " "
                             << "[1] Sortare dupa tipul depozitului\n"
                             << setw(5) << " "
                             << "[2] Sortare alfabetic A-Z\n"
                             << setw(5) << " "
                             << "[3] Sortare alfabetic Z-A\n"
                             << setw(5) << " "
                             << "[4] Sortare descrescatoare dupa ID\n"
                             << setw(5) << " "
                             << "[5] Reseteaza afisarea\n"
                             << setw(5) << " "
                             << "[6] Cautare date\n"
                             << setw(5) << " "
                             << "[0] EXIT\n";

                        underline(80, true);

                        cout << setw(5) << " "
                             << "Introduceti numarul meniului: ";

                        cin >> MENIU_3_1;

                        switch (MENIU_3_1)
                        {
                        case 1:
                            sortare_tip_depozit();
                            break;
                        case 2:
                            sortare_depozit_alfabetic(1);
                            break;
                        case 3:
                            sortare_depozit_alfabetic(2);
                            break;
                        case 4:
                            sortare_date_oras(2);
                            break;
                        case 5:
                            sortare_date_oras(1);
                            break;
                        case 6:
                            unsigned int MENIU_3_1_6;

                            do
                            {
                                clear_screen();

                                afisare_date_tabel_oras();

                                cout << setw(5) << " "
                                     << "[1] Cautare depozit dupa denumire\n"
                                     << setw(5) << " "
                                     << "[2] Cautare depozit dupa ID\n"
                                     << setw(5) << " "
                                     << "[0] EXIT\n";

                                underline(80, true);

                                cout << setw(5) << " "
                                     << "Introduceti numarul meniului: ";
                                cin >> MENIU_3_1_6;

                                switch (MENIU_3_1_6)
                                {
                                case 1:
                                    cautare_depozit_denumire();
                                    break;
                                case 2:
                                    cautare_oras_ID();
                                    break;

                                default:
                                    break;
                                }

                            } while (MENIU_3_1_6 != 0);

                            break;

                        default:
                            break;
                        }

                    } while (MENIU_3_1 != 0);
                }
                break;
                case 2:
                {
                    unsigned int MENIU_3_2;

                    do
                    {
                        clear_screen();

                        afisare_date_tabel_produs();

                        cout << setw(5) << " "
                             << "[1] Sortare dupa categoria produselor\n"
                             << setw(5) << " "
                             << "[2] Sortare alfabetic A-Z\n"
                             << setw(5) << " "
                             << "[3] Sortare alfabetic Z-A\n"
                             << setw(5) << " "
                             << "[4] Sortare descrescatoare dupa ID\n"
                             << setw(5) << " "
                             << "[5] Sortare in ordine crescatoare dupa pret\n"
                             << setw(5) << " "
                             << "[6] Sortare in ordine descrescatoare dupa pret\n"
                             << setw(5) << " "
                             << "[7] Reseteaza afisarea\n"
                             << setw(5) << " "
                             << "[8] Cautare date\n"
                             << setw(5) << " "
                             << "[0] EXIT\n";

                        underline(80, true);

                        cout << setw(5) << " "
                             << "Introduceti numarul meniului: ";

                        cin >> MENIU_3_2;

                        switch (MENIU_3_2)
                        {
                        case 1:
                            sortare_categorie_produs();
                            break;
                        case 2:
                            sortare_produs_alfabetic(1);
                            break;
                        case 3:
                            sortare_produs_alfabetic(2);
                            break;
                        case 4:
                            sortare_date_produs(2);
                            break;
                        case 5:
                            sortare_produs_pret(1);
                            break;
                        case 6:
                            sortare_produs_pret(2);
                            break;
                        case 7:
                            sortare_date_produs(1);
                            break;
                        case 8:
                            unsigned int MENIU_3_2_8;

                            do
                            {
                                clear_screen();

                                afisare_date_tabel_produs();

                                cout << setw(5) << " "
                                     << "[1] Cautare produs dupa denumire\n"
                                     << setw(5) << " "
                                     << "[2] Cautare produs dupa ID\n"
                                     << setw(5) << " "
                                     << "[0] EXIT\n";

                                underline(80, true);

                                cout << setw(5) << " "
                                     << "Introduceti numarul meniului: ";
                                cin >> MENIU_3_2_8;

                                switch (MENIU_3_2_8)
                                {
                                case 1:
                                    cautare_produs_denumire();
                                    break;
                                case 2:
                                    cautare_produs_ID();
                                    break;

                                default:
                                    break;
                                }

                            } while (MENIU_3_2_8 != 0);

                            break;

                        default:
                            break;
                        }

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
            unsigned int MENIU_5;

            do
            {
                clear_screen();

                cout << "\n\n"
                     << setw(15) << " "
                     << "+------------------+\n"
                     << setw(15) << " "
                     << "| ACTUALIZARE DATE |\n"
                     << setw(15) << " "
                     << "+------------------+\n";

                underline(40, true);

                cout << setw(5) << " "
                     << "[1] Actualizare trasee GPS\n"
                     << setw(5) << " "
                     << "[2] Adauga un depozit\n"
                     << setw(5) << " "
                     << "[3] Sterge un depozit\n"
                     << setw(5) << " "
                     << "[0] EXIT\n";

                underline(40, true);

                cout << setw(5) << " "
                     << "Introduceti numarul meniului: ";
                cin >> MENIU_5;

                switch (MENIU_5)
                {
                case 1:
                    _GPS_UPDATE_DATA_();
                    break;
                case 2:
                    if (SQL_Data_Update(1) == EXIT_FAILURE)
                    {
                        cerr << setw(5) << " "
                             << "SQL: Failed to updated table!";
                        _getch();
                    }
                    break;
                case 3:
                    if (SQL_Data_Update(2) == EXIT_FAILURE)
                    {
                        cerr << setw(5) << " "
                             << "SQL: Failed to updated table!";
                        _getch();
                    }
                    break;

                default:
                    break;
                }

            } while (MENIU_5 != 0);
            break;

        default:
            break;
        }

    } while (MENIU != 0);

    free_memory();
    clear_screen();

    return EXIT_SUCCESS;
}
