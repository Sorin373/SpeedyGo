#pragma once
#ifndef DEPOZIT_HPP
#define DEPOZIT_HPP

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
    };

    DEPOT_NODE *head_depot = nullptr, *tail_depot = nullptr;

public:
    DEPOT_NODE *getHead(void) const;
    DEPOT_NODE *getTail(void) const;
    void getData(const char *Product_ID, const char *City_ID, const double Product_Quantity);
    void fetchTable(void);
    void sortData(void);
    bool refreshData(void);
    ~DEPOT();
};

#endif