/*
 *
 *  SUPPLY_HPP
 *  Restocking information
 *
 *  This C++ header file defines the Supply class which stores the information about the products which need to be restocked.
 *  See logic.cpp (void tsp::productTransportTSP(void))
 *
 */

#pragma once
#ifndef __SUPPLY_HPP__
#define __SUPPLY_HPP__

class SUPPLY
{
public:
    struct SUPPLY_NODE
    {
    private:
        char *Product_ID = nullptr;
        int total_necessary_quantity = 0;

    public:
        SUPPLY_NODE *next = nullptr, *prev = nullptr;

        SUPPLY_NODE(char *Product_ID, int total_necessary_quantity);
        ~SUPPLY_NODE();

        char *getID(void) const noexcept;
        int getQuantity(void) const noexcept;
        void updateQuantity(const int newQuantity);
    };

    SUPPLY_NODE *head_supply = nullptr, *tail_supply = nullptr;

public:
    SUPPLY_NODE *getHead(void) const noexcept;
    SUPPLY_NODE *getTail(void) const noexcept;
    void getData(char *ID_Produs, int Cantitate_totala_necesara);
    ~SUPPLY();
};

#endif // SUPPLY_HPP