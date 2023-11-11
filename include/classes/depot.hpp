/*
 *
 *  DEPOT_HPP
 *  Depot Data Management
 *
 *  This C++ header file defines the Depot class
 *
 *  - Defines the DEPOT class and DEPOT_NODE structure.
 *  - Methods for sorting, refreshing, and retrieving depot data.
 *
 */

#pragma once
#ifndef __DEPOT_HPP__
#define __DEPOT_HPP__

class DEPOT
{
public:
    struct DEPOT_NODE
    {
    private:
        char *Product_ID = nullptr, *City_ID = nullptr;
        double Product_Quantity = 0.0;

    public:
        DEPOT_NODE *prev = nullptr, *next = nullptr;

        DEPOT_NODE(const char *Product_ID, const char *City_ID, const double Product_Quantity);
        ~DEPOT_NODE();

        char *getProductID(void) const;
        char *getCityID(void) const;
        double getProductQuantity(void) const;
        void swapData(DEPOT_NODE &node);
        void updateQuantity(const double newQuantity);

        static void sortData(void);
        static void searchLimitedStockCities(void);
    };

    DEPOT_NODE *head_depot = nullptr, *tail_depot = nullptr;

public:
    DEPOT_NODE *getHead(void) const;
    DEPOT_NODE *getTail(void) const;
    void getData(const char *Product_ID, const char *City_ID, const double Product_Quantity);
    void fetchTable(void);
    bool refreshData(void);
    ~DEPOT();
};

#endif // DEPOT_HPP