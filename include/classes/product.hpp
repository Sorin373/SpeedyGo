#pragma once
#ifndef PRODUCT_HPP
#define PRODUCT_HPP

class PRODUCT
{
public:
    struct PRODUCT_NODE
    {
    private:
        char *Product_ID = nullptr;
        char *Product_Name = nullptr;
        char *Product_Category = nullptr;
        double Product_Price = 0.0;

    public:
        PRODUCT_NODE *next = nullptr;
        PRODUCT_NODE *prev = nullptr;

        PRODUCT_NODE(const char *Product_ID, const char *Product_Name, const char *Product_Category, const double Product_Price);
        ~PRODUCT_NODE();

        char *getProductID(void);
        char *getProductName(void);
        char *getProductCategory(void);
        double getProductPrice(void);
        void swapData(PRODUCT_NODE &node);
    };

    PRODUCT_NODE *head_product = nullptr;
    PRODUCT_NODE *tail_product = nullptr;

public:
    PRODUCT_NODE *getHead();
    PRODUCT_NODE *getTail();
    void getData(const char *Product_ID, const char *Product_Name, const char *Product_Category, const double Product_Price);
    void sortData(const int sortType);
    ~PRODUCT();
};

#endif