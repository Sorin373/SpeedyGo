#pragma once
#ifndef DEPOZIT_HPP
#define DEPOZIT_HPP

class DEPOT
{
public:
    struct DEPOT_NODE
    {
    private:
        char *Product_ID = nullptr;
        char *City_ID = nullptr;
        double Product_Quantity = 0.0;

    public:
        DEPOT_NODE *prev = nullptr;
        DEPOT_NODE *next = nullptr;

        DEPOT_NODE(const char *Product_ID, const char *City_ID, const double Product_Quantity);
        ~DEPOT_NODE();

        char *getProductID(void);
        char *getCityID(void);
        double getProductQuantity(void);
        void swapData(DEPOT_NODE &node);
        void updateQuantity(const double newQuantity);
    };

    DEPOT_NODE *head_depot = nullptr;
    DEPOT_NODE *tail_depot = nullptr;

public:
    DEPOT_NODE *getHead(void);
    DEPOT_NODE *getTail(void);
    void getData(const char *Product_ID, const char *City_ID, const double Product_Quantity);
    void sortData(void);
    bool refreshData(void);
    ~DEPOT();
};

#endif