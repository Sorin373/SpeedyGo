#pragma once
#ifndef GOOGLE_MATRIX_API
#define GOOGLE_MATRIX_API

#include <string>

struct HTTP_RESPONSE
{
    std::string body;
    long status_cod;
};

std::string _GET_API_KEY_(const std::string &config_file_path);
size_t _response_data_(void *content, size_t element_size, size_t elements, std::string *buffer);
HTTP_RESPONSE _http_request_(const std::string &url);
extern bool _GPS_UPDATE_DATA_(void);

#endif