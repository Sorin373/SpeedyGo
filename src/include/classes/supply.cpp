#include "../../../include/classes/supply.hpp"

#include <stdlib.h>
#include <cstring>

SUPPLY::SUPPLY_NODE::SUPPLY_NODE(char *Product_ID, int Cantitate_totala_necesara)
{
    this->Product_ID = strdup(Product_ID);
    this->total_necessary_quantity = Cantitate_totala_necesara;
    this->next = nullptr;
    this->prev = nullptr;
}

SUPPLY::SUPPLY_NODE::~SUPPLY_NODE(void)
{
    free(Product_ID);
}

char *SUPPLY::SUPPLY_NODE::getID(void) const noexcept
{
    return Product_ID;
}

int SUPPLY::SUPPLY_NODE::getQuantity(void) const noexcept
{
    return total_necessary_quantity;
}

void SUPPLY::SUPPLY_NODE::updateQuantity(const int newQuantity)
{
    total_necessary_quantity = newQuantity;
}

SUPPLY::SUPPLY_NODE *SUPPLY::getHead(void) const noexcept
{
    return head_supply;
}

SUPPLY::SUPPLY_NODE *SUPPLY::getTail(void) const noexcept
{
    return tail_supply;
}

void SUPPLY::getData(char *Product_ID, int Cantitate_totala_necesara)
{
    SUPPLY_NODE *newnod = new SUPPLY_NODE(Product_ID, Cantitate_totala_necesara);

    if (head_supply == nullptr)
    {
        head_supply = newnod;
        tail_supply = newnod;
    }
    else
    {
        tail_supply->next = newnod;
        newnod->prev = tail_supply;
        tail_supply = newnod;
    }
}

SUPPLY::~SUPPLY(void)
{
    SUPPLY_NODE *ptr = head_supply;

    while (ptr != nullptr)
    {
        SUPPLY_NODE *temp = ptr;
        ptr = ptr->next;
        delete temp;
    }
}