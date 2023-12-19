#include <algorithm>
#include "Naudotojas.h"
using namespace std;

void toLowercase(string& str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
}

void naudotojas(string& Ivedimas) {
    do {
        cout << "Pasirinkite ar duomenis isves i faila ar i ekrana tiesiog. F-failas, E-ekranas ";
        cin >> Ivedimas;
        toLowercase(Ivedimas);
        if (Ivedimas != "f" && Ivedimas != "e") {
            cout << "Neteisingai parasete! Bandykite dar karta." << endl;
        }
    } while (Ivedimas != "f" && Ivedimas != "e");

            

        cin.ignore(); // leis per nauj ivesti 
    
}

