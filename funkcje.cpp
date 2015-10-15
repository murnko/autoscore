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

void parsuj_odpowiedzi(ifstream &baza, int rozmiar, int**klucz_odp, int**punkt_odp, int*dlugosci){
    char kolejna_linia[256];
    string s, z;
              string * wiersze = new string[rozmiar-2];
    int tmp=0;
    string delimiter = "|";




    for (int i = 0; i<rozmiar-2; i++){
        baza.getline(kolejna_linia, 256);
            wiersze[i] = kolejna_linia;
            if ((i%2)) {

                size_t pos = 0;
                string token;
                int j = 0;
                dlugosci[(i-1)/2] = strlen( wiersze[i].c_str() )/2;
                klucz_odp[(i-1)/2] = new int[dlugosci[(i-1)/2]];
                punkt_odp[(i-1)/2] = new int[dlugosci[(i-1)/2]];
                s = wiersze[i-1];
                cout << s <<endl;
                while ((pos = s.find(delimiter)) != std::string::npos) {
                    token = s.substr(0, pos);
                    //cout << token <<endl;
                    sscanf(token.c_str(), "%d", &tmp);
                    klucz_odp[(i-1)/2][j] = tmp;
                    j++;
                    s.erase(0, pos + delimiter.length());
                }
                s = wiersze[i];
                cout << s <<endl;
                pos = 0;
                j = 0;
                while ((pos = s.find(delimiter)) != std::string::npos) {
                    token = s.substr(0, pos);
                    //cout << token <<endl;
                    sscanf(token.c_str(), "%d", &tmp);
                    punkt_odp[(i-1)/2][j] = tmp;
                    j++;
                    s.erase(0, pos + delimiter.length());
                }


            }

        }
    baza.getline(kolejna_linia, 256);
    z = kolejna_linia;
    sscanf(z.c_str(), "%d", &tmp);
    dlugosci[rozmiar/2] = tmp;

            baza.clear();
            baza.seekg(0,ios::beg);
            //cout << "koniec wag pytań";
    }



