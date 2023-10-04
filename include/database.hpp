#pragma once
#ifndef DATABASE
#define DATABASE

#ifdef _WIN32
#include <jdbc/mysql_connection.h>
#include <jdbc/mysql_driver.h>
#elif __linux__
#include <mysql_connection.h>
#include <mysql_driver.h>
#endif

extern sql::Driver *driver;
extern sql::Connection *con;

bool addGraphEdge(void);
extern bool fetchTables(void);
extern bool SQL_Data_Update(const int input);

#endif