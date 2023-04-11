#ifndef DEPO_DATABASE
#define DEPO_DATABASE

#include <iostream>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace sql;
using namespace std;

void accesareDate()
{
    try
    {
        Driver *driver;
        Connection *con;
        Statement *stmt;
        ResultSet *res;

        driver = get_driver_instance();
        con = driver -> connect("tcp://localhost:3306", "root", "Sorin!2005");
        con -> setSchema("MyDepoChain");

        stmt = con -> createStatement();
        res = stmt -> executeQuery("");

        while (res -> next())
        {

        }

        res = stmt -> executeQuery("");

        while (res -> next())
        {

        }

        delete res, stmt, con;
    }

    catch (sql::SQLException &e)
    {
        cout << e.what() << endl;
    }
}

#endif