#include <iostream>

#include "database.hpp"
#include "declarations.hpp"
#include "compatibilityFunctions.hpp"
#include "login.hpp"
#include "logic.hpp"

using namespace std;

bool autentificare()
{
    if (form())
    {
        initDrum();
        accesareDate();
        return true;
    }
    return false;
}

int main()
{
    if (autentificare())
    {
        unsigned int MENIU;

        do
        {
            clear_screen();

            cout << "MENIU | ADMINISTRATOR\n\n"
                 << "[1] Vizualizarea livrarilor din urmatoarea zi";

            cout << "Introduceti optiunea: ";
            cin >> MENIU;

            switch (MENIU)
            {
                case 1:
                    break;
            }

        } while (MENIU != 0);

        for (unsigned int i = 0; i < MAXN; i++)
            delete[] matrice_drum[i];
        delete[] matrice_drum;
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

    return 0;
}