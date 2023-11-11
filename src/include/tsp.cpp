#include "../../include/speedyGo.hpp"
#include "../../include/tsp.hpp"
#include "../../include/declarations.hpp"
#include "../../include/database.hpp"

#include <iostream>
#include <iomanip>
#include <fstream>

void tsp::hamiltonianGraph::initStack(void)
{
    stack[stackCounter] = -1;
}

bool tsp::hamiltonianGraph::successor(void)
{
    if (stack[stackCounter] < VERTEX_COUNT - 1)
    {
        stack[stackCounter]++;
        return true;
    }
    return false;
}

bool tsp::hamiltonianGraph::solution(void)
{
    if (stackCounter == limited_stock_cities_count)
        return true;
    return false;
}

bool tsp::hamiltonianGraph::valid(void)
{
    for (unsigned int i = 1; i < stackCounter; i++)
        if (stack[stackCounter] == stack[i])
            return false;

    if (stackCounter > 1)
        if (adjacencyMatrix[stack[stackCounter]][stack[stackCounter - 1]].distance == 0)
            return false;

    if (stackCounter > 1)
        if (centralDepos[stack[1]] == false)
            return false;

    return true;
}

void tsp::hamiltonianGraph::determiningMinimumHamiltonianCycle(void)
{
    double totalDistance = 0.0;
    int totalDuration = 0;

    for (int i = 1; i <= stackCounter; i++)
    {
        totalDistance += adjacencyMatrix[stack[i]][stack[i + 1]].distance;
        totalDuration += adjacencyMatrix[stack[i]][stack[i + 1]].duration;
    }

    if (totalDistance < minimumDistanceCostTSP || (totalDistance == minimumDistanceCostTSP && totalDuration < minimumDurationCostTSP))
    {
        minimumDistanceCostTSP = totalDistance;
        minimumDurationCostTSP = totalDuration;

        for (int i = 1; i <= stackCounter; i++)
        {
            minimumRouteTSP[i] = stack[i];
            TSP_RouteCounter = stackCounter;
        }
    }
}

void tsp::hamiltonianGraph::back(void)
{
    stackCounter = 1;
    tsp::hamiltonianGraph::initStack();
    while (stackCounter > 0)
    {
        int vSuccesor, vValid;
        do
        {
            vSuccesor = tsp::hamiltonianGraph::successor();
            if (vSuccesor == 1)
                vValid = tsp::hamiltonianGraph::valid();
        } while (vSuccesor == 1 && vValid == 0);
        if (vSuccesor == 1)
        {
            if (tsp::hamiltonianGraph::solution() == 1)
                tsp::hamiltonianGraph::determiningMinimumHamiltonianCycle();
            else
            {
                stackCounter++;
                tsp::hamiltonianGraph::initStack();
            }
        }
        else
            stackCounter--;
    }
}

void tsp::acyclicGraph::initStack(void)
{
    stack[stackCounter] = -1;
}

bool tsp::acyclicGraph::successor(void)
{
    if (stack[stackCounter] < VERTEX_COUNT - 1)
    {
        stack[stackCounter]++;
        return true;
    }
    return false;
}

bool tsp::acyclicGraph::solution(void)
{
    if (stackCounter == VERTEX_COUNT + 1)
        return true;
    return false;
}

bool tsp::acyclicGraph::valid(void)
{
    if (stackCounter == VERTEX_COUNT + 1)
        for (unsigned int i = 0; i < VERTEX_COUNT; i++)
        {
            bool isFound = false;
            if (limitedStockCities[i] == true && !isolatedVertex[i])
            {
                for (unsigned int j = 1; j <= stackCounter && !isFound; j++)
                    if (stack[j] == i)
                        isFound = true;

                if (!isFound)
                    return false;
            }
        }

    if (stackCounter > 1)
        if (adjacencyMatrix[stack[stackCounter]][stack[stackCounter - 1]].distance == 0)
            return false;

    if (stackCounter > 1)
        if (centralDepos[stack[1]] == false)
            return false;

    return true;
}

