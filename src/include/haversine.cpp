#include "../../include/haversine.hpp"
#include "../../include/declarations.hpp"

#include <fstream>
#include <cmath>

double toRadians(const double degrees)
{
    return degrees * M_PI / 180.0;
}

double distanceCalculator(const double Latitude_City_1, const double Longitude_City_1, const double lat_2, const double long_2)
{
    double dLat = toRadians(lat_2 - Latitude_City_1),
           dLon = toRadians(long_2 - Longitude_City_1),
           a = sin(dLat / 2) * sin(dLat / 2) + cos(toRadians(Latitude_City_1)) * cos(toRadians(lat_2)) * sin(dLon / 2) * sin(dLon / 2),
           c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return EARTH_RADIUS_KM * c;
}

bool _HAVERSINE_INIT_(const char *path)
{
    unsigned int City_ID_1 = 0, City_ID_2 = 0;
    std::ifstream edgeFile(path);

    if (!edgeFile.is_open())
        return EXIT_FAILURE;

    while (edgeFile >> City_ID_1 >> City_ID_2)
    {
        char *City_1 = (char *)malloc(MAXL * sizeof(char) + 1), *City_2 = (char *)malloc(MAXL * sizeof(char) + 1);
        double Latitude_City_1 = 0.0, Longitude_City_1 = 0.0, Latitude_City_2 = 0.0, Longitude_City_2 = 0.0;

        for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
        {
            if (Latitude_City_1 != 0.0 && Latitude_City_2 != 0.0 && Longitude_City_1 != 0.0 && Longitude_City_2 != 0.0)
                break;

            if (std::stoi(date_oras->getCityID()) == City_ID_1)
            {
                strcpy(City_1, date_oras->getCityName());
                Latitude_City_1 = date_oras->getLatitude();
                Longitude_City_1 = date_oras->getLongitude();
            }
            else if (std::stoi(date_oras->getCityID()) == City_ID_2)
            {
                strcpy(City_2, date_oras->getCityName());
                Latitude_City_2 = date_oras->getLatitude();
                Longitude_City_2 = date_oras->getLongitude();
            }
        }

        adjacency_matrix_init.getData(distanceCalculator(Latitude_City_1, Longitude_City_1, Latitude_City_2, Longitude_City_2),
                                      false, City_1, City_2);

        free(City_1);
        free(City_2);
    }

    edgeFile.close();

    return EXIT_SUCCESS;
}