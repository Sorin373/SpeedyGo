/*
 * SPEEDY_GO_HPP
 *
 * This C++ header file has the core function prototypes for the SpeedyGo application.
 * It encapsulates MySQL credential validation, adjacency matrix initialization, and console operations.
 *
 */

#pragma once
#ifndef __SPEEDY_GO_HPP__
#define __SPEEDY_GO_HPP__

#include <iostream>
#include <iomanip>
#include <limits>
#include <type_traits>

namespace speedyGo
{
    struct CinProxy {
        template <typename T>
        CinProxy& operator>>(T &var);
    };

    extern CinProxy cin;

    bool _INIT_(void);
    bool validateMysqlCredentials(void); // Check MySQL database credentials
    bool _ADJACENCY_MATRIX_INIT_(void);  // Init graph
    void displaySpeedyGoDetails(void);
    void MySQLconsole(void); // Console to write SQL
    void alignConsoleOutput(void);
    void free_memory(void);
    void stockStatusVisualization(void);
}

template <typename T>
speedyGo::CinProxy& speedyGo::CinProxy::operator>>(T &var) {
    static_assert(std::is_arithmetic<T>::value, "Input type must be numeric");

    while (!(std::cin >> var)) {
        std::cout << std::setw(5) << " " << "Expected numeric input: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return *this;
}

#endif // SPEEDY_GO_HPP