#include "../include/haversine.hpp"
#include "../include/declarations.hpp"
#include <fstream>
#include <string>
#include <cmath>
#include <cstring>

using std::stoi;

double toRadians(const double degrees)
{
    return degrees * M_PI / 180.0;
}

double calculare_distante(const double lat_1, const double long_1, const double lat_2, const double long_2)
{
    double dLat = toRadians(lat_2 - lat_1),
           dLon = toRadians(long_2 - long_1),
           a = sin(dLat / 2) * sin(dLat / 2) + cos(toRadians(lat_1)) * cos(toRadians(lat_2)) * sin(dLon / 2) * sin(dLon / 2),
           c = 2 * atan2(sqrt(a), sqrt(1 - a));
           
    return EARTH_RADIUS_KM * c;
}

bool load_data(const char *path)
{
    std::ifstream fisier(path);

    if (!fisier.is_open())
        return EXIT_FAILURE;

    unsigned int ID_1 = 0, ID_2 = 0;
    while (fisier >> ID_1 >> ID_2)
    {
        char *oras1 = (char *)malloc(MAXL * sizeof(char) + 1);
        char *oras2 = (char *)malloc(MAXL * sizeof(char) + 1);

        double lat_oras1 = 0.0, long_oras1 = 0.0;
        double lat_oras2 = 0.0, long_oras2 = 0.0;

        for (CITY::CITY_NODE *date_oras = city.getHead(); date_oras != nullptr; date_oras = date_oras->next)
        {
            if (lat_oras1 != 0.0 && lat_oras2 != 0.0 && long_oras1 != 0.0 && long_oras2 != 0.0)
                break;

            unsigned int temp_ID = stoi(date_oras->getCityID());

            if (temp_ID == ID_1)
            {
                strcpy(oras1, date_oras->getCityName());
                lat_oras1 = date_oras->getLatitude();
                long_oras1 = date_oras->getLongitude();
            }
            else if (temp_ID == ID_2)
            {
                strcpy(oras2, date_oras->getCityName());
                lat_oras2 = date_oras->getLatitude();
                long_oras2 = date_oras->getLongitude();
            }
        }

        adjacency_matrix_init.getData(calculare_distante(lat_oras1, long_oras1, lat_oras2, long_oras2),
                                               false, oras1, oras2);

        free(oras1);
        free(oras2);
    }
    fisier.close();

    return EXIT_SUCCESS;
}