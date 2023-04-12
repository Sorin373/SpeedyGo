#ifndef LOGIC
#define LOGIC

#include "declarations.hpp"

/*Determinarea oraselor in nevoie de reaprovizionare*/

void cautare_stoc_depozite()
{

}

int determinare_depozit(char *ptr)
{
    int ID_Oras = stoi(ptr), cID_Oras = 0;
    double distanta = 100000;
    for (unsigned int i = 1; i <= 5; i++)
        if (matrice_drum[i][ID_Oras] < distanta)
        {
            distanta = matrice_drum[i][ID_Oras];
            cID_Oras = i;
        }
    return cID_Oras;
}

void validare_reaprovizionare()
{
    NOD_PRODUS_DEPOZIT_LOCAL *ptr = head_local;

    while (ptr != nullptr)
    {   
        if (ptr->cantitate_produs < 50)
        {
            int id_oras_depozit = determinare_depozit(ptr->ID_Oras);
            if (id_oras_depozit != 0)
                cout << orase[id_oras_depozit].denumire_oras << endl;
        }
        ptr = ptr->next_l;
    }
}

/*Determinarea traseului optim de a reaproviziona*/

/*Determinarea depozitului cel mai apropiat de orasele in nevoie*/

#endif