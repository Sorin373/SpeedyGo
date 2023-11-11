#include "../../include/GoogleMatrixAPI.hpp"
#include "../../include/declarations.hpp"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <nlohmann/json.hpp>
#include <curl/curl.h>

std::string _GET_API_KEY_(const char *path)
{
    std::ifstream config_file(path);
    std::string API_KEY = "";

    if (!config_file.is_open())
    {
        std::cerr << std::setw(6) << " "
                  << "--> Error: " << path << "\n"
                  << std::setw(5) << " "
                  << "Enter the API key ('N' to cancel): ";

        std::cin.ignore(MAXL, '\n');
        std::getline(std::cin, API_KEY);
        std::cout << "\n";

        if (API_KEY == "N")
            return "";
        else
            return API_KEY;
    }
    else
        std::cout << std::setw(5) << " "
                  << "--> File found!\n";

    nlohmann::json config_data;
    config_file >> config_data;

    API_KEY = config_data["API_KEY"];

    if (API_KEY.empty())
    {
        std::cerr << std::setw(5) << " "
                  << "--> API key not found.\n\n"
                  << std::setw(5) << " "
                  << "Enter the API key ('N' to cancel): ";

        std::cin.ignore(MAXL, '\n');
        std::getline(std::cin, API_KEY);
        std::cout << "\n";

        if (API_KEY == "N")
            return "";
    }

    return API_KEY;
}

size_t _response_data_(void *content, size_t element_size, size_t elements, std::string *buffer)
{
    size_t total_size = element_size * elements;
    buffer->append((char *)(content), total_size);
    return total_size;
}

HTTP_RESPONSE _http_request_(const std::string &url)
{
    CURL *curl = curl_easy_init();
    
    std::string response_body = "";
    long response_code = 0;

    if (!curl)
    {
        std::cerr << std::setw(5) << " "
                  << "Failed to initialize Curl!\n";
        return HTTP_RESPONSE{};
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _response_data_);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK)
        std::cerr << curl_easy_strerror(res) << "\n";
    else
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    curl_easy_cleanup(curl);
    return HTTP_RESPONSE{response_body, response_code};
}

