#include "../include/GoogleMatrixAPI.hpp"
#include "../include/declarations.hpp"
#include <iostream>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <curl/curl.h>

using std::cout;
using std::cin;
using std::cerr;
using std::setw;
using std::to_string;
using std::stoi;
using std::ifstream;
using std::ofstream;
using std::string;

#pragma region GOOGLE_DISTANCE_MATRIX_API
string _GET_API_KEY_(const string &config_file_path)
{
    std::ifstream config_file(config_file_path);
    if (!config_file.is_open())
    {
        cerr << "-> Eroare: " << config_file_path << "\n";
        return nullptr;
    }
    else
        cout << "\n"
             << setw(5) << " "
             << "-> File found!\n";

    nlohmann::json config_data;
    config_file >> config_data;

    string API_KEY = config_data["API_KEY"];

    return API_KEY;
}

size_t _response_data_(void *content, size_t element_size, size_t elements, std::string *buffer)
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
        cerr << setw(5) << " "
             << "Failed to initialize Curl!\n";
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
    cout << "\n";

    char *input = (char *)malloc(MAXL * sizeof(char) + 1);

    if (!use_API)
        cout << setw(5) << " "
             << "-- Doriti sa folositi serviciile Google pentru a calcula distantele intre orase? [Y]/[N]: ";

    cin >> input;

    if (_strcasecmp_(input, "N") == 0)
    {
        free(input);
        return EXIT_FAILURE;
    }
    else if (_strcasecmp_(input, "Y") == 0)
    {
        string API_KEY = _GET_API_KEY_("utils/config.json");

        if (API_KEY.empty())
        {
            cerr << setw(5) << " "
                 << "-> API key not found.\n\n"
                 << setw(5) << " "
                 << "Check 'config.json' (do not change the name of the file) or the file path to be 'src/utils' and to contain a valid key.\n"
                 << setw(5) << " "
                 << "The app will still run correctly but the distances will not be calculated using the Google Matrix API.\n"
                 << setw(5) << " "
                 << "Rebuild the application after solving the issues or the API functionalies will not be used...";
            _getch();
            return EXIT_FAILURE;
        }

        use_API = true;

        std::unordered_map<string, double> distante_orase;

        std::ifstream legaturi("utils/legaturi.txt");

        if (!legaturi.is_open())
        {
            use_API = false;
            cerr << setw(5) << " "
                 << "Failed to open file!\n";
            _getch();
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

                    nlohmann::json json_data = nlohmann::json::parse(response.body);

                    string status_str = json_data["status"];
                    char status[20];
                    strncpy(status, status_str.c_str(), sizeof(status) - 1);
                    status[sizeof(status) - 1] = '\0';
                    status_str.clear();

                    if (_strcasecmp_(status, "REQUEST_DENIED") != 0)
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
                        use_API = false;
                        free(oras1);
                        free(oras2);
                        return EXIT_FAILURE;
                    }
                }
                else
                {
                    use_API = false;
                    free(oras1);
                    free(oras2);
                    return EXIT_FAILURE;
                }

                nlohmann::json updated_data;

                for (std::unordered_map<string, double>::const_iterator i = distante_orase.begin(); i != distante_orase.end(); i++)
                {
                    const std::pair<const string, double> &pair = *i;
                    const string &key = pair.first;
                    double value = pair.second;
                    updated_data[key] = value;
                }

                std::ofstream fout("utils/distante_orase.json");

                string json_string = updated_data.dump(4);
                fout << json_string;

                fout.close();
#ifdef _WIN32
                changeText(FOREGROUND_GREEN);
                cout << setw(5) << " "
                     << "DONE!\n";
                resetText();
#elif __linux__
                cout << setw(5) << " "
                     << "\033[1m"
                     << "DONE!\n"
                     << "\033[0m";
#endif
                underline(100, true);

                free(oras1);
                free(oras2);
            }

            free(input);
        }
    }
    return EXIT_SUCCESS;
}