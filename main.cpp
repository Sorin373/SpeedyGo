#include <iostream>

#include "database.hpp"
#include "declarations.hpp"
#include "compatibilityFunctions.hpp"

using namespace std;

int main()
{
    initDrum();
    accesareDate();
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
        
    for (unsigned int i = 0; i < MAXN; i++)
        delete[] matrice_drum[i];
    delete[] matrice_drum;
    
    return 0;
}