#pragma once
#ifndef ORAS_HPP
#define ORAS_HPP

class CITY
{
public:
    struct CITY_NODE
    {
    private:
        char *City_ID = nullptr;
        char *City_Name = nullptr;
        char *City_Type = nullptr;
        double latitude = 0.0;
        double longitude = 0.0;

    public:
        CITY_NODE *prev = nullptr;
        CITY_NODE *next = nullptr;

        CITY_NODE(const char *City_ID, const char *City_Name, const char *City_Type, const double latitude, const double longitude);
        ~CITY_NODE();
        char *getCityID(void);
        char *getCityName(void);
        char *getCityType(void);
        double getLatitude(void);
        double getLongitude(void);
        void swapData(CITY_NODE &node);
    };

    CITY_NODE *head_city = nullptr;
    CITY_NODE *tail_city = nullptr;

public:
    CITY_NODE *getHead(void);
    CITY_NODE *getTail(void);
    void getData(const char *City_ID, const char *City_Name, const char *City_Type, const double latitude, const double longitude);
    void sortData(const int sortType);
    ~CITY();
};

#endif