void tsp::acyclicGraph::determiningMinimumRoute(void)
{
    double totalDistance = 0.0;
    int totalDuration = 0;

    for (int i = 1; i < stackCounter; i++)
    {
        totalDistance += adjacencyMatrix[stack[i]][stack[i + 1]].distance;
        totalDuration += adjacencyMatrix[stack[i]][stack[i + 1]].duration;
    }

    if (totalDistance < minimumDistanceCostTSP || (totalDistance == minimumDistanceCostTSP && totalDuration < minimumDurationCostTSP))
    {
        minimumDistanceCostTSP = totalDistance;
        minimumDurationCostTSP = totalDuration;

        for (int i = 1; i <= stackCounter; i++)
        {
            minimumRouteTSP[i] = stack[i];
            TSP_RouteCounter = stackCounter;
        }
    }
}

void tsp::acyclicGraph::back(void)
{
    int vSuccesor, vValid;
    stackCounter = 1;
    tsp::acyclicGraph::initStack();
    while (stackCounter > 0)
    {
        do
        {
            vSuccesor = tsp::acyclicGraph::successor();
            if (vSuccesor == 1)
                vValid = tsp::acyclicGraph::valid();
        } while (vSuccesor == 1 && vValid == 0);
        if (vSuccesor == 1)
            if (tsp::acyclicGraph::solution() == 1)
                tsp::acyclicGraph::determiningMinimumRoute();
            else
            {
                stackCounter++;
                tsp::acyclicGraph::initStack();
            }
        else
            stackCounter--;
    }
}

void tsp::TSP(void)
{
#ifdef _WIN32
    changeText(FOREGROUND_INTENSITY | FOREGROUND_BLUE);
    std::cout << std::setw(5) << " "
              << "Calculating the most optimal route...\n";
    resetText();
#elif __linux__
    std::cout << "\033[1;34m" << std::setw(5) << " "
              << "Calculating the most optimal route..."
              << "\033[0m"
              << "\n";
#endif

    bool vIsolatedVertex = false;
    for (unsigned int i = 0; i < VERTEX_COUNT && !vIsolatedVertex; i++)
        if (isolatedVertex[i] == true)
            vIsolatedVertex = true;

    if (!vIsolatedVertex)
    {
        tsp::hamiltonianGraph::back();
        clear_screen();
        std::cout << "\n";

        if (!minimumRouteTSP.empty())
        {
#ifdef _WIN32
            changeText(FOREGROUND_INTENSITY);
            std::cout << std::setw(5) << " "
                      << "Route length: " << minimumDistanceCostTSP << "km | "
                      << "Route duration: " << minimumDurationCostTSP << "min"
                      << "\n"
                      << std::setw(5) << " ";
            resetText();
#elif __linux__
            std::cout << std::setw(5) << " "
                      << "\033[1m"
                      << "Route length: " << minimumDistanceCostTSP << "km | "
                      << "Route duration: " << minimumDurationCostTSP << "min\n"
                      << std::setw(5) << " "
                      << "\033[0m";
#endif
            for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
                for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
                    if (std::stoi(city_data->getCityID()) == minimumRouteTSP[i])
                    {
                        std::cout << city_data->getCityName();
                        if (i < TSP_RouteCounter)
                            std::cout << " --> ";
                        break;
                    }

            std::cout << "\n";
            underline(190, false);
        }
        else
        {
            minimumRouteTSP.clear();
            stack.clear();
            tsp::acyclicGraph::back();

            clear_screen();
            std::cout << "\n";

            if (!minimumRouteTSP.empty())
            {
#ifdef _WIN32
                changeText(FOREGROUND_INTENSITY);

                std::cout << std::setw(5) << " "
                          << "Route length: " << minimumDistanceCostTSP << "km | "
                          << "Route duration: " << minimumDurationCostTSP << "min"
                          << "\n"
                          << std::setw(5) << " ";

                resetText();
#elif __linux__
                std::cout << std::setw(5) << " "
                          << "\033[1m"
                          << "Route length: " << minimumDistanceCostTSP << "km | "
                          << "Route duration: " << minimumDurationCostTSP << "min\n"
                          << std::setw(5) << " "
                          << "\033[0m";
#endif
                for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
                    for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
                        if (std::stoi(city_data->getCityID()) == minimumRouteTSP[i])
                        {
                            std::cout << city_data->getCityName();
                            if (i < TSP_RouteCounter)
                                std::cout << " --> ";
                            break;
                        }

                std::cout << "\n";
                underline(190, false);
            }
            else
                std::cout << std::setw(5) << " "
                          << "--> All depots are isolated!\n";
        }
    }
    else
    {
        tsp::acyclicGraph::back();
        clear_screen();

        std::cout << "\n";
        if (!minimumRouteTSP.empty())
        {
#ifdef _WIN32
            changeText(FOREGROUND_INTENSITY);

            std::cout << std::setw(5) << " "
                      << "Route length: " << minimumDistanceCostTSP << "km | "
                      << "Route duration: " << minimumDurationCostTSP << "min"
                      << "\n"
                      << std::setw(5) << " ";

            resetText();
#elif __linux__
            std::cout << std::setw(5) << " "
                      << "\033[1m"
                      << "Route length: " << minimumDistanceCostTSP << "km | "
                      << "Route duration: " << minimumDurationCostTSP << "min\n"
                      << std::setw(5) << " "
                      << "\033[0m";
#endif

            for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
                for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
                    if (std::stoi(city_data->getCityID()) == minimumRouteTSP[i])
                    {
                        std::cout << city_data->getCityName();
                        if (i != TSP_RouteCounter)
                            std::cout << " --> ";
                        break;
                    }

            std::cout << "\n";
            underline(190, false);
        }
        else
            std::cout << "\n"
                      << std::setw(5) << " "
                      << "--> All depots are isolated!\n";
    }
}

