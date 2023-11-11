/*
*  DATABASE_HPP
*  https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-getting-started-examples.html
*  
*  This header file establishes the connection with a MySQL database using the MySQL Connector/C++ library
*/

#pragma once
#ifndef __DATABASE_HPP__
#define __DATABASE_HPP__

#ifdef _WIN32
#include <jdbc/mysql_connection.h>
#include <jdbc/mysql_driver.h>
#elif __linux__
#include <mysql_connection.h>
#include <mysql_driver.h>
#endif

extern sql::Driver     *driver;
extern sql::Connection *con;

bool fetchTables(void);
bool addGraphEdge(void); // This function is called by the city object

#endif // DATABASE_HPP