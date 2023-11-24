/*
 *
 *  PRODUCT_HPP
 *  Product Data Management
 *
 *  This C++ header file defines the Product class
 *
 *  - Various methods for adding, deleting, and retrieving city data.
 *  - Functions for sorting, searching, and filtering city data.
 *
 */

#pragma once
#ifndef __PRODUCT_HPP__
#define __PRODUCT_HPP__

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

        char *getProductID(void) const noexcept;
        char *getProductName(void) const noexcept;
        char *getProductCategory(void) const noexcept;
        double getProductPrice(void) const noexcept;
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
    PRODUCT_NODE *getHead() const noexcept;
    PRODUCT_NODE *getTail() const noexcept;
    void getData(const char *Product_ID, const char *Product_Name, const char *Product_Category, const double Product_Price);
    void fetchTable(void);
    void clear(void);
    ~PRODUCT();
};

#endif // PRODUCT_HPP