void tsp::productTransportTSP(void)
{
    for (PRODUCT::PRODUCT_NODE *product_data = product.getHead(); product_data != nullptr; product_data = product_data->next)
    {
        int PRODUCT_ID = std::stoi(product_data->getProductID()), quantity = 0;
        for (DEPOT::DEPOT_NODE *depot_data = depot.getHead(); depot_data != nullptr; depot_data = depot_data->next)
            if (limitedStockCities[std::stoi(depot_data->getCityID())] == true &&
                isolatedVertex[std::stoi(depot_data->getCityID())] == false &&
                centralDepos[std::stoi(depot_data->getCityID())] == false)
                if (PRODUCT_ID == std::stoi(depot_data->getProductID()))
                    quantity += MAXIMUM_STOCK_VALUE - depot_data->getProductQuantity();

        supply.getData(product_data->getProductID(), quantity);
    }
}

void tsp::mainPageTSP(void)
{
    clear_screen();

    traveledDistanceTSP = 0.0;
    elapsedDurationTSP = 0;

    std::cout << "\n";
    if (!minimumRouteTSP.empty())
    {
        std::cout << std::setw(5) << " "
                  << "Route length: " << minimumDistanceCostTSP << "km | "
                  << "Route duration: " << minimumDurationCostTSP << "min\n"
                  << std::setw(5) << " ";

        for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
            for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
            {
                int ID = std::stoi(city_data->getCityID());
                if (i == 1)
                    if (ID == minimumRouteTSP[1])
                    {
#ifdef _WIN32
                        changeText(FOREGROUND_INTENSITY | COMMON_LVB_UNDERSCORE | FOREGROUND_RED | FOREGROUND_GREEN);
                        std::cout << city_data->getCityName();
                        resetText();
                        std::cout << " --> ";
#elif __linux__
                        std::cout << "\033[4m"
                                  << "\033[1m" << city_data->getCityName() << "\033[0m"
                                  << " --> ";
#endif
                    }

                if (ID == minimumRouteTSP[i] && i != 1)
                {
                    std::cout << city_data->getCityName();
                    if (i < TSP_RouteCounter)
                        std::cout << " --> ";
                    break;
                }
            }
    }

    std::cout << "\n";
    underline(190, false);

    std::cout << "\n";
    for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
        if (std::stoi(city_data->getCityID()) == minimumRouteTSP[1])
        {
            std::cout << std::setw(5) << " "
                      << "+-----------------------+\n"
                      << std::setw(8) << " [" << city_data->getCityID() << "] " << city_data->getCityName() << " - START\n"
                      << std::setw(5) << " "
                      << "+-----------------------+";
            break;
        }

#ifdef _WIN32
    changeText(FOREGROUND_INTENSITY);
    std::cout << std::setw(100) << " "
              << "Distance traveled: " << traveledDistanceTSP;
    resetText();
    std::cout << "km | ";
    changeText(FOREGROUND_INTENSITY);
    std::cout << "Travel duration: " << elapsedDurationTSP;
    resetText();
    std::cout << "min\n";
#elif __linux__
    std::cout << std::setw(100) << " "
              << "\033[1m"
              << "Distance traveled: "
              << "\033[0m" << traveledDistanceTSP << "km | "
              << "\033[1m"
              << "Travel duration: "
              << "\033[0m" << elapsedDurationTSP << "min\n";
#endif
    std::cout << "\n\n"
              << std::setw(5) << " "
              << "Product_ID" << std::setw(7) << " "
              << "Product_Name" << std::setw(13) << " "
              << "Total_transport_quantity\n";
    underline(70, true);

    for (SUPPLY::SUPPLY_NODE *supply_data = supply.getHead(); supply_data != nullptr; supply_data = supply_data->next)
    {
        int supplyProductID = std::stoi(supply_data->getID());
        std::cout << std::setw(5 + 1)
                  << " [" << supplyProductID << "] "
                  << std::setw(13) << " ";

        for (PRODUCT::PRODUCT_NODE *product_data = product.getHead(); product_data != nullptr; product_data = product_data->next)
            if (std::stoi(product_data->getProductID()) == supplyProductID)
            {
                std::cout << product_data->getProductName()
                          << std::setw(maxProductNameLength - strlen(product_data->getProductName()) + 5) << " ";
                break;
            }

        std::cout << supply_data->getQuantity() << " buc.\n";
    }

    underline(70, true);
    std::cout << "\n\n";
    underline(190, false);
    std::cout << std::setw(5) << " "
              << "[1] PREVIOUS" << std::setw(80) << " -" << consolePage << "- " << std::setw(80) << " "
              << "[2] NEXT\n";
    underline(190, false);

    traveledDistanceTSP += adjacencyMatrix[minimumRouteTSP[1]][minimumRouteTSP[2]].distance;
    elapsedDurationTSP += adjacencyMatrix[minimumRouteTSP[1]][minimumRouteTSP[2]].duration;
}

