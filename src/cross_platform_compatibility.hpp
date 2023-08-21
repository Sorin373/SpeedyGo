#pragma once
#ifndef CMP
#define CMP

#include "declarations.hpp"

using namespace std;

#ifdef __linux__
char _getch(void)
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    printf("%c\n", buf);
    return buf;
}
#endif

void clear_screen(void)
{
#ifdef _WIN32
    system("CLS");
#elif __linux__
    system("clear");
#endif
}

void sleepcp(const int ms)
{
#ifdef _WIN32
    Sleep(ms);
#elif __linux__
    usleep(ms * 1000);
#endif
}

#endif