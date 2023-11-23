/*
 *
 *                sSpeedyGo - A Depot Management Application
 *
 * Comaptibility: I provide both UNIX and WINDOWS executables since I also wanted to learn about system compatibility.
 *                It is tested on Ubuntu 22.04 and Windows 10 (22H2).
 *
 * Author:        Sorin Tudose
 *
 * Description:   This application manages depots restocking operations, including inventory analysis, database visualization, and more.
 *                This app was created from scratch as a final project only by using some sources and documentation for the 3rd party libraries and vcpkg
 *
 *                (https://github.com/nlohmann/json)
 *                (https://curl.se/libcurl/c/)
 *                (https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-getting-started-examples.html)
 *                (https://vcpkg.io/en/getting-started)
 *
 * Github:        https://github.com/sorin373/SpeedyGo
 *
 * Test it:       To run the app yourself I provided a demo database in the github repo and a set of instructions.
 *                ! Please report any bugs !
 *
 *                Thank you for checking out my project! :D
 *
 */

#include "../include/speedyGo.hpp"
#include "../include/tsp.hpp"
#include "../include/dijkstra.hpp"
#include "../include/declarations.hpp"
#include "../include/GoogleMatrixAPI.hpp"
#include "../include/haversine.hpp"
#include "../include/database.hpp"

#include <iostream>
#include <iomanip>