void tsp::finalPageTSP(void)
{
    std::ifstream get_contor("utils/contor_TSP_log.txt");
    if (!get_contor.is_open())
    {
        std::cerr << std::setw(5) << " "
                  << "Failed to open utils file!\n";
        _getch();
    }
    else
    {
        unsigned int contor_temp;
        get_contor >> contor_temp;
        logCounter = contor_temp;
    }
    get_contor.close();

    TSP_RoutesCompleted = true;
    logCounter++;

    std::ofstream file(LOGS);
    if (!file.is_open())
    {
        std::cerr << std::setw(5) << " "
                  << "Failed to open file!\n";
        _getch();
    }
    else
        file << logCounter;

    file.close();

    if (!depot.refreshData())
        std::cerr << std::setw(5) << " "
                  << "Could not update the database!\n";
    else if (fetchTables() == EXIT_FAILURE)
        _getch();

    std::ofstream log;
    log.open(TSP_LOG, std::ios::app);

    if (!log.is_open())
    {
        std::cerr << std::setw(5) << " "
                  << "Failed to open TSP log!\n";
        _getch();
    }
    else
    {
        std::string s(500, '=');
        log << "LOG [" << logCounter << "]\n";

        for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
        {
            for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
            {
                int ID = std::stoi(city_data->getCityID());
                if (ID == minimumRouteTSP[i])
                {
                    log << city_data->getCityName();
                    if (i < TSP_RouteCounter)
                        log << " --> ";
                    break;
                }
            }
        }

        log << "\n"
            << s << "\n"
            << "Distance traveled: " << minimumDistanceCostTSP << "km\n"
            << "Cantitate totala transportata: " << totalSuppliedQuantity << "BUC.\n"
            << "Cost total: " << totalSupplyCost << "RON\nEND-LOG\n"
            << s << "\n\n\n";

        log.close();
    }

    clear_screen();

    std::cout << "\n";
    underline(50, true);

    std::cout << std::setw(5) << " "
              << "-> Aprovizionare completa!\n"
              << std::setw(5) << " "
              << "-> Baza de date reinprostpatata!\n"
              << std::setw(5) << " "
              << "-> Mai multe detalii in fisierul TSP_log.txt!\n";

    underline(50, true);

    std::cout << std::setw(5) << " "
              << "Apasati 'ENTER' pentru a va intoarce...";

    _getch();
}

