#pragma once
#ifndef APROVIZIONARE_HPP
#define APROVIZIONARE_HPP

class APROVIZIONARE
{
public:
    struct NOD_APROVIZIONARE
    {
        char *ID_Produs;
        int cantitate_totala_necesara = 0;
        NOD_APROVIZIONARE *next = nullptr;
        NOD_APROVIZIONARE *prev = nullptr;

        NOD_APROVIZIONARE(char *ID_Produs, int Cantitate_totala_necesara);
        ~NOD_APROVIZIONARE();
    };

    NOD_APROVIZIONARE *head_aprovizionare = nullptr;
    NOD_APROVIZIONARE *tail_aprovizionare = nullptr;

public:
    NOD_APROVIZIONARE *getHead();
    NOD_APROVIZIONARE *getTail();
    void inserareDateAprovizionare(char *ID_Produs, int Cantitate_totala_necesara);
    ~APROVIZIONARE();
};

#endif