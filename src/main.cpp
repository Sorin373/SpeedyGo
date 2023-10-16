#include "../include/declarations.hpp"
#include "../include/logic.hpp"
#include "../include/GoogleMatrixAPI.hpp"
#include "../include/haversine.hpp"
#include "../include/database.hpp"

#include <iostream>
#include <iomanip>

bool _INIT_(void)
{
#ifdef _WIN32
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    originalAttributes = csbi.wAttributes;
#endif

    if (validateMysqlCredentials() == EXIT_FAILURE)
    {
        std::cerr << "\n"
                  << std::setw(5) << " "
                  << "-- Authentication process did not complete!\n";
        ERROR_CNT++;
        return EXIT_FAILURE;
    }
    else
        std::cout << std::setw(5) << " "
                  << "-- Authentication successful!\n";

    if (_ADJACENCY_MATRIX_INIT_() == EXIT_FAILURE)
    {
        std::cerr << std::setw(5) << " "
                  << "-- Application initialisation process did not complete!\n";
        ERROR_CNT++;
        return EXIT_FAILURE;
    }
    else if (ERROR_CNT == 1)
        std::cout << std::setw(5) << " "
                  << "-- All the necessary files were initialized with " << ERROR_CNT << " error!\n";
    else
        std::cout << std::setw(5) << " "
                  << "-- All the necessary files were initialized with " << ERROR_CNT << " errors!\n";

    std::cout << "\n"
              << std::setw(5) << " "
              << "-- Starting application configuration...\n\n";

    DEPOT::DEPOT_NODE::sortData();
    std::cout << std::setw(6) << " "
              << "--> Data sort complete (1)\n";

    CITY::CITY_NODE::sortData(1);
    std::cout << std::setw(6) << " "
              << "--> Data sort complete (2)\n";

    PRODUCT::PRODUCT_NODE::sortData(1);
    std::cout << std::setw(6) << " "
              << "--> Data sort complete (3)\n";

    determinare_tip_depozit();
    std::cout << std::setw(6) << " "
              << "--> Data configuration complete (1)\n";

    cautare_orase_stoc_limitat();
    std::cout << std::setw(6) << " "
              << "--> Data configuration complete (2)\n";

    cautare_orase_izolate();
    std::cout << std::setw(6) << " "
              << "--> Data configuration complete (3)\n";

    produse_transport_TSP();
    std::cout << std::setw(6) << " "
              << "--> Data configuration complete (4)\n";

    nr_max_caractere_den();
    std::cout << std::setw(6) << " "
              << "--> Data configuration complete (5)\n";

    minimumRouteTSP[1] = -1;

    return EXIT_SUCCESS;
}

void free_memory(void)
{
    adjacencyMatrix.clear();
    centralDepos.clear();
    limitedStockCities.clear();
    isolatedVertex.clear();
    oneEdgeVertex.clear();
    stack.clear();
    minimumRouteTSP.clear();
    free(newCityName);
    AUTHENTICATION::cleanup();
    delete con;
}

