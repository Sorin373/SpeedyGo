#include "../../include/classes/product.hpp"
#include "../../include/declarations.hpp"
#include "../../include/database.hpp"
#include "../../include/logic.hpp"

#include <iostream>
#include <iomanip>
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

void PRODUCT::PRODUCT_NODE::searchProductByName(void)
{
    clear_screen();
    afisare_date_tabel_produs();

    char *UserInput_Product_Name = (char *)malloc(MAXL * sizeof(char) + 1);
    bool isFound = false;

#ifdef _WIN32
    changeText(FOREGROUND_INTENSITY);
    std::cout << std::setw(5) << " "
              << "Enter '0' to return\n\n";
    resetText();
    std::cout << std::setw(5) << " "
              << "Enter the product name: ";
#elif __linux__
    std::cout << setw(5) << " "
              << "\033[3m"
              << "Enter '0' to return\n\n"
              << "\033[0m" << setw(5) << " "
              << "Enter the product name: ";
#endif

    std::cin.get();
    std::cin.get(UserInput_Product_Name, MAXL);

    if (_strcasecmp_(UserInput_Product_Name, "0") == 0)
    {
        free(UserInput_Product_Name);
        return;
    }

    clear_screen();

    std::cout << "\n\n"
              << std::setw(5) << " "
              << "+---------------+\n"
              << std::setw(5) << " "
              << "| PRODUCT TABLE |\n"
              << std::setw(5) << " "
              << "+---------------+\n\n";

    std::cout << std::setw(5) << " "
              << "Product_ID"
              << std::setw(5) << " "
              << "Product_Name"
              << std::setw(cmax_denumire_produse - 10) << " "
              << "Product_Category"
              << std::setw(5) << " "
              << "Product_Price\n";

    underline(80, true);

    for (PRODUCT::PRODUCT_NODE *product_data = product.getHead(); product_data != nullptr; product_data = product_data->next)
        if (_strcasecmp_(product_data->Product_Name, UserInput_Product_Name) == 0)
        {
            isFound = true;

            std::cout << std::setw(5 + 1)
                      << " [" << product_data->Product_ID << "]"
                      << std::setw(cmax_ID_produs - strlen(product_data->Product_ID) + 9)
                      << " " << product_data->Product_Name
                      << std::setw(cmax_denumire_produse - strlen(product_data->Product_Name) + 5)
                      << " " << product_data->Product_Category
                      << std::setw(cmax_categorie_produse - strlen(product_data->Product_Category) + 10) << " ";

            char *Temp_Product_Price = (char *)malloc(MAXL * sizeof(char) + 1);
            snprintf(Temp_Product_Price, MAXL, "%g", product_data->Product_Price);

            std::cout << std::fixed << std::setprecision(2)
                      << product_data->Product_Price
                      << std::setw(cmax_pret_produse - std::to_string(round(product_data->Product_Price)).length() + 10)
                      << "RON\n";

            free(Temp_Product_Price);
            break;
        }

    underline(80, true);

    if (!isFound)
    {
        clear_screen();

        std::cout << "\n\n"
                  << std::setw(5) << " "
                  << "Invalid user input!";

        free(UserInput_Product_Name);
        _getch();
        searchProductByName();
    }
    else
    {
        std::cout << "\n"
                  << std::setw(5) << " "
                  << "Press 'ENTER' to return...";

        _getch();
        free(UserInput_Product_Name);
        searchProductByName();
    }
}

