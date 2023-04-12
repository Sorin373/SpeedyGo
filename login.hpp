#ifndef LOGIN
#define LOGIN

#include <iostream>
#include <string.h>
#include <fstream>
#include "declarations.hpp"
#include "compatibilityFunctions.hpp"

using namespace std;

constexpr int MAXL = 256;
char *username, *parola;

void initForm()
{
    ifstream fin("dateDeLogare.txt");
    username = (char *)malloc(sizeof(char));
    parola = (char *)malloc(sizeof(char));

    fin >> username >> parola;

    if (strcmp(username, "-") == NULL && strcmp(parola, "-") == NULL)
    {
        ofstream fout("dateDeLogare.txt");
        cin >> username >> parola;
        fout << username << endl
             << parola;
        fout.close();
    }
    fin.close();
}

void verificareDate(unsigned int &MENIU)
{
    clear_screen();

    cout << "TASTEAZA '0' PENTRU A TE INTOARCE\n\n";

    char *tempUN, *tempP;
    tempUN = (char *)malloc(MAXL * sizeof(char));
    tempP = (char *)malloc(MAXL * sizeof(char));

    cout << "Username: ";
    cin >> tempUN;

    if (strcmp(tempUN, "0") == NULL)
        return;
    
    cout << "Parola: ";
    cin >> tempP;

    if (strcmp(tempP, "0") == NULL)
        return;

    if (strcmp(username, tempUN) != NULL)
    {
        cerr << "Date incorecte!";
        sleepcp(1000);
        verificareDate(MENIU);
    }
    else if (strcmp(tempP, parola) != NULL)
    {
        cerr << "Date incorecte!";
        sleepcp(1000);
        verificareDate(MENIU);
    }
    else 
    {
        cout << "Bine ai venit!";
        MENIU = 0;
    }
        

    free(tempUN);
    free(tempP);
}

bool resetareParola()
{
    return false;
}

bool form()
{   
    initForm();
    unsigned int MENIU;

    do
    {
        clear_screen();

        cout << "CONT | ADMIN\n"
             << "[1] Logare\n"
             << "[2] Resetare parola\n"
             << "[0] EXIT\n";

        cout << "Introduceti: ";
        cin >> MENIU;

        switch (MENIU)
        {
        case 1:
            verificareDate(MENIU);
            break;
        case 2:
            resetareParola();
            break;
        }
    } while (MENIU != 0);
    return true;
}

#endif