#include <iostream>

#include "database.hpp"
#include "declarations.hpp"
#include "compatibilityFunctions.hpp"
#include "login.hpp"
#include "logic.hpp"

using namespace std;

bool autentificare()
{
    return true;
}

int main()
{  

    accesareDate(); 
    init();

    //afisareDateLocal();
    
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
                    statisticaStoc();
                    /*
                    for (unsigned int i = 0; i < 45; i++)
                        cout <<  matrice_drum_aprovizionare[i] << " ";*/
                    getch();
                    break;
                case 5:
                    determinareTraseu();
                    getch();
                    break;
            }

        } while (MENIU != 0);

        for (unsigned int i = 0; i < MAXN; i++)
            delete[] matrice_drum[i];
        delete[] matrice_drum;
    }
    else
    {
        cerr << "Nu s-a putut autentifica contul!";
        return -1;
    }

    // afisareDate();
    // afisareDateProdus();

    /*
    for (unsigned int i = 0; i < contorDrum; i++)
        cout << distanta_drum[i] << " km" << endl;*/

    /*
    for (unsigned int i = 0; i < 20; i++)
    {
        for (unsigned int j = 0; j < 20; j++)
            cout << matrice_drum[i][j] << " ";
        cout << endl;
    }*/

    /*
    for (unsigned int i = 0; i < MAXN; i++)
            delete[] matrice_drum[i];
        delete[] matrice_drum;
    */
    return 0;
}