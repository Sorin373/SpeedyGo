#pragma once
#ifndef HAVERSINE_HPP
#define HAVERSINE_HPP

extern double toRadians(const double degrees);
extern double calculare_distante(const double lat_1, const double long_1, const double lat_2, const double long_2);
extern bool load_data(const char *path);

#endif