void PRODUCT::PRODUCT_NODE::searchProductByID(void)
{
    clear_screen();
    afisare_date_tabel_produs();

    char *UserInput_Product_ID = (char *)malloc(MAXL * sizeof(char) + 1);
    bool isFound = false;

#ifdef _WIN32
    changeText(FOREGROUND_INTENSITY);
    std::cout << std::setw(5) << " "
              << "Type 'EXIT' to return\n\n";
    resetText();
    std::cout << std::setw(5) << " "
              << "Enter the product ID: ";
#elif __linux__
    std::cout << setw(5) << " "
              << "\033[3m"
              << "Type 'EXIT' to return\n\n"
              << "\033[0m" << setw(5) << " "
              << "Enter the product ID: ";
#endif
    std::cin >> UserInput_Product_ID;

    clear_screen();

    if (_strcasecmp_(UserInput_Product_ID, "exit") == 0)
    {
        free(UserInput_Product_ID);
        return;
    }

    std::cout << "\n\n"
              << std::setw(5) << " "
              << "+---------------+\n"
              << std::setw(5) << " "
              << "| PRODUCT TABLE |\n"
              << std::setw(5) << " "
              << "+---------------+\n\n";

    std::cout << std::setw(5) << " "
              << "Product_ID"
              << std::setw(5) << " "
              << "Product_Name"
              << std::setw(cmax_denumire_produse - 10) << " "
              << "Product_Category"
              << std::setw(5) << " "
              << "Product_Price\n";

    underline(80, true);

    for (PRODUCT::PRODUCT_NODE *product_data = product.getHead(); product_data != nullptr; product_data = product_data->next)
        if (_strcasecmp_(product_data->Product_ID, UserInput_Product_ID) == 0)
        {
            isFound = true;

            std::cout << std::setw(5 + 1)
                      << " [" << product_data->Product_ID << "]"
                      << std::setw(cmax_ID_produs - strlen(product_data->Product_ID) + 9)
                      << " " << product_data->Product_Name
                      << std::setw(cmax_denumire_produse - strlen(product_data->Product_Name) + 5)
                      << " " << product_data->Product_Category
                      << std::setw(cmax_categorie_produse - strlen(product_data->Product_Category) + 10) << " ";

            char *Temp_Product_Price = (char *)malloc(MAXL * sizeof(char) + 1);
            snprintf(Temp_Product_Price, MAXL, "%g", product_data->Product_Price);

            std::cout << std::fixed << std::setprecision(2)
                      << product_data->Product_Price
                      << std::setw(cmax_pret_produse - std::to_string(round(product_data->Product_Price)).length() + 10)
                      << "RON\n";

            free(Temp_Product_Price);
            break;
        }

    underline(80, true);

    if (!isFound)
    {
        clear_screen();

        std::cout << "\n\n"
                  << std::setw(5) << " "
                  << "Invalid user input!";

        free(UserInput_Product_ID);
        _getch();
        searchProductByID();
    }
    else
    {
        std::cout << "\n"
                  << std::setw(5) << " "
                  << "Press 'ENTER' to return...";

        _getch();
        free(UserInput_Product_ID);
        searchProductByID();
    }
}

void PRODUCT::PRODUCT_NODE::swapData(PRODUCT_NODE &node)
{
    std::swap(Product_ID, node.Product_ID);
    std::swap(Product_Name, node.Product_Name);
    std::swap(Product_Category, node.Product_Category);
    std::swap(Product_Price, node.Product_Price);
}

