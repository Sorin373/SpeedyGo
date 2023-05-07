#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include "database.hpp"
#include "declarations.hpp"
#include "compatibilityFunctions.hpp"
#include "logic.hpp"

using namespace std;

void animatie(bool& incarcare_date)
{
    cout << "\033[?25l";
    cout << "\n\n\n\t\t\t\tLoading...";
    cout << "\n\n\t\t\t\t";

    for(int i = 0; i < 25; i++)
        cout << "\u2591";

    cout <<"\r";
    cout <<"\t\t\t\t";

    while (incarcare_date)
    {
        for(int i = 0; i < 25; i++)
        {
            cout << "\u2588";
            cout.flush();
            this_thread::sleep_for(chrono::milliseconds(50));
        }
        cout << "\r\t\t\t\t";
        for(int i = 0; i < 25; i++)
        {
            cout << "\u2591";
            cout.flush();
            this_thread::sleep_for(chrono::milliseconds(50));
        }
        cout <<"\r\t\t\t\t";
    }
    cout << "\033[?25h";
}

bool start()
{
    if (autentificare_cont() == EXIT_FAILURE)
        return EXIT_FAILURE;
    else if (_init_() == EXIT_FAILURE)
        return EXIT_FAILURE;

    bool incarcare_date = true;
    thread incarcare_date_thread(animatie, ref(incarcare_date));

    thread depozit_thread(sortare_date_depozit);
    thread oras_thread(sortare_date_oras);
    thread produs_thread(sortare_date_produs);

    depozit_thread.join();
    oras_thread.join();
    produs_thread.join();

    incarcare_date = false;
    incarcare_date_thread.join();

    clear_screen();

    return EXIT_SUCCESS;
}

int main()
{
    unsigned int MENIU;

    if (start() != 0)
        return EXIT_FAILURE;

    do
    {
        clear_screen();

        cout << "\n\n" << setw(20) << " " << "MENIU\n";
        underline(45);

        cout << setw(5) << " " << "[1] Aprovizioare stoc Romania\n"
             << setw(5) << " " << "[2] Vizualizare stoc Romania\n"
             << setw(5) << " " << "[0] EXIT\n";

        underline(45);
        cout << setw(5) << " " << "Introduceti numarul meniului: ";

        cin >> MENIU;

        switch (MENIU)
        {
        case 1:
            verificare_rute();
            getch();
            break;
        case 2:
            vizualizare_status_stoc();
            break;
        case 3:
            getch();
            break;
        case 4:
            getch();
            break;
        case 5:
            getch();
            break;

        default:
            break;
        }

    } while (MENIU != 0);

    return EXIT_SUCCESS;
}