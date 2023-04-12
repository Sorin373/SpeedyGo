#ifndef CMP
#define CMP

#include <iostream>
#include <iomanip>

using namespace std;

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
char getch()
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
#endif // _WIN32

void clear_screen()
{
#ifdef WINDOWS
    system("CLS");
#else
    // Assume POSIX
    system("clear");
#endif
}

void sleepcp(const int ms)
{
#ifdef _WIN32
    sleep(ms);
#else
    usleep(ms * 1000);
#endif // _WIN32
}

void underline(const unsigned int vWidth)
{
    cout << setw(5 - 2) << " ";
    char fillLine;
    fillLine = cout.fill('_');
    cout.width(vWidth);
    cout << '_' << endl;
    cout.fill(fillLine);
    cout << endl;
}

#endif