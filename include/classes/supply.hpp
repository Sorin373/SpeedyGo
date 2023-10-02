#pragma once
#ifndef SUPPLY_HPP
#define SUPPLY_HPP

class SUPPLY
{
public:
    struct SUPPLY_NODE
    {
    private:
        char *Product_ID = nullptr;
        int total_necessary_quantity = 0;

    public:
        SUPPLY_NODE *next = nullptr;
        SUPPLY_NODE *prev = nullptr;

        SUPPLY_NODE(char *Product_ID, int total_necessary_quantity);
        ~SUPPLY_NODE();

        char *getID(void);
        int getQuantity(void);
        void updateQuantity(const int newQuantity);
    };

    SUPPLY_NODE *head_supply = nullptr;
    SUPPLY_NODE *tail_supply = nullptr;

public:
    SUPPLY_NODE *getHead(void);
    SUPPLY_NODE *getTail(void);
    void getData(char *ID_Produs, int Cantitate_totala_necesara);
    ~SUPPLY();
};

#endif