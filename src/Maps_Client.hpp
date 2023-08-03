#ifndef GPS
#define GPS

#include "declarations.hpp"

using namespace std;
using namespace nlohmann;

#pragma region GOOGLE_DISTANCE_MATRIX_API
string _GET_API_KEY_(const string &config_file_path)
{
    ifstream config_file(config_file_path);
    if (!config_file.is_open())
    {
        cerr << "Eroare: " << config_file_path << "\n";
        return nullptr;
    }
    else
        cout << "\n"
             << setw(5) << " "
             << "File found!\n";

    json config_data;
    config_file >> config_data;

    string API_KEY = config_data["API_KEY"];

    return API_KEY;
}

size_t _response_data_(void *content, size_t element_size, size_t elements, string *buffer)
{
    size_t total_size = element_size * elements;
    buffer->append(static_cast<char *>(content), total_size);
    return total_size;
}

HTTP_RESPONSE _http_request_(const string &url)
{
    CURL *curl = curl_easy_init();
    if (!curl)
    {
        cerr << setw(5) << " " << "Failed to initialize Curl!\n";
        return HTTP_RESPONSE{};
    }

    string response_body;
    long response_code = 0;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _response_data_);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK)
        cerr << curl_easy_strerror(res) << "\n";
    else
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    curl_easy_cleanup(curl);
    return HTTP_RESPONSE{response_body, response_code};
}
#pragma endregion

bool _GPS_UPDATE_DATA_(void)
{
    clear_screen();

    cout << "\n\n";

    char *input = (char *)malloc(MAXL * sizeof(char) + 1);
    char config_file_path[MAXL] = "utils/config.json"; // schimba adresa in functie de unde se afla local API-ul gMaps

    if (!use_API)
        cout << setw(5) << " "
            << "Daca doriti sa folositi servicile Google pentru a calcula distantele intre orase\n"
            << setw(5) << " "
            << "introduceti 1 si 0 in caz contrar!\n";

    underline(85, true);

    cout << setw(5) << " "
         << "I: ";
    cin >> input;

    if (strcasecmp(input, "0") == 0)
    {
        free(input);
        return EXIT_FAILURE;
    }
    else if (strcasecmp(input, "1") == 0)
    {
        clear_screen();

        cout << "\n\n" << setw(5) << " "
             << "Introduceti numele fisierului unde se afla API-ul si '0' pentru a anula:\n";

        underline(85, true);

        cout << setw(5) << " "
             << "I: ";

        cin >> config_file_path;

        if (strcasecmp(config_file_path, "0") == 0)
        {
            free(input);
            return EXIT_FAILURE;
        }

        string API_KEY = _GET_API_KEY_(config_file_path);

        if (API_KEY.empty())
            return EXIT_FAILURE;

        use_API = true;

        unordered_map<string, double> distante_orase;

        ifstream legaturi("utils/legaturi.txt");

        if (!legaturi.is_open())
        {
            cerr << setw(5) << " "
                 << "Failed to open file!\n";
            getch();
            return EXIT_FAILURE;
        }
        else
        {
            unsigned int ID_1 = 0, ID_2 = 0;
            bool success_flag = true;

            while (legaturi >> ID_1 >> ID_2)
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

                string url = "https://maps.googleapis.com/maps/api/distancematrix/json?origins=" +
                             to_string(lat_oras1) + "," + to_string(long_oras1) + "&destinations=" +
                             to_string(lat_oras2) + "," + to_string(long_oras2) + "&mode=driving&key=" +
                             API_KEY;

                HTTP_RESPONSE response = _http_request_(url);

                if (response.status_cod == 200)
                {
                    cout << setw(5) << " " << oras1 << " --> " << oras2 << "\n"
                         << setw(5) << " " << lat_oras1 << "\u00B0"
                         << " " << long_oras1 << "\u00B0"
                         << " "
                         << lat_oras2 << "\u00B0"
                         << " " << long_oras2 << "\u00B0"
                         << "\n";

                    json json_data = json::parse(response.body);

                    string status_str = json_data["status"];
                    char status[20];
                    strncpy(status, status_str.c_str(), sizeof(status) - 1);
                    status[sizeof(status) - 1] = '\0';
                    status_str.clear();

                    if (strcasecmp(status, "REQUEST_DENIED") != 0)
                    {
                        double result = json_data["rows"][0]["elements"][0]["distance"]["value"];
                        int durata = json_data["rows"][0]["elements"][0]["duration"]["value"];

                        string s_oras1(oras1);
                        string s_oras2(oras2);

                        result /= 1000;
                        int durata_finala = durata / 60;

                        distante_orase[s_oras1 + "_" + s_oras2] = result;
                        distante_orase[s_oras2 + "_" + s_oras1] = result;

                        if (result == 1)
                            result = 0;

                        if (result == 0)
                            durata_finala = 0;

                        trasee_gps.introducere_date_trasee_gps(result, durata_finala, oras1, oras2);
                    }
                    else
                    {
                        free(oras1);
                        free(oras2);
                        return EXIT_FAILURE;
                    }
                }
                else
                {
                    free(oras1);
                    free(oras2);
                    return EXIT_FAILURE;
                }
                    
                json updated_data;

                for (unordered_map<string, double>::const_iterator i = distante_orase.begin(); i != distante_orase.end(); i++)
                {
                    const pair<const string, double> &pair = *i;
                    const string &key = pair.first;
                    double value = pair.second;
                    updated_data[key] = value;
                }

                ofstream fout("utils/distante_orase.json");

                string json_string = updated_data.dump(4);
                fout << json_string;

                fout.close();

                cout << setw(5) << " "
                     << "\033[1m"
                     << "DONE!\n"
                     << "\033[0m";

                underline(100, true);

                free(oras1);
                free(oras2);
            }
            
            free(input);
        }
    }
    return EXIT_SUCCESS;
}

#endif