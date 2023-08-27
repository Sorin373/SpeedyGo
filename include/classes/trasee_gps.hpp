#pragma once
#ifndef TRASEE_GPS_HPP
#define TRASEE_GPS_HPP

class TRASEE_GPS
{
public:
    struct NOD_TRASEE_GPS
    {
        double distanta = 0.0;
        int durata = 0;
        char *start = nullptr;
        char *destinatie = nullptr;
        NOD_TRASEE_GPS *prev = nullptr;
        NOD_TRASEE_GPS *next = nullptr;

        NOD_TRASEE_GPS(double distanta, int durata, char *start, char *destinatie);
        ~NOD_TRASEE_GPS();
    };

    NOD_TRASEE_GPS *head_gps = nullptr;
    NOD_TRASEE_GPS *tail_gps = nullptr;

public:
    NOD_TRASEE_GPS *getHead();
    NOD_TRASEE_GPS *getTail();
    void introducere_date_trasee_gps(double distanta, int durata, char *start, char *destinatie);
    ~TRASEE_GPS();
};

#endif