void tsp::leftPageTSP(void)
{
    clear_screen();

    if (consolePage - 1 > 1)
    {
        consolePage--;

        traveledDistanceTSP -= adjacencyMatrix[minimumRouteTSP[consolePage]][minimumRouteTSP[consolePage + 1]].distance;
        elapsedDurationTSP -= adjacencyMatrix[minimumRouteTSP[consolePage]][minimumRouteTSP[consolePage + 1]].duration;

        std::cout << "\n";
        if (!minimumRouteTSP.empty())
        {
            std::cout << std::setw(5) << " "
                      << "Route length: " << minimumDistanceCostTSP << " | "
                      << "Route duration: " << minimumDurationCostTSP << "min\n"
                      << std::setw(5) << " ";

            for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
            {
                for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
                {
                    int ID = std::stoi(city_data->getCityID());

                    if (ID == minimumRouteTSP[consolePage] && i == consolePage)
                    {
#ifdef _WIN32
                        changeText(FOREGROUND_INTENSITY | COMMON_LVB_UNDERSCORE | FOREGROUND_RED | FOREGROUND_GREEN);

                        std::cout << city_data->getCityName();

                        resetText();
#elif __linux__
                        std::cout << "\033[4m"
                                  << "\033[1m" << city_data->getCityName() << "\033[0m"
                                  << " --> ";
#endif
                        if (i < TSP_RouteCounter)
                            std::cout << " --> ";
                        break;
                    }

                    if (ID == minimumRouteTSP[i] && i != consolePage)
                    {
                        std::cout << city_data->getCityName();
                        if (i < TSP_RouteCounter)
                            std::cout << " --> ";
                        break;
                    }
                }
            }
        }
        std::cout << "\n";
        underline(190, false);
        std::cout << "\n";

        for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
        {
            int ID = std::stoi(city_data->getCityID());
            if (ID == minimumRouteTSP[consolePage])
            {
                std::cout << std::setw(5) << " "
                          << "+-----------------------+\n"
                          << std::setw(8) << " [" << city_data->getCityID() << "] " << city_data->getCityName() << "\n"
                          << std::setw(5) << " "
                          << "+-----------------------+";
                break;
            }
        }

#ifdef _WIN32
        changeText(FOREGROUND_INTENSITY);
        std::cout << std::setw(100) << " "
                  << "Distance traveled: " << traveledDistanceTSP;
        resetText();
        std::cout << "km | ";
        changeText(FOREGROUND_INTENSITY);
        std::cout << "Travel duration: " << elapsedDurationTSP;
        resetText();
        std::cout << "min\n";

#elif __linux__
        std::cout << std::setw(100) << " "
                  << "\033[1m"
                  << "Distance traveled: "
                  << "\033[0m" << traveledDistanceTSP << "km | "
                  << "\033[1m"
                  << "Travel duration: "
                  << "\033[0m" << elapsedDurationTSP << "min\n";
#endif

        std::cout << "\n\n"
                  << std::setw(5) << " "
                  << "Product_ID" << std::setw(5) << " "
                  << "Product_Name" << std::setw(13) << " "
                  << "Quantity Transported/Quantity Remaining\n";
        underline(85, true);

        if (centralDepos[minimumRouteTSP[consolePage]] == false)
        {
            if (limitedStockCities[minimumRouteTSP[consolePage]] == false)
            {
#ifdef _WIN32
                changeText(FOREGROUND_INTENSITY);
                std::cout << std::setw(5) << " "
                          << "The depot has been restocked!"
                          << "\n";
                resetText();
#elif __linux__
                std::cout << std::setw(5) << " "
                          << "\033[1m"
                          << "The depot has been restocked!\n"
                          << "\033[0m";
#endif
            }
        }
        else
        {
#ifdef _WIN32
            changeText(FOREGROUND_INTENSITY);
            std::cout << std::setw(5) << " "
                      << "Central depot!"
                      << "\n";
            resetText();
#elif __linux__
            std::cout << std::setw(5) << " "
                      << "\033[1m"
                      << "Central depot!\n"
                      << "\033[0m";
#endif
        }

        underline(85, true);
        std::cout << "\n\n";
        underline(190, false);
        std::cout << std::setw(5) << " "
                  << "[1] PREVIOUS" << std::setw(80) << " -" << consolePage << "- " << std::setw(80) << " "
                  << "[2] NEXT\n";
        underline(190, false);
    }
    else
    {
        consolePage = 1;
        tsp::mainPageTSP();
    }
}

