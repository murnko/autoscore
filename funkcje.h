#ifndef FUNKCJE_H
#define FUNKCJE_H
#include "main.h"


size_t sprawdz_baze(ifstream &baza_pytan);
void parsuj_odpowiedzi(ifstream &baza, int rozmiar, int **klucz_odp, int **punkt_odp, int *dlugosci);

#endif // FUNKCJE_H