void PRODUCT::PRODUCT_NODE::sortData(const int sortType)
{
    bool isSorted = true;

    PRODUCT::PRODUCT_NODE *ptr = nullptr, *l_ptr = nullptr;

    if (product.getHead() == nullptr)
        return;
    do
    {
        isSorted = true;
        ptr = product.getHead();
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

void PRODUCT::PRODUCT_NODE::sortDataByPrice(const int sortType)
{
    clear_screen();

    std::cout << "\n\n"
              << std::setw(5) << " "
              << "+---------------+\n"
              << std::setw(5) << " "
              << "| PRODUCT TABLE |\n"
              << std::setw(5) << " "
              << "+---------------+\n\n";

    std::cout << std::setw(5) << " "
              << "Product_ID"
              << std::setw(5) << " "
              << "Product_Name"
              << std::setw(cmax_denumire_produse - 10) << " "
              << "Product_Category"
              << std::setw(5) << " "
              << "Product_Price\n";

    underline(85, true);

    bool isSorted = true;

    PRODUCT::PRODUCT_NODE *ptr = nullptr, *l_ptr = nullptr;

    if (product.getHead() == nullptr)
        return;

    do
    {
        isSorted = true;
        ptr = product.getHead();

        while (ptr->next != l_ptr)
        {
            if (sortType == 1)
            {
                if (ptr->getProductPrice() > ptr->next->getProductPrice())
                {
                    ptr->swapData(*(ptr->next));
                    isSorted = false;
                }
            }
            else if (sortType == 2)
            {
                if (ptr->getProductPrice() < ptr->next->getProductPrice())
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

void PRODUCT::PRODUCT_NODE::alphabeticalProductSort(const int sortType)
{
    clear_screen();

    std::cout << "\n\n"
              << std::setw(5) << " "
              << "+---------------+\n"
              << std::setw(5) << " "
              << "| PRODUCT TABLE |\n"
              << std::setw(5) << " "
              << "+---------------+\n\n";

    std::cout << std::setw(5) << " "
              << "Product_ID"
              << std::setw(5) << " "
              << "Product_Name"
              << std::setw(cmax_denumire_produse - 10) << " "
              << "Product_Category"
              << std::setw(5) << " "
              << "Product_Price\n";

    underline(85, true);

    bool isSorted = true;

    PRODUCT::PRODUCT_NODE *ptr = nullptr, *l_ptr = nullptr;

    if (product.getHead() == nullptr)
        return;

    do
    {
        isSorted = true;
        ptr = product.getHead();

        while (ptr->next != l_ptr)
        {
            if (sortType == 1)
            {
                if (strcmp(ptr->getProductName(), ptr->next->getProductName()) > 0)
                {

                    ptr->PRODUCT::PRODUCT_NODE::swapData(*(ptr->next));
                    isSorted = false;
                }
            }
            else if (sortType == 2)
            {
                if (strcmp(ptr->getProductName(), ptr->next->getProductName()) < 0)
                {
                    ptr->PRODUCT::PRODUCT_NODE::swapData(*(ptr->next));
                    isSorted = false;
                }
            }
            ptr = ptr->next;
        }
        l_ptr = ptr;
    } while (!isSorted);
}

void PRODUCT::PRODUCT_NODE::filterProductByCategory(void)
{
    clear_screen();

    afisare_date_tabel_produs();

    char *UserInput_Product_Category = (char *)malloc(MAXL * sizeof(char) + 1);

#ifdef _WIN32
    changeText(FOREGROUND_INTENSITY);
    std::cout << std::setw(5) << " "
              << "Enter '0' to return\n\n";
    resetText();
    std::cout << std::setw(5) << " "
              << "Enter the product category: ";
#elif __linux__
    std::cout << setw(5) << " "
              << "\033[3m"
              << "Enter '0' to return\n\n"
              << "\033[0m" << setw(5) << " "
              << "Enter the product category: ";
#endif
    std::cin >> UserInput_Product_Category;

    if (_strcasecmp_(UserInput_Product_Category, "0") == 0)
    {
        free(UserInput_Product_Category);
        return;
    }

    clear_screen();

    std::cout << "\n\n"
              << std::setw(5) << " "
              << "+---------------+\n"
              << std::setw(5) << " "
              << "| PRODUCT TABLE |\n"
              << std::setw(5) << " "
              << "+---------------+\n\n";

    std::cout << std::setw(5) << " "
              << "Product_ID"
              << std::setw(5) << " "
              << "Product_Name"
              << std::setw(cmax_denumire_produse - 10) << " "
              << "Product_Category"
              << std::setw(5) << " "
              << "Product_Price\n";

    underline(80, true);

    for (PRODUCT::PRODUCT_NODE *product_data = product.getHead(); product_data != nullptr; product_data = product_data->next)
        if (_strcasecmp_(product_data->Product_Category, UserInput_Product_Category) == 0)
        {
            std::cout << std::setw(5 + 1)
                      << " [" << product_data->Product_ID << "]"
                      << std::setw(cmax_ID_produs - strlen(product_data->Product_ID) + 9)
                      << " " << product_data->Product_Name
                      << std::setw(cmax_denumire_produse - strlen(product_data->Product_Name) + 5)
                      << " " << product_data->Product_Category
                      << std::setw(cmax_categorie_produse - strlen(product_data->Product_Category) + 10) << " ";

            char *pret = (char *)malloc(MAXL * sizeof(char) + 1);
            snprintf(pret, MAXL, "%g", product_data->Product_Price);

            std::cout << std::fixed << std::setprecision(2)
                      << product_data->Product_Price
                      << std::setw(cmax_pret_produse - std::to_string(round(product_data->Product_Price)).length() + 10)
                      << "RON\n";

            free(pret);
        }

    underline(80, true);

    std::cout << "\n\n"
              << std::setw(5) << " "
              << "Press 'ENTER' to return...";

    free(UserInput_Product_Category);
    _getch();
    filterProductByCategory();
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

void PRODUCT::fetchTable(void)
{
    sql::Statement *stmt = nullptr;
    sql::ResultSet *res = nullptr;

    stmt = con->createStatement();
    res = stmt->executeQuery("SELECT * FROM produs");

    while (res->next())
    {
        sql::SQLString sqlstr;

        int iProduct_ID = res->getInt("ID_Produs");
        char *cProduct_ID = (char *)malloc(std::to_string(iProduct_ID).length() + 1);
        strcpy(cProduct_ID, std::to_string(iProduct_ID).c_str());

        sqlstr = res->getString("Denumire_Produs");
        char *Product_Name = (char *)malloc(sqlstr.asStdString().length() + 1);
        strcpy(Product_Name, sqlstr.asStdString().c_str());

        sqlstr = res->getString("Categorie_Produs");
        char *Product_Category = (char *)malloc(sqlstr.asStdString().length() + 1);
        strcpy(Product_Category, sqlstr.asStdString().c_str());

        double Product_Price = res->getDouble("Pret_Produs");

        product.getData(cProduct_ID, Product_Name, Product_Category, Product_Price);

        free(cProduct_ID);
        free(Product_Name);
        free(Product_Category);
    }

    res->close();
    stmt->close();
    delete res;
    delete stmt;
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

    head_product = nullptr;
    tail_product = nullptr;
}