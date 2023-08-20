#ifndef GEO
#define GEO

#include "declarations.hpp"

using namespace std;

double toRadians(const double degrees)
{
    return degrees * 3.14159265358979323846 / 180.0;
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
    cout << path;
    ifstream fisier(path);

    cout << "success3\n";
    if (!fisier.is_open())
        return EXIT_FAILURE;

        cout << "success4\n";

    unsigned int ID_1 = 0, ID_2 = 0;
    while (fisier >> ID_1 >> ID_2)
    {
        char *oras1 = (char *)malloc(MAXL * sizeof(char) + 1);
        char *oras2 = (char *)malloc(MAXL * sizeof(char) + 1);

        double lat_oras1 = 0.0, long_oras1 = 0.0;
        double lat_oras2 = 0.0, long_oras2 = 0.0;

        for (ORAS::NOD_ORAS *date_oras = oras.getHead(); date_oras != nullptr; date_oras = date_oras->next)
        {
            if (lat_oras1 != 0.0 && lat_oras2 != 0.0 && long_oras1 != 0.0 && long_oras2 != 0.0)
                break;

            unsigned int temp_ID = stoi(date_oras->ID_Oras);

            if (temp_ID == ID_1)
            {
                strcpy(oras1, date_oras->denumire_oras);
                lat_oras1 = date_oras->latitudine;
                long_oras1 = date_oras->longitudine;
            }
            else if (temp_ID == ID_2)
            {
                strcpy(oras2, date_oras->denumire_oras);
                lat_oras2 = date_oras->latitudine;
                long_oras2 = date_oras->longitudine;
            }
        }

        trasee_gps.introducere_date_trasee_gps(calculare_distante(lat_oras1, long_oras1, lat_oras2, long_oras2),
                                               false, oras1, oras2);

        free(oras1);
        free(oras2);
    }
    fisier.close();

    return EXIT_SUCCESS;
}

#endif