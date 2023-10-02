#include "../../include/classes/product.hpp"
#include <iostream>
#include <cstring>
#include <string>

PRODUCT::PRODUCT_NODE::PRODUCT_NODE(const char *Product_ID, const char *Product_Name, const char *Product_Category, const double Product_Price)
{
    this->Product_ID = strdup(Product_ID);
    this->Product_Name = strdup(Product_Name);
    this->Product_Category = strdup(Product_Category);
    this->Product_Price = Product_Price;
    next = nullptr;
    prev = nullptr;
}

PRODUCT::PRODUCT_NODE::~PRODUCT_NODE()
{
    delete[] Product_ID;
    delete[] Product_Name;
    delete[] Product_Category;
}

PRODUCT::PRODUCT_NODE *PRODUCT::getHead()
{
    return head_product;
}

PRODUCT::PRODUCT_NODE *PRODUCT::getTail()
{
    return tail_product;
}

char *PRODUCT::PRODUCT_NODE::getProductID(void)
{
    return Product_ID;
}

char *PRODUCT::PRODUCT_NODE::getProductName(void)
{
    return Product_Name;
}

char *PRODUCT::PRODUCT_NODE::getProductCategory(void)
{
    return Product_Category;
}

double PRODUCT::PRODUCT_NODE::getProductPrice(void)
{
    return Product_Price;
}

void PRODUCT::PRODUCT_NODE::swapData(PRODUCT_NODE &node)
{
    std::swap(Product_ID, node.Product_ID);
    std::swap(Product_Name, node.Product_Name);
    std::swap(Product_Category, node.Product_Category);
    std::swap(Product_Price, node.Product_Price);
}

void PRODUCT::sortData(const int sortType)
{
    bool isSorted = true;

    PRODUCT::PRODUCT_NODE *ptr, *l_ptr = nullptr;

    if (head_product == nullptr)
        return;
    do
    {
        isSorted = true;
        ptr = head_product;
        while (ptr->next != l_ptr)
        {
            if (sortType == 1)
            {
                if (std::stoi(ptr->getProductID()) > std::stoi(ptr->next->getProductID()))
                {
                    ptr->swapData(*(ptr->next));
                    isSorted = false;
                }
            }
            else if (sortType == 2)
            {
                if (std::stoi(ptr->getProductID()) < std::stoi(ptr->next->getProductID()))
                {
                    ptr->swapData(*(ptr->next));
                    isSorted = false;
                }
            }

            ptr = ptr->next;
        }
        l_ptr = ptr;
    } while (!isSorted);
}

void PRODUCT::getData(const char *Product_ID, const char *Product_Name, const char *Product_Category, const double Product_Price)
{
    PRODUCT_NODE *newnod = new PRODUCT_NODE(Product_ID, Product_Name, Product_Category, Product_Price);

    if (head_product == nullptr)
    {
        head_product = newnod;
        tail_product = newnod;
    }
    else
    {
        tail_product->next = newnod;
        newnod->prev = tail_product;
        tail_product = newnod;
    }
}

PRODUCT::~PRODUCT()
{
    PRODUCT_NODE *ptr = head_product;

    while (ptr != nullptr)
    {
        PRODUCT_NODE *temp = ptr;
        ptr = ptr->next;
        delete temp;
    }
}