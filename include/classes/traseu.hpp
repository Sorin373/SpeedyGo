#pragma once
#ifndef TRASEU_HPP
#define TRASEU_HPP

#include <vector>

class TRASEU
{
public:
    struct NOD_TRASEU
    {
        int start = 0, destinatie = 0;
        double distanta = 0.0;
        std::vector<int> traseu;
        NOD_TRASEU *prev = nullptr;
        NOD_TRASEU *next = nullptr;

        NOD_TRASEU(int start, int destinatie, double distanta, std::vector<int> &traseu);
        ~NOD_TRASEU();
    };

    NOD_TRASEU *head_traseu = nullptr;
    NOD_TRASEU *tail_traseu = nullptr;

public:
    NOD_TRASEU *getHead();
    NOD_TRASEU *getTail();
    void inserareDateTraseu(int start, int destinatie, double distanta, std::vector<int> &traseu);
    ~TRASEU();
};

#endif