int main(int argc, char **argv)
{
    if (_INIT_() == EXIT_FAILURE)
    {
        std::cerr << std::setw(5) << " "
                  << "-- Can not start the application!\n";
        _getch();
        free_memory();
        return EXIT_FAILURE;
    }
    else if (ERROR_CNT == 1)
    {
        std::cout << "\n"
                  << std::setw(5) << " "
                  << "-- Application configuration completed with " << ERROR_CNT << " error!\n\n";

        underline(65, true);

        std::cout << std::setw(5) << " "
                  << "Press 'ENTER' to continue...";
        _getch();
    }
    else
    {
        std::cout << "\n"
                  << std::setw(5) << " "
                  << "-- Application configuration completed with " << ERROR_CNT << " errors!\n\n";

        underline(65, true);

        std::cout << std::setw(5) << " "
                  << "Press 'ENTER' to continue...";

        _getch();
    }

    unsigned int MENIU;

    do
    {
        clear_screen();

        std::cout << "\n\n"
                  << std::setw(30) << " "
                  << "+-------+\n"
                  << std::setw(30) << " "
                  << "| MENIU |\n"
                  << std::setw(30) << " "
                  << "+-------+\n";
        underline(60, true);

        std::cout << std::setw(5) << " "
                  << "[1] Aprovizionarea stocurilor in depozitele din Romania\n"
                  << std::setw(5) << " "
                  << "[2] Analiza stoc Romania\n"
                  << std::setw(5) << " "
                  << "[3] Vizualizare baza de date\n"
                  << std::setw(5) << " "
                  << "[4] SQL Query Tool\n"
                  << std::setw(5) << " "
                  << "[5] Actualizare date\n"
                  << std::setw(5) << " "
                  << "[0] EXIT\n";

        underline(60, true);
        std::cout << std::setw(5) << " "
                  << "Introduceti numarul meniului: ";

        std::cin >> MENIU;

        switch (MENIU)
        {
        case 1:
            unsigned int MENIU_1;

            do
            {
                clear_screen();

                std::cout << "\n\n"
                          << std::setw(18) << " "
                          << "+--------------------+\n"
                          << std::setw(18) << " "
                          << "| GESTIONARE STOCURI |\n"
                          << std::setw(18) << " "
                          << "+--------------------+\n";
                underline(50, true);

                std::cout << std::setw(5) << " "
                          << "[1] Ruta optima a aprovizionarii depozitelor\n"
                          << std::setw(5) << " "
                          << "[2] Traseul optim de la un depozit la altul\n"
                          << std::setw(5) << " "
                          << "[3] Depozite izolate\n"
                          << std::setw(5) << " "
                          << "[4] Depozite cu o singura ruta de conexiune\n"
                          << std::setw(5) << " "
                          << "[0] EXIT\n";
                underline(50, true);

                std::cout << std::setw(5) << " "
                          << "Introduceti numarul meniului: ";
                std::cin >> MENIU_1;

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

                std::cout << "\n\n"
                          << std::setw(15) << " "
                          << "+--------------+\n"
                          << std::setw(15) << " "
                          << "| BAZA DE DATE |\n"
                          << std::setw(15) << " "
                          << "+--------------+\n";

                underline(40, true);

                std::cout << std::setw(5) << " "
                          << "[1] Tabel Depozite Ro\n"
                          << std::setw(5) << " "
                          << "[2] Tabel Produse\n"
                          << std::setw(5) << " "
                          << "[0] EXIT\n";

                underline(40, true);

                std::cout << std::setw(5) << " "
                          << "Introduceti numarul meniului: ";
                std::cin >> MENIU_3;

                switch (MENIU_3)
                {
                case 1:
                {
                    unsigned int MENIU_3_1;

                    do
                    {
                        clear_screen();

                        CITY::CITY_NODE::displayCityData();

                        std::cout << std::setw(5) << " "
                                  << "[1] Sortare dupa tipul depozitului\n"
                                  << std::setw(5) << " "
                                  << "[2] Sortare alfabetic A-Z\n"
                                  << std::setw(5) << " "
                                  << "[3] Sortare alfabetic Z-A\n"
                                  << std::setw(5) << " "
                                  << "[4] Sortare descrescatoare dupa ID\n"
                                  << std::setw(5) << " "
                                  << "[5] Reseteaza afisarea\n"
                                  << std::setw(5) << " "
                                  << "[6] Cautare date\n"
                                  << std::setw(5) << " "
                                  << "[0] EXIT\n";

                        underline(80, true);

                        std::cout << std::setw(5) << " "
                                  << "Introduceti numarul meniului: ";

                        std::cin >> MENIU_3_1;

                        switch (MENIU_3_1)
                        {
                        case 1:
                            CITY::CITY_NODE::filterCityByCategory();
                            break;
                        case 2:
                            CITY::CITY_NODE::alphabeticalCitySort(1);
                            break;
                        case 3:
                            CITY::CITY_NODE::alphabeticalCitySort(2);
                            break;
                        case 4:
                            CITY::CITY_NODE::sortData(2);
                            break;
                        case 5:
                            CITY::CITY_NODE::sortData(1);
                            break;
                        case 6:
                            unsigned int MENIU_3_1_6;

                            do
                            {
                                clear_screen();

                                CITY::CITY_NODE::displayCityData();

                                std::cout << std::setw(5) << " "
                                          << "[1] Cautare depozit dupa denumire\n"
                                          << std::setw(5) << " "
                                          << "[2] Cautare depozit dupa ID\n"
                                          << std::setw(5) << " "
                                          << "[0] EXIT\n";

                                underline(80, true);

                                std::cout << std::setw(5) << " "
                                          << "Introduceti numarul meniului: ";
                                std::cin >> MENIU_3_1_6;

                                switch (MENIU_3_1_6)
                                {
                                case 1:
                                    CITY::CITY_NODE::searchCityByName();
                                    break;
                                case 2:
                                    CITY::CITY_NODE::searchCityByID();
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

                        PRODUCT::PRODUCT_NODE::displayProductData();

                        std::cout << std::setw(5) << " "
                                  << "[1] Sortare dupa categoria produselor\n"
                                  << std::setw(5) << " "
                                  << "[2] Sortare alfabetic A-Z\n"
                                  << std::setw(5) << " "
                                  << "[3] Sortare alfabetic Z-A\n"
                                  << std::setw(5) << " "
                                  << "[4] Sortare descrescatoare dupa ID\n"
                                  << std::setw(5) << " "
                                  << "[5] Sortare in ordine crescatoare dupa pret\n"
                                  << std::setw(5) << " "
                                  << "[6] Sortare in ordine descrescatoare dupa pret\n"
                                  << std::setw(5) << " "
                                  << "[7] Reseteaza afisarea\n"
                                  << std::setw(5) << " "
                                  << "[8] Cautare date\n"
                                  << std::setw(5) << " "
                                  << "[0] EXIT\n";

                        underline(80, true);

                        std::cout << std::setw(5) << " "
                                  << "Introduceti numarul meniului: ";

                        std::cin >> MENIU_3_2;

                        switch (MENIU_3_2)
                        {
                        case 1:
                            PRODUCT::PRODUCT_NODE::filterProductByCategory();
                            break;
                        case 2:
                            PRODUCT::PRODUCT_NODE::alphabeticalProductSort(1);
                            break;
                        case 3:
                            PRODUCT::PRODUCT_NODE::alphabeticalProductSort(2);
                            break;
                        case 4:
                            PRODUCT::PRODUCT_NODE::sortData(2);
                            break;
                        case 5:
                            PRODUCT::PRODUCT_NODE::sortDataByPrice(1);
                            break;
                        case 6:
                            PRODUCT::PRODUCT_NODE::sortDataByPrice(2);
                            break;
                        case 7:
                            PRODUCT::PRODUCT_NODE::sortData(1);
                            break;
                        case 8:
                            unsigned int MENIU_3_2_8;

                            do
                            {
                                clear_screen();

                                PRODUCT::PRODUCT_NODE::displayProductData();

                                std::cout << std::setw(5) << " "
                                          << "[1] Cautare produs dupa denumire\n"
                                          << std::setw(5) << " "
                                          << "[2] Cautare produs dupa ID\n"
                                          << std::setw(5) << " "
                                          << "[0] EXIT\n";

                                underline(80, true);

                                std::cout << std::setw(5) << " "
                                          << "Introduceti numarul meniului: ";
                                std::cin >> MENIU_3_2_8;

                                switch (MENIU_3_2_8)
                                {
                                case 1:
                                    PRODUCT::PRODUCT_NODE::searchProductByName();
                                    break;
                                case 2:
                                    PRODUCT::PRODUCT_NODE::searchProductByID();
                                    break;

                                default:
                                    break;
                                }

                            } while (MENIU_3_2_8 != 0);

                            break;

                        default:
                            PRODUCT::PRODUCT_NODE::sortData(1);
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

                std::cout << "\n\n"
                          << std::setw(15) << " "
                          << "+------------------+\n"
                          << std::setw(15) << " "
                          << "| ACTUALIZARE DATE |\n"
                          << std::setw(15) << " "
                          << "+------------------+\n";

                underline(40, true);

                std::cout << std::setw(5) << " "
                          << "[1] Actualizare trasee GPS\n"
                          << std::setw(5) << " "
                          << "[2] Adauga un depozit\n"
                          << std::setw(5) << " "
                          << "[3] Sterge un depozit\n"
                          << std::setw(5) << " "
                          << "[0] EXIT\n";

                underline(40, true);

                std::cout << std::setw(5) << " "
                          << "Introduceti numarul meniului: ";
                std::cin >> MENIU_5;

                switch (MENIU_5)
                {
                case 1:
                    _GOOGLE_MATRIX_API_INIT_();
                    break;
                case 2:
                    if (CITY::addCity() == EXIT_FAILURE)
                    {
                        std::cerr << std::setw(5) << " "
                                  << "sql-> Failed to add row!\n";
                        _getch();
                    }
                    break;
                case 3:
                    if (CITY::deleteCity() == EXIT_FAILURE)
                    {
                        std::cerr << std::setw(5) << " "
                                  << "sql-> Failed to delete row!\n";
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
