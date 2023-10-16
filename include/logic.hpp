#pragma once
#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <vector>

extern bool validateMysqlCredentials(void);
extern bool _ADJACENCY_MATRIX_INIT_(void);

extern void nr_max_caractere_den(void);
extern bool verificare_orase_stoc_limitat(void);
extern void cautare_produse_ID_stoc_limitat(const int ID_Depozit);
extern void depozite_conectate(int ID_Depozit);
extern void determinare_tip_depozit(void);
extern void cautare_orase_stoc_limitat(void);
extern void cautare_orase_izolate(void);
extern void vizualizare_status_stoc(void);
extern void creare_solutie_distanta(int start, std::vector<double> &distanta, std::vector<int> &distanta_minima, bool afisare, bool creare_trasee);
extern void dijkstra(int start, std::vector<double> &distanta, std::vector<int> &distanta_minima);
extern void afisare_depozite_centralizare(void);
extern void afisare_trasee_optime(const int _ID, const int vStart);
extern void afisare_optiuni_trasee_optime(const int vStart);
extern void sistem_aprovizionare_independent(void);
extern void afisare_depozite_izolate(void);
extern void afisare_depozite_unic_drum(void);

namespace hamiltonianGraph
{
    extern void init_stiva(void);
    extern bool succesor(void);
    extern bool solutie(void);
    extern bool valid(void);
    extern void determinare_ciclu_hc_minim(void);
    extern void back(void);
}

namespace acyclicGraph
{
    extern void init_stiva(void);
    extern bool succesor(void);
    extern bool solutie(void);
    extern bool valid(void);
    extern void determinare_traseu_minim(void);
    extern void back(void);
}

extern void TSP(void);
extern void produse_transport_TSP(void);
extern void pagina_principala_TSP(void);
extern void pagina_finala_TSP(void);
extern void pagina_stanga_TSP(void);
extern void pagina_dreapta_TSP(void);
extern void parcurgere_traseu_TSP(void);
extern void afisare_detalii_SpeedyGo(void);
extern void consola_mysql(void);

#endif // LOGIC_HPP
