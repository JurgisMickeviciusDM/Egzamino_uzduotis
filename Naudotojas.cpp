#include <algorithm>
#include "Naudotojas.h"
using namespace std;

void toLowercase(string& str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
}

void naudotojas(string& Ivedimas, string& Pasirinkimas) {
    do {
        cout << "Pasirinkite ar duomenis isves i faila ar i ekrana tiesiog. F-failas, E-ekranas ";
        cin >> Ivedimas;
        toLowercase(Ivedimas);
        if (Ivedimas != "f" && Ivedimas != "e") {
            cout << "Neteisingai parasete! Bandykite dar karta." << endl;
        }
    } while (Ivedimas != "f" && Ivedimas != "e");

    if (Ivedimas == "f") {
        do {
            cout << "Prasome pasirinkti pagal ka rikiuosite zodzius ar pasikartojimu skaiciu: Z-zodis, P-pasikartojimai " << endl;
            cin >> Pasirinkimas;
            toLowercase(Pasirinkimas);
            if (Pasirinkimas != "z" && Pasirinkimas != "p") {
                cout << "Neteisinga pasirinkimas! Bandykite dar karta. " << endl;
            }
        } while (Pasirinkimas != "z" && Pasirinkimas != "p");

            

        cin.ignore(); // leis per nauj ivesti 
    }
}

