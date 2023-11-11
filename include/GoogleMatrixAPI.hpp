/*
 *
 *  GOOGLE_MATRIX_API_HPP
 *  Google Maps Distance Matrix API Client
 *
 *  This C++ header file interacts with the Google Maps Distance Matrix API to collect costs for the graph.
 *  Curl version >= 8.2.0 (https://curl.se/libcurl/c/)
 *
 */

#pragma once
#ifndef __GOOGLE_MATRIX_API_HPP__
#define __GOOGLE_MATRIX_API_HPP__

#include <string>

typedef struct
{
    std::string body;
    long status_code;
} HTTP_RESPONSE;

std::string _GET_API_KEY_(const char *path);
size_t _response_data_(void *content, size_t element_size, size_t elements, std::string *buffer); // Callback function to collect response data
HTTP_RESPONSE _http_request_(const std::string &url);                                             // Function to make an HTTP request
bool _GOOGLE_MATRIX_API_INIT_(void);

#endif // GOOGLE_MATRIX_API_HPP