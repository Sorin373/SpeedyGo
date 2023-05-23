#include "declarations.hpp"
#include "logic.hpp"
#include "database.hpp"
#include "Maps_Client.hpp"

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
    sortare_date_oras(1);
    sortare_date_produs(1);
    produse_transport_TSP();
    nr_max_caractere_den();

    traseu_minim_TSP[1] = -1;

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
}

int main(int argc, char *argv[])
{
    unsigned int MENIU;

    if (start() == EXIT_FAILURE)
        return EXIT_FAILURE;

    do
    {
        clear_screen();

        cout << "\n\n";
        cout << setw(28) << " "
             << "┌───────┐\n";
        cout << setw(30) << " "
             << "MENIU\n";
        cout << setw(28) << " "
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
             << "[5] Actualizare trasee GPS\n"
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
                cout << setw(18) << " "
                     << "┌──────────────────┐\n";
                cout << setw(19) << " "
                     << "GESTIONARE STOCURI\n";
                cout << setw(18) << " "
                     << "└──────────────────┘\n";
                underline(50, true);

                cout << setw(5) << " "
                     << "[1] Ruta optima a aprovizionarii depozitelor\n"
                     << setw(5) << " "
                     << "[2] Trasee optime punct-la-punct\n"
                     << setw(5) << " "
                     << "[3] Depozite izolate\n"
                     << setw(5) << " "
                     << "[4] Depozite cu o singură ruta de conexiune\n"
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

                        underline(85, true);

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
            _GPS_UPDATE_DATA_();
            _init_();
            break;

        default:
            break;
        }

    } while (MENIU != 0);

    free_memory();

    return EXIT_SUCCESS;
}