#include "main.h"
#include "funkcje.h"


size_t sprawdz_baze(ifstream &baza_pytan){
    if( !baza_pytan.good() )  {cout << "bazaZla\n"; return -1;}
    baza_pytan.clear();
    baza_pytan.seekg(0, ios::beg);
    baza_pytan.unsetf(std::ios_base::skipws);
    size_t line_count = count(istream_iterator<char>(baza_pytan),istream_iterator<char>(), '\n');
    baza_pytan.clear();
    baza_pytan.seekg(0,ios::beg);
    return line_count;
}

