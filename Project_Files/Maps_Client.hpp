#ifndef GPS
#define GPS

#include "declarations.hpp"

using namespace std;
using namespace nlohmann;

string _GET_API_KEY_(const string& config_file_path)
{
    ifstream config_file(config_file_path);
    if (!config_file.is_open())
    {
        cerr << "Eroare: " << config_file_path << "\n";
        return nullptr;
    }

    json config_data;
    config_file >> config_data;

    string API_KEY = config_data["API_KEY"];

    return API_KEY;
}

size_t _response_data_(void *content, size_t size, size_t nmemb, string *buffer)
{
    size_t total_size = size * nmemb;
    buffer->append(static_cast<char *>(content), total_size);
    return total_size;
}

HTTP_RESPONSE _http_request_(const string &url)
{
    CURL *curl = curl_easy_init();
    if (!curl)
    {
        cerr << "Eroare!\n";
        return HTTP_RESPONSE{};
    }

    string response_body;
    long response_code = 0;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _response_data_);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);

    CURLcode res = curl_easy_perform(curl);
    
    if (res != CURLE_OK)
        cerr << "Eroare: " << curl_easy_strerror(res) << "\n";
    else
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    curl_easy_cleanup(curl);
    return HTTP_RESPONSE{response_body, response_code};
}

void _GPS_UPDATE_DATA_(void)
{
    clear_screen();

    cout << "\n\n";
 
    string confi_file_path = "config.json";                                                 // schimba adresa in functie de unde se afla local API-ul gMaps
    string API_KEY = _GET_API_KEY_(confi_file_path);

    unordered_map<string, double> distante_orase;

    ifstream file_json("distante_orase.json");
    json data;

    try
    {
        file_json >> data;
    }
    catch (json::parse_error &e)
    {
        cerr << e.what() << '\n';
    }

    for (json::iterator i = data.begin(); i != data.end(); i++)
    {
        bool success_flag = true; 

        char *pereche_orase = (char *)malloc(MAXL * sizeof(char) + 1);
        char *oras1 = (char *)malloc(MAXL * sizeof(char) + 1);
        char *oras2 = (char *)malloc(MAXL * sizeof(char) + 1);

        double lat_oras1 = 0.0, long_oras1 = 0.0;
        double lat_oras2 = 0.0, long_oras2 = 0.0;

        strcpy(pereche_orase, i.key().c_str());

        char *ptr = strtok(pereche_orase, "_");

        int ID_Oras1 = 0, ID_Oras2 = 0;
        bool nume_oras = true;

        if (ptr != NULL)
        {
            strcpy(oras1, ptr);
            ptr = strtok(NULL, "_");
        }

        if (ptr != NULL)
            strcpy(oras2, ptr);

        for (ORAS::NOD_ORAS *date_oras = oras.getHead(); date_oras != nullptr; date_oras = date_oras->next)
        {
            if (lat_oras1 != 0.0 && lat_oras2 != 0.0 && long_oras1 != 0.0 && long_oras2 != 0)
                break;

            if (strcasecmp(date_oras->denumire_oras, oras1) == 0)
            {
                lat_oras1 = date_oras->latitudine;
                long_oras1 = date_oras->longitudine;
            }

            if (strcasecmp(date_oras->denumire_oras, oras2) == 0)
            {
                lat_oras2 = date_oras->latitudine;
                long_oras2 = date_oras->longitudine;
            }
        }

        string url = "https://maps.googleapis.com/maps/api/distancematrix/json?origins=" +
                     to_string(lat_oras1) + "," + to_string(long_oras1) + "&destinations=" +
                     to_string(lat_oras2) + "," + to_string(long_oras2) + "&mode=driving&key=" +
                     API_KEY;

        HTTP_RESPONSE response = _http_request_(url);

        if (response.status_cod == 200)
        {
            cout << setw(5) << " " << oras1 << " --> " << oras2 << "\n"
                 << setw(5) << " " << lat_oras1 << "\u00B0" << " " << long_oras1 << "\u00B0" << " " 
                 << lat_oras2 << "\u00B0" << " " << long_oras2 << "\u00B0" << "\n";

            json json_data = json::parse(response.body);
            double result = json_data["rows"][0]["elements"][0]["distance"]["value"];
            string durata = json_data["rows"][0]["elements"][0]["duration"]["text"];

            char *c_duration = (char *)malloc(MAXL * sizeof(char) + 1);
            strcpy(c_duration, durata.c_str());

            int ore = 0, minute = 0;
            bool ora_f = false;

            char *ptr = strtok(c_duration, " ");
            while (ptr != nullptr)
            {
                char *endPtr;
                int val = strtol(ptr, &endPtr, 10);

                if (endPtr != ptr)
                {
                    if (!ora_f)
                    {
                        ore = val;
                        ora_f = true;
                    }
                    else
                        minute = val;
                }

                ptr = strtok(nullptr, " ");
            }

            string s_oras1(oras1);
            string s_oras2(oras2);

            result /= 1000;
            int durata_finala = ore * 60 + minute;

            distante_orase[s_oras1 + "_" + s_oras2] = result;
            distante_orase[s_oras2 + "_" + s_oras1] = result;

            if (result == 1)
                result = 0;

            if (result == 0)
                durata_finala = 0;   

            trasee_gps.introducere_date_trasee_gps(result, durata_finala, oras1, oras2);

            free(c_duration);
        }
        else
            success_flag = false;

        json updated_data;

        for (const auto &pair : distante_orase)
        {
            const string key = pair.first;
            double value = pair.second;
            updated_data[key] = value;
        }

        ofstream fout("distante_orase.json");

        string json_string = updated_data.dump(4);
        fout << json_string;

        fout.close();

        free(pereche_orase);
        free(oras1);
        free(oras2);

        if (success_flag)
            cout << setw(5) << " " << "\033[1m" << "DONE!\n" << "\033[0m";
        else
            cout << setw(5) << " " << "\033[2m" << "Nu s-au putut calcula datele!\n" << "\033[0m";

        underline(100, true);
    }
}

#endif