void tsp::rightPageTSP(void)
{
    clear_screen();

    if (consolePage < TSP_RouteCounter)
    {
        if (consolePage > 2)
        {
            traveledDistanceTSP += adjacencyMatrix[minimumRouteTSP[consolePage]][minimumRouteTSP[consolePage + 1]].distance;
            elapsedDurationTSP += adjacencyMatrix[minimumRouteTSP[consolePage]][minimumRouteTSP[consolePage + 1]].duration;
        }

        consolePage++;

        std::cout << "\n";
        if (!minimumRouteTSP.empty())
        {
            std::cout << std::setw(5) << " "
                      << "Route length: " << minimumDistanceCostTSP << "km | "
                      << "Route duration: " << minimumDurationCostTSP << "min\n"
                      << std::setw(5) << " ";

            for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
            {
                for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
                {
                    int ID = std::stoi(city_data->getCityID());

                    if (ID == minimumRouteTSP[consolePage] && i == consolePage)
                    {
#ifdef _WIN32
                        changeText(FOREGROUND_INTENSITY | COMMON_LVB_UNDERSCORE | FOREGROUND_RED | FOREGROUND_GREEN);

                        std::cout << city_data->getCityName();

                        resetText();
#elif __linux__
                        std::cout << "\033[4m"
                                  << "\033[1m" << city_data->getCityName() << "\033[0m"
                                  << " --> ";
#endif
                        if (i < TSP_RouteCounter)
                            std::cout << " --> ";
                        break;
                    }

                    if (ID == minimumRouteTSP[i] && i != consolePage)
                    {
                        std::cout << city_data->getCityName();
                        if (i < TSP_RouteCounter)
                            std::cout << " --> ";
                        break;
                    }
                }
            }
        }
        std::cout << "\n";
        underline(190, false);
        std::cout << "\n";

        for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
        {
            int ID = std::stoi(city_data->getCityID());
            if (ID == minimumRouteTSP[consolePage])
            {
                std::cout << std::setw(5) << " "
                          << "+-----------------------+\n"
                          << std::setw(8) << " [" << city_data->getCityID() << "] " << city_data->getCityName() << "\n"
                          << std::setw(5) << " "
                          << "+-----------------------+";
                break;
            }
        }

#ifdef _WIN32
        changeText(FOREGROUND_INTENSITY);
        std::cout << std::setw(100) << " "
                  << "Distance traveled: " << traveledDistanceTSP;
        resetText();
        std::cout << "km | ";
        changeText(FOREGROUND_INTENSITY);
        std::cout << "Travel duration: " << elapsedDurationTSP;
        resetText();
        std::cout << "min\n";

#elif __linux__
        std::cout << std::setw(100) << " "
                  << "\033[1m"
                  << "Distance traveled: "
                  << "\033[0m" << traveledDistanceTSP << "km | "
                  << "\033[1m"
                  << "Travel duration: "
                  << "\033[0m" << elapsedDurationTSP << "min\n";
#endif

        std::cout << "\n\n"
                  << std::setw(5) << " "
                  << "Product_ID" << std::setw(5) << " "
                  << "Product_Name" << std::setw(13) << " "
                  << "Quantity Transported/Quantity Remaining\n";
        underline(85, true);

        if (centralDepos[minimumRouteTSP[consolePage]] == false)
        {
            if (limitedStockCities[minimumRouteTSP[consolePage]])
            {
                for (PRODUCT::PRODUCT_NODE *product_data = product.getHead(); product_data != nullptr; product_data = product_data->next)
                {
                    int PRODUCT_ID = std::stoi(product_data->getProductID());
                    double requiredQuantity = 0.0;

                    for (DEPOT::DEPOT_NODE *depot_data = depot.getHead(); depot_data != nullptr; depot_data = depot_data->next)
                    {
                        int ID_PRODUS_DEPOZIT = std::stoi(depot_data->getProductID()), ID_DEPOZIT = std::stoi(depot_data->getCityID());
                        if (ID_DEPOZIT == minimumRouteTSP[consolePage] && !isolatedVertex[ID_DEPOZIT])
                            if (ID_PRODUS_DEPOZIT == PRODUCT_ID)
                            {
                                requiredQuantity += depot_data->getProductQuantity();
                                requiredQuantity = MAXIMUM_STOCK_VALUE - requiredQuantity;
                                totalSupplyCost += requiredQuantity * product_data->getProductPrice();
                                totalSuppliedQuantity += requiredQuantity;
                                depot_data->updateQuantity(MAXIMUM_STOCK_VALUE);

                                std::cout << std::setw(5 + 1)
                                          << " [" << depot_data->getProductID() << "] "
                                          << std::setw(11) << " "
                                          << product_data->getProductName()
                                          << std::setw(maxProductNameLength - strlen(product_data->getProductName()) + 5)
                                          << " " << requiredQuantity << " buc. /";

                                for (SUPPLY::SUPPLY_NODE *supply_data = supply.getHead(); supply_data != nullptr; supply_data = supply_data->next)
                                    if (std::stoi(supply_data->getID()) == PRODUCT_ID)
                                    {
                                        std::cout << supply_data->getQuantity() << " buc.";
                                        int currentQuantity = supply_data->getQuantity();
                                        supply_data->updateQuantity(currentQuantity - requiredQuantity);
                                        break;
                                    }

                                std::cout << "\n";
                            }
                    }
                }
            }
            else
            {
#ifdef _WIN32
                changeText(FOREGROUND_INTENSITY);
                std::cout << std::setw(5) << " "
                          << "The depot has been restocked!"
                          << "\n";
                resetText();
#elif __linux__
                std::cout << std::setw(5) << " "
                          << "\033[1m"
                          << "The depot has been restocked!\n"
                          << "\033[0m";
#endif
            }
        }
        else
        {
#ifdef _WIN32
            changeText(FOREGROUND_INTENSITY);
            std::cout << std::setw(5) << " "
                      << "Central depot!"
                      << "\n";
            resetText();
#elif __linux__
            std::cout << std::setw(5) << " "
                      << "\033[1m"
                      << "Central depot!\n"
                      << "\033[0m";
#endif
        }

        underline(85, true);
        std::cout << "\n\n";
        underline(190, false);
        std::cout << std::setw(5) << " "
                  << "[1] PREVIOUS" << std::setw(80) << " -" << consolePage << "- " << std::setw(80) << " "
                  << "[2] NEXT\n";
        underline(190, false);

        limitedStockCities[minimumRouteTSP[consolePage]] = false;

        if (consolePage - 1 == 1)
        {
            traveledDistanceTSP += adjacencyMatrix[minimumRouteTSP[consolePage]][minimumRouteTSP[consolePage + 1]].distance;
            elapsedDurationTSP += adjacencyMatrix[minimumRouteTSP[consolePage]][minimumRouteTSP[consolePage + 1]].duration;
        }
    }
    else
        tsp::finalPageTSP();
}

