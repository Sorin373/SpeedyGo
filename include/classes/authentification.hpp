/*
*  AUTHENTICATION_HPP
*  Database Authentication Information
*
*  This C++ header file defines the AUTHENTICATION class
*  It is responsible for storing and providing the database credentials
*
*/

#pragma once
#ifndef AUTHENTICATION_HPP
#define AUTHENTICATION_HPP

class AUTHENTICATION
{
private:
    struct AUTHENTICATION_NODE
    {
        char *host_name = nullptr, *username = nullptr, *password = nullptr, *database = nullptr;

        AUTHENTICATION_NODE(const char *host_name, const char *username, const char *password, const char *database);
        ~AUTHENTICATION_NODE();
    };

    static AUTHENTICATION_NODE *data;

public:
    static void getData(const char *host_name, const char *username, const char *password, const char *database);
    static  AUTHENTICATION_NODE *getNode(void);
    static void cleanup(void);
    ~AUTHENTICATION() = default;
};

#endif // AUTHENTICATION_HPP