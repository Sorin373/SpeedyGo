#include "../../include/classes/depot.hpp"
#include "../../include/declarations.hpp"
#include "../../include/database.hpp"

#include <cstring>
#include <string>
#include <iostream>
#include <iomanip>

DEPOT::DEPOT_NODE::DEPOT_NODE(const char *Product_ID, const char *City_ID, const double Product_Quantity)
{
    this->Product_ID = strdup(Product_ID);
    this->City_ID = strdup(City_ID);
    this->Product_Quantity = Product_Quantity;
    prev = nullptr;
    next = nullptr;
}

DEPOT::DEPOT_NODE::~DEPOT_NODE(void)
{
    delete[] Product_ID;
    delete[] City_ID;
}

char *DEPOT::DEPOT_NODE::getProductID(void)
{
    return Product_ID;
}

char *DEPOT::DEPOT_NODE::getCityID(void)
{
    return City_ID;
}

double DEPOT::DEPOT_NODE::getProductQuantity(void)
{
    return Product_Quantity;
}

DEPOT::DEPOT_NODE *DEPOT::getHead(void)
{
    return head_depot;
}

DEPOT::DEPOT_NODE *DEPOT::getTail(void)
{
    return tail_depot;
}

void DEPOT::getData(const char *Product_ID, const char *City_ID, const double Product_Quantity)
{
    DEPOT_NODE *newnod = new DEPOT_NODE(Product_ID, City_ID, Product_Quantity);

    if (head_depot == nullptr)
    {
        head_depot = newnod;
        tail_depot = newnod;
    }
    else
    {
        tail_depot->next = newnod;
        newnod->prev = tail_depot;
        tail_depot = newnod;
    }
}

void DEPOT::DEPOT_NODE::swapData(DEPOT_NODE &node)
{
    std::swap(Product_ID, node.Product_ID);
    std::swap(City_ID, node.City_ID);
    std::swap(Product_Quantity, node.Product_Quantity);
}

void DEPOT::sortData(void)
{
    bool isSorted = true;

    DEPOT_NODE *ptr;
    DEPOT_NODE *l_ptr = nullptr;

    if (head_depot == nullptr)
        return;

    do
    {
        isSorted = true;
        ptr = head_depot;

        while (ptr->next != l_ptr)
        {
            int id1 = std::stoi(ptr->getProductID());
            int id2 = std::stoi(ptr->next->getProductID());

            if (id1 > id2)
            {
                ptr->swapData(*(ptr->next));

                isSorted = false;
            }
            ptr = ptr->next;
        }
        l_ptr = ptr;
    } while (!isSorted);
}

void DEPOT::DEPOT_NODE::updateQuantity(const double newQuantity)
{
    Product_Quantity = newQuantity;
}

bool DEPOT::refreshData(void)
{
    try
    {
        std::string deleteQuery = "DELETE FROM depozit";
        sql::Statement *stmt = con->createStatement();

        stmt->executeUpdate(deleteQuery);
        delete stmt;

        std::string insertQuery = "INSERT INTO depozit (ID_Produs, ID_Oras, Cantitate_Produs) VALUES (?, ?, ?)";
        sql::PreparedStatement *prepStmt = con->prepareStatement(insertQuery);

        for (DEPOT::DEPOT_NODE *date_depozit = head_depot; date_depozit != nullptr; date_depozit = date_depozit->next)
        {
            int Product_ID = std::stoi(date_depozit->getProductID()),
                City_ID = std::stoi(date_depozit->getCityID()),
                Product_Quantity = date_depozit->getProductQuantity();

            prepStmt->setInt(1, Product_ID);
            prepStmt->setInt(2, City_ID);
            prepStmt->setInt(3, Product_Quantity);

            prepStmt->executeUpdate();
        }

        delete prepStmt;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "\n"
                  << std::setw(5) << " "
                  << "-- Error code: " << e.getErrorCode() << "\n"
                  << std::setw(5) << " "
                  << "-- Error message: " << e.what() << "\n"
                  << std::setw(5) << " "
                  << "-- SQLState: " << e.getSQLState() << "\n";

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

DEPOT::~DEPOT()
{
    DEPOT_NODE *ptr = head_depot;

    while (ptr != nullptr)
    {
        DEPOT_NODE *temp = ptr;
        ptr = ptr->next;
        delete temp;
    }
}