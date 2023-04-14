#ifndef LOGIC
#define LOGIC

#include "declarations.hpp"

void statisticaStoc()
{
    NOD_PRODUS_DEPOZIT_LOCAL *ptr = head_local;
    while (ptr != nullptr)
    {
        if (ptr->cantitate_produs < 50)
        {
            int tempIdOras = stoi(ptr->ID_Oras);
            if (tempIdOras > 5)
                matrice_drum[tempIdOras][tempIdOras] = true;
        }
        ptr = ptr->next_l;
    }
    
    /*
    for (unsigned int i = 1; i < 45; i++)
    {
        for (unsigned int j = 1; j < 45; j++)
            if (matrice_drum[i][j] == true)
                cout << 1;
            else
                cout << 0;
        cout << endl;
    }
    */
}

int detDepozitStart(const int tempID_Oras)
{

}

void determinareStartTraseu()
{
    
}

void determinareTraseu()
{

}

#endif