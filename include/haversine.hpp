/*
*
* HAVERSINE_HPP
* Haversine formula implementation
*
* This is a simple, but not reliable solution for people who do not have an API key
* For more information on the Haversine formula: (https://github.com/sorin373/SpeedyGo#the-haversine-formula)
*
*/

#pragma once
#ifndef HAVERSINE_HPP
#define HAVERSINE_HPP

double toRadians(const double degrees);
double distanceCalculator(const double Latitude_City_1, const double Longitude_City_1, const double lat_2, const double long_2);
bool   _HAVERSINE_INIT_(const char *path);

#endif // HAVERSINE_HPP