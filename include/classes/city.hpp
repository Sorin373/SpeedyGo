/*
*
*  CITY_HPP
*  City Data Management
*
*  This C++ header file defines the City class
*
*  - Defines the CITY class and CITY_NODE structure.
*  - Various methods for adding, deleting, and retrieving city data.
*  - Functions for sorting, searching, and filtering city data.
*
*/

#pragma once
#ifndef CITY_HPP
#define CITY_HPP

class CITY
{
public:
    struct CITY_NODE
    {
    private:
        char *City_ID = nullptr, *City_Name = nullptr, *City_Type = nullptr;
        double latitude = 0.0, longitude = 0.0;

    public:
        CITY_NODE *prev = nullptr, *next = nullptr;

        CITY_NODE(const char *City_ID, const char *City_Name, const char *City_Type, const double latitude, const double longitude);
        ~CITY_NODE();
        char *getCityID(void) const;
        char *getCityName(void) const;
        char *getCityType(void) const;
        double getLatitude(void) const;
        double getLongitude(void) const;
        void swapData(CITY_NODE &node);

        static void sortData(const int sortType);
        static void alphabeticalCitySort(const int sortType);
        static void searchCityByName(void);
        static void searchCityByID(void);
        static void filterCityByCategory(void);
        static void displayCityData(void);
        static void displayIsolatedCities(void);
        static void displayUniqueRouteDepots(void);
    };

    CITY_NODE *head_city = nullptr, *tail_city = nullptr;

public:
    CITY_NODE *getHead(void) const;
    CITY_NODE *getTail(void) const;
    void getData(const char *City_ID, const char *City_Name, const char *City_Type, const double latitude, const double longitude);
    void fetchTable(void);
    static bool addCity(void);
    static bool deleteCity(void);
    ~CITY();
};

#endif // CITY_HPP