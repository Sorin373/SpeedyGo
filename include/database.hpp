#pragma once
#ifndef DATABASE_HPP
#define DATABASE_HPP

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

#endif