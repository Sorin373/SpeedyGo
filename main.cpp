#include <iostream>
#include "database.hpp"
#include "declarations.hpp"
#include "compatibilityFunctions.hpp"
//#include "login.hpp"
#include "logic.hpp"

using namespace std;

bool autentificare()
{
    return true;
}

int main()
{  

    accesareDate();

    if (!_init_())
    {
        cerr << "Eroare!";
        return -1;
    }
    
    if (autentificare())
    {   
        unsigned int MENIU;

        do
        {
            clear_screen();

            cout << "MENIU | ADMINISTRATOR\n\n"
                 << "[1]\n" 
                 << "[2]\n"
                 << "[3]\n";

            cout << "Introduceti optiunea: ";
            cin >> MENIU;

            switch (MENIU)
            {
                case 1:
                    afisareDateDepozit();
                    getch();
                    break;
                case 2:
                    afisareDateLocal();
                    getch();
                    break;
                case 3:
                    afisareDateProdus();
                    getch();
                    break;
                case 4:
                    afisareDateOrase();
                    getch();
                    break;
                case 5:
                    determinareStartAprovizionare();
                    getch();
                    break;
            }

        } while (MENIU != 0);
    }
    else
    {
        cerr << "Nu s-a putut autentifica contul!";
        return -1;
    }
    return 0;
}