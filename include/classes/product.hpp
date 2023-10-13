#pragma once
#ifndef PRODUCT_HPP
#define PRODUCT_HPP

class PRODUCT
{
public:
    struct PRODUCT_NODE
    {
    private:
        char *Product_ID = nullptr, *Product_Name = nullptr, *Product_Category = nullptr;
        double Product_Price = 0.0;

    public:
        PRODUCT_NODE *next = nullptr, *prev = nullptr;

        PRODUCT_NODE(const char *Product_ID, const char *Product_Name, const char *Product_Category, const double Product_Price);
        ~PRODUCT_NODE();

        char *getProductID(void) const;
        char *getProductName(void) const;
        char *getProductCategory(void) const;
        double getProductPrice(void) const;
        void swapData(PRODUCT_NODE &node);

        static void sortData(const int sortType);
        static void sortDataByPrice(const int sortType);
        static void alphabeticalProductSort(const int sortType);
        static void searchProductByName(void);
        static void searchProductByID(void);
        static void filterProductByCategory(void);
        static void displayProductData(void);
    };

    PRODUCT_NODE *head_product = nullptr, *tail_product = nullptr;

public:
    PRODUCT_NODE *getHead() const;
    PRODUCT_NODE *getTail() const;
    void getData(const char *Product_ID, const char *Product_Name, const char *Product_Category, const double Product_Price);
    void fetchTable(void);
    ~PRODUCT();
};

#endif