bool _GOOGLE_MATRIX_API_INIT_(void)
{
    std::cout << "\n";

    char *input = (char *)malloc(MAXL * sizeof(char) + 1);

    if (input == NULL)
        return EXIT_FAILURE;

    if (!_ENABLE_API)
    {
        std::cout << std::setw(5) << " "
                  << "-- Do you want to use Google services to calculate distances between cities? [Y]/[N]: ";
        std::cin >> input;

        std::cout << "\n";

        if (_strcasecmp_(input, "N") == 0)
        {
            free(input);
            return EXIT_FAILURE;
        }
    }

    if (_strcasecmp_(input, "Y") == 0 || _ENABLE_API)
    {
        _ENABLE_API = true;

        std::string API_KEY = _GET_API_KEY_(API);

        if (API_KEY.empty())
        {
            std::cerr << std::setw(5) << " "
                      << "--> API key not found.\n\n"
                      << std::setw(5) << " "
                      << "Check 'config.json' (do not change the name of the file) or the file path to be 'src/utils' and to contain a valid key.\n"
                      << std::setw(5) << " "
                      << "The app will still run correctly but the distances will not be calculated using the Google Matrix API\n"
                      << std::setw(5) << " "
                      << "Press 'ENTER' to continue...";
            _getch();
            std::cout << "\n\n";

            return EXIT_FAILURE;
        }

        underline(65, true);

        std::ifstream edgeFile(EDGES);

        if (!edgeFile.is_open())
        {
            _ENABLE_API = false;
            std::cerr << std::setw(5) << " "
                      << "Failed to open file!\n";
            _getch();
            return EXIT_FAILURE;
        }
        else
        {
            unsigned int City_ID_1 = 0, City_ID_2 = 0;
            bool success_flag = true;

            while (edgeFile >> City_ID_1 >> City_ID_2)
            {
                char *City_1 = (char *)malloc(MAXL * sizeof(char) + 1); 

                if (City_1 == NULL)
                    return EXIT_FAILURE;

                char *City_2 = (char *)malloc(MAXL * sizeof(char) + 1);

                if (City_2 == NULL)
                {
                    free(City_1);
                    return EXIT_FAILURE;
                }

                double Latitude_City_1 = 0.0, Longitude_City_1 = 0.0, Latitude_City_2 = 0.0, Longitude_City_2 = 0.0;

                for (CITY::CITY_NODE *city_data = city.getHead(); city_data != nullptr; city_data = city_data->next)
                {
                    if (Latitude_City_1 != 0.0 && Latitude_City_2 != 0.0 && Longitude_City_1 != 0.0 && Longitude_City_2 != 0.0)
                        break;

                    if (std::stoi(city_data->getCityID()) == City_ID_1)
                    {
                        strcpy(City_1, city_data->getCityName());
                        Latitude_City_1 = city_data->getLatitude();
                        Longitude_City_1 = city_data->getLongitude();
                    }
                    else if (std::stoi(city_data->getCityID()) == City_ID_2)
                    {
                        strcpy(City_2, city_data->getCityName());
                        Latitude_City_2 = city_data->getLatitude();
                        Longitude_City_2 = city_data->getLongitude();
                    }
                }

                std::string url = "https://maps.googleapis.com/maps/api/distancematrix/json?origins=" +
                                  std::to_string(Latitude_City_1) + "," + std::to_string(Longitude_City_1) + "&destinations=" +
                                  std::to_string(Latitude_City_2) + "," + std::to_string(Longitude_City_2) + "&mode=driving&key=" +
                                  API_KEY;

                HTTP_RESPONSE response = _http_request_(url);

                if (response.status_code == 200)
                {
                    nlohmann::json json_data = nlohmann::json::parse(response.body);
                    std::string status_str = json_data["status"];

                    if (status_str.compare("REQUEST_DENIED") != 0)
                    {
                        std::cout << "\n"
                                  << std::setw(5) << " " << City_1 << " --> " << City_2 << "\n"
                                  << std::setw(5) << " " << Latitude_City_1;
#ifdef _WIN32
                        std::cout << "\370";
#elif __linux__
                        std::cout << "\u00B0";
#endif
                        std::cout << Longitude_City_1;
#ifdef _WIN32
                        std::cout << "\370"
                                  << " ";
#elif __linux__
                        std::cout << "\u00B0"
                                  << " ";
#endif
                        std::cout << Latitude_City_2;
#ifdef _WIN32
                        std::cout << "\370";
#elif __linux__
                        std::cout << "\u00B0"
                                  << " ";
#endif
                        std::cout << Longitude_City_2;
#ifdef _WIN32
                        std::cout << "\370"
                                  << "\n";
#elif __linux__
                        std::cout << "\u00B0"
                                  << "\n";
#endif
                        double distance = json_data["rows"][0]["elements"][0]["distance"]["value"];
                        int duration = json_data["rows"][0]["elements"][0]["duration"]["value"];

                        distance /= 1000;
                        duration /= 60;

                        if (distance == 1)
                            distance = 0;

                        if (distance == 0)
                            duration = 0;

                        adjacency_matrix_init.getData(distance, duration, City_1, City_2);
                    }
                    else
                    {
                        _ENABLE_API = false;

                        std::cerr << std::setw(5) << " "
                                  << "--> API key not valid.\n\n"
                                  << std::setw(5) << " "
                                  << "Check 'config.json' (do not change the name of the file) or the file path to be 'src/utils' and to contain a valid key.\n"
                                  << std::setw(5) << " "
                                  << "The app will still run correctly but the distances will not be calculated using the Google Matrix API.\n"
                                  << std::setw(5) << " "
                                  << "Press 'ENTER' to continue...";
                        _getch();
                        std::cout << "\n\n";

                        free(City_1);
                        free(City_2);

                        return EXIT_FAILURE;
                    }
                }
                else
                {
                    _ENABLE_API = false;

                    free(City_1);
                    free(City_2);

                    return EXIT_FAILURE;
                }
#ifdef _WIN32
                changeText(FOREGROUND_GREEN);
                std::cout << std::setw(5) << " "
                          << "Successful...\n";
                resetText();
#elif __linux__
                std::cout << std::setw(5) << " "
                          << "\033[1m"
                          << "Successful...\n"
                          << "\033[0m";
#endif

                free(City_1);
                free(City_2);
            }

            free(input);
        }

        edgeFile.close();
    }

    underline(65, true);

    return EXIT_SUCCESS;
}