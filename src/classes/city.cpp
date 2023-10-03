#include "../../include/classes/city.hpp"
#include <cstring>
#include <iostream>
#include <string>

CITY::CITY_NODE::CITY_NODE(const char *City_ID, const char *City_Name, const char *City_Type, const double latitude, const double longitude)
{
    this->City_ID = strdup(City_ID);
    this->City_Name = strdup(City_Name);
    this->City_Type = strdup(City_Type);
    this->latitude = latitude;
    this->longitude = longitude;
    next = nullptr;
    prev = nullptr;
}

CITY::CITY_NODE::~CITY_NODE(void)
{
    delete[] City_ID;
    delete[] City_Name;
    delete[] City_Type;
}

CITY::CITY_NODE *CITY::getHead(void)
{
    return head_city;
}

CITY::CITY_NODE *CITY::getTail(void)
{
    return tail_city;
}

char *CITY::CITY_NODE::getCityID(void)
{
    return City_ID;
}

char *CITY::CITY_NODE::getCityName(void)
{
    return City_Name;
}

char *CITY::CITY_NODE::getCityType(void)
{
    return City_Type;
}

double CITY::CITY_NODE::getLatitude(void)
{
    return latitude;
}

double CITY::CITY_NODE::getLongitude(void)
{
    return longitude;
}

void CITY::CITY_NODE::swapData(CITY_NODE &node)
{
    std::swap(City_ID, node.City_ID);
    std::swap(City_Name, node.City_Name);
    std::swap(City_Type, node.City_Type);
    std::swap(latitude, node.latitude);
    std::swap(longitude, node.longitude);
}

void CITY::getData(const char *City_ID, const char *City_Name, const char *City_Type, const double latitude, const double longitude)
{
    CITY_NODE *newnod = new CITY_NODE(City_ID, City_Name, City_Type, latitude, longitude);

    if (head_city == nullptr)
    {
        head_city = newnod;
        tail_city = newnod;
    }
    else
    {
        tail_city->next = newnod;
        newnod->prev = tail_city;
        tail_city = newnod;
    }
}

void CITY::sortData(const int sortType)
{
    bool isSorted = true;

    CITY::CITY_NODE *ptr;
    CITY::CITY_NODE *l_ptr = nullptr;

    if (head_city == nullptr)
        return;
    do
    {
        isSorted = true;
        ptr = head_city;

        while (ptr->next != l_ptr)
        {
            if (sortType == 1)
            {
                if (std::stoi(ptr->getCityID()) > std::stoi(ptr->next->getCityID()))
                {
                    ptr->CITY::CITY_NODE::swapData(*(ptr->next));
                    isSorted = false;
                }
            }
            else if (sortType == 2)
            {
                if (std::stoi(ptr->getCityID()) < std::stoi(ptr->next->getCityID()))
                {
                    ptr->CITY::CITY_NODE::swapData(*ptr->next);
                    isSorted = false;
                }
            }

            ptr = ptr->next;
        }
        l_ptr = ptr;
    } while (!isSorted);
}

CITY::~CITY()
{
    CITY_NODE *ptr = head_city;

    while (ptr != nullptr)
    {
        CITY_NODE *temp = ptr;
        ptr = ptr->next;
        delete temp;
    }
}