void tsp::routeTraversalTSP(void)
{
    consolePage = 1;
    std::cout << "\n";

    bool isFound = false;

    for (unsigned int i = 0; i < limited_stock_cities_count && !isFound; i++)
        if (limitedStockCities[i] == true)
            isFound = true;

    if (!TSP_RoutesCompleted && isFound)
    {
        if (minimumRouteTSP[1] == -1)
            tsp::TSP();
        else
        {
            clear_screen();
#ifdef _WIN32
            changeText(FOREGROUND_INTENSITY);

            std::cout << std::setw(5) << " "
                      << "Route length: " << minimumDistanceCostTSP << "km | "
                      << "Route duration: " << minimumDurationCostTSP << "\n"
                      << std::setw(5) << " ";

            resetText();
#elif __linux__
            std::cout << std::setw(5) << " "
                      << "\033[1m"
                      << "Route length: " << minimumDistanceCostTSP << "km | "
                      << "Route duration: " << minimumDurationCostTSP << "\n"
                      << std::setw(5) << " "
                      << "\033[0m";
#endif
            for (unsigned int i = 1; i <= TSP_RouteCounter; i++)
                for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
                    if (std::stoi(city_data->getCityID()) == minimumRouteTSP[i])
                    {
                        std::cout << city_data->getCityName();
                        if (i < TSP_RouteCounter)
                            std::cout << " --> ";
                        break;
                    }

            std::cout << "\n";
            underline(190, false);
        }
    }
    else
    {
        clear_screen();

#ifdef _WIN32
        changeText(FOREGROUND_INTENSITY);

        std::cout << "\n\n"
                  << std::setw(5) << " "
                  << "There are no depots with limited/exhausted stock...";

        resetText();
#elif __linux__
        std::cout << "\n\n"
                  << std::setw(5) << " "
                  << "\033[1m"
                  << "There are no depots with limited/exhausted stock..."
                  << "\033[0m";
#endif
        minimumRouteTSP.clear();
        _getch();
        return;
    }

    char *input = (char *)malloc(MAXL * sizeof(char) + 1);

    if (input == NULL)
        return;

    std::cout << std::setw(5) << " "
              << "Enter '0' to return\n\n"
              << std::setw(5) << " "
              << "[S] Start: ";
    std::cin >> input;

    if (_strcasecmp_(input, "0") == 0)
    {
        free(input);
        return;
    }
    else if (_strcasecmp_(input, "s") == 0)
    {
        unsigned int MENU = 0;

        tsp::mainPageTSP();

        do
        {
            std::cout << "\n"
                      << std::setw(5) << " "
                      << "Enter menu number: ";
            speedyGo::cin >> MENU;

            switch (MENU)
            {
            case 1:
                tsp::leftPageTSP();
                break;
            case 2:
                tsp::rightPageTSP();
                break;

            default:
                break;
            }
        } while (MENU != 0 && !TSP_RoutesCompleted);
    }

    free(input);
}