int main(void)
{
    if (speedyGo::_INIT_() == EXIT_FAILURE)
    {
        std::cerr << std::setw(5) << " "
                  << "-- Can not start the application!\n";
        _getch();
        speedyGo::free_memory();
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

    unsigned int MENU;

    do
    {
        clear_screen();

        std::cout << "\n\n"
                  << std::setw(30) << " "
                  << "+------+\n"
                  << std::setw(30) << " "
                  << "| MENU |\n"
                  << std::setw(30) << " "
                  << "+------+\n";
        underline(60, true);

        std::cout << std::setw(5) << " "
                  << "[1] Restocking the depots\n"
                  << std::setw(5) << " "
                  << "[2] Inventory Analysis \n"
                  << std::setw(5) << " "
                  << "[3] Database Visualization\n"
                  << std::setw(5) << " "
                  << "[4] SQL Query Tool\n"
                  << std::setw(5) << " "
                  << "[5] Updating data\n"
                  << std::setw(5) << " "
                  << "[0] EXIT\n";

        underline(60, true);
        std::cout << std::setw(5) << " "
                  << "Enter menu number: ";

        speedyGo::cin >> MENU;

        switch (MENU)
        {
        case 1:
            unsigned int MENU_1;

            do
            {
                clear_screen();

                std::cout << "\n\n"
                          << std::setw(18) << " "
                          << "+------------------+\n"
                          << std::setw(18) << " "
                          << "| STOCK MANAGEMENT |\n"
                          << std::setw(18) << " "
                          << "+------------------+\n";
                underline(50, true);

                std::cout << std::setw(5) << " "
                          << "[1] Optimal depot restocking route\n"
                          << std::setw(5) << " "
                          << "[2] Optimal route between depots\n"
                          << std::setw(5) << " "
                          << "[3] Isolated depots\n"
                          << std::setw(5) << " "
                          << "[4] Depots with a single connection route\n"
                          << std::setw(5) << " "
                          << "[0] EXIT\n";
                underline(50, true);

                std::cout << std::setw(5) << " "
                          << "Enter menu number: ";
                speedyGo::cin >> MENU_1;

                switch (MENU_1)
                {
                case 1:
                    tsp::routeTraversalTSP();
                    break;
                case 2:
                    Dijkstra::CityToCityDistanceCalculator();
                    break;
                case 3:
                    CITY::CITY_NODE::displayIsolatedCities();
                    _getch();
                    break;
                case 4:
                    CITY::CITY_NODE::displayUniqueRouteDepots();
                    _getch();
                    break;

                default:
                    break;
                }
            } while (MENU_1 != 0);

            break;
        case 2:
            speedyGo::stockStatusVisualization();
            break;
        case 3:
            unsigned int MENU_3;

            do
            {
                clear_screen();

                std::cout << "\n\n"
                          << std::setw(15) << " "
                          << "+----------+\n"
                          << std::setw(15) << " "
                          << "| DATABASE |\n"
                          << std::setw(15) << " "
                          << "+----------+\n";

                underline(40, true);

                std::cout << std::setw(5) << " "
                          << "[1] Depots table\n"
                          << std::setw(5) << " "
                          << "[2] Products Table\n"
                          << std::setw(5) << " "
                          << "[0] EXIT\n";

                underline(40, true);

                std::cout << std::setw(5) << " "
                          << "Enter menu number: ";
                speedyGo::cin >> MENU_3;

                switch (MENU_3)
                {
                case 1:
                {
                    unsigned int MENU_3_1;

                    do
                    {
                        clear_screen();

                        CITY::CITY_NODE::displayCityData();

                        std::cout << std::setw(5) << " "
                                  << "[1] Sort by depot category\n"
                                  << std::setw(5) << " "
                                  << "[2] Sort alphabetically A-Z\n"
                                  << std::setw(5) << " "
                                  << "[3] Sort alphabetically Z-A\n"
                                  << std::setw(5) << " "
                                  << "[4] Sort by ID descending\n"
                                  << std::setw(5) << " "
                                  << "[5] Reset data order\n"
                                  << std::setw(5) << " "
                                  << "[6] Search data\n"
                                  << std::setw(5) << " "
                                  << "[0] EXIT\n";

                        underline(80, true);

                        std::cout << std::setw(5) << " "
                                  << "Enter menu number: ";

                        speedyGo::cin >> MENU_3_1;

                        switch (MENU_3_1)
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
                            unsigned int MENU_3_1_6;

                            do
                            {
                                clear_screen();

                                CITY::CITY_NODE::displayCityData();

                                std::cout << std::setw(5) << " "
                                          << "[1] Search depot by name\n"
                                          << std::setw(5) << " "
                                          << "[2] Search depot by ID\n"
                                          << std::setw(5) << " "
                                          << "[0] EXIT\n";

                                underline(80, true);

                                std::cout << std::setw(5) << " "
                                          << "Enter menu number: ";
                                speedyGo::cin >> MENU_3_1_6;

                                switch (MENU_3_1_6)
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

                            } while (MENU_3_1_6 != 0);

                            break;

                        default:
                            break;
                        }

                    } while (MENU_3_1 != 0);
                }
                break;
                case 2:
                {
                    unsigned int MENU_3_2;

                    do
                    {
                        clear_screen();

                        PRODUCT::PRODUCT_NODE::displayProductData();

                        std::cout << std::setw(5) << " "
                                  << "[1] Sort by product category\n"
                                  << std::setw(5) << " "
                                  << "[2] Sort alphabetically A-Z\n"
                                  << std::setw(5) << " "
                                  << "[3] Sort alphabetically Z-A\n"
                                  << std::setw(5) << " "
                                  << "[4] Sort by ID descending\n"
                                  << std::setw(5) << " "
                                  << "[5] Sort by price ascending\n"
                                  << std::setw(5) << " "
                                  << "[6] Sort by price descending\n"
                                  << std::setw(5) << " "
                                  << "[7] Reset data order\n"
                                  << std::setw(5) << " "
                                  << "[8] Search data\n"
                                  << std::setw(5) << " "
                                  << "[0] EXIT\n";

                        underline(80, true);

                        std::cout << std::setw(5) << " "
                                  << "Enter menu number: ";

                        speedyGo::cin >> MENU_3_2;

                        switch (MENU_3_2)
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
                            unsigned int MENU_3_2_8;

                            do
                            {
                                clear_screen();

                                PRODUCT::PRODUCT_NODE::displayProductData();

                                std::cout << std::setw(5) << " "
                                          << "[1] Search product by name\n"
                                          << std::setw(5) << " "
                                          << "[2] Search product by ID\n"
                                          << std::setw(5) << " "
                                          << "[0] EXIT\n";

                                underline(80, true);

                                std::cout << std::setw(5) << " "
                                          << "Enter menu number: ";
                                speedyGo::cin >> MENU_3_2_8;

                                switch (MENU_3_2_8)
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

                            } while (MENU_3_2_8 != 0);

                            break;

                        default:
                            PRODUCT::PRODUCT_NODE::sortData(1);
                            break;
                        }

                    } while (MENU_3_2 != 0);
                }
                break;

                default:
                    break;
                }
            } while (MENU_3 != 0);

            break;
        case 4:
            speedyGo::MySQLconsole();
            break;
        case 5:
            unsigned int MENU_5;

            do
            {
                clear_screen();

                std::cout << "\n\n"
                          << std::setw(15) << " "
                          << "+-------------+\n"
                          << std::setw(15) << " "
                          << "| DATA UPDATE |\n"
                          << std::setw(15) << " "
                          << "+-------------+\n";

                underline(40, true);

                std::cout << std::setw(5) << " "
                          << "[1] Update GPS routes\n"
                          << std::setw(5) << " "
                          << "[2] Add a depot\n"
                          << std::setw(5) << " "
                          << "[3] Delete a depot\n"
                          << std::setw(5) << " "
                          << "[0] EXIT\n";

                underline(40, true);

                std::cout << std::setw(5) << " "
                          << "Enter menu number: ";
                speedyGo::cin >> MENU_5;

                switch (MENU_5)
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

            } while (MENU_5 != 0);
            break;

        default:
            break;
        }

    } while (MENU != 0);

    speedyGo::free_memory();
    clear_screen();

    return EXIT_SUCCESS;
}
