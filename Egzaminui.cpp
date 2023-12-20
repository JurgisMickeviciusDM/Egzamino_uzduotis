#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <regex>
#include <locale>
#include <set>
#include <cctype> 
#include <algorithm>
#include <iomanip>
#include <functional>
#include <Windows.h>

using namespace std;

void naudotojas(std::string& Ivedimas);
void Domenai(set<string>& domenas);
void Adresas(const string& tekstas, const set<string>& domenas, vector<string>& urls);
void Skaiciuoti_Zodzius(ifstream& failas, map<string, map<int, int>>& sk_zodi);
using PrintFunction = function<void(ostream&)>;
void Isvedimas(const string& Ivedimas, const vector<PrintFunction>& printFunctions, const vector<string>& filenames);
void ZodziuSkaicius(ostream& os, const map<string, int>& zodziu__viso);
void ZodisVieta(ostream& os, const map<string, map<int, int>>& wordLocations, const map<string, int>& zod_viso_sk);
void Domenai_adresai(ostream& os, const vector<string>& urls);



int main() {
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "en_US.UTF-8");
    string Ivedimas;
    naudotojas(Ivedimas); 

    ifstream failas("Tekstas.txt");
    if (!failas.is_open()) {
        cerr << "Nepavyko atidaryti failo." << endl;
        return 1;
    }

    vector<string> url;
    string tekstas, eilute;
    while (getline(failas, eilute)) {
        tekstas += eilute + "\n";
    }
    failas.close();

    set<string> domenas;
    Domenai(domenas);

    map<string, map<int, int>> sk_zodi;
    failas.open("Tekstas.txt");
    if (!failas.is_open()) {
        cerr << "Nepavyko atidaryti failo." << endl;
        return 1;
    }

    Skaiciuoti_Zodzius(failas, sk_zodi);
    failas.close();

    map<string, int> bendras_sk_zodi;
    for (const auto& zodzio_pora : sk_zodi) {
        int bendras_kiekis = 0;
        for (const auto& eilutes_pora : zodzio_pora.second) {
            bendras_kiekis += eilutes_pora.second;
        }
        bendras_sk_zodi[zodzio_pora.first] = bendras_kiekis;
    }

    Adresas(tekstas, domenas, url); 

    if (Ivedimas == "e") {
        ZodziuSkaicius(cout, bendras_sk_zodi);
        ZodisVieta(cout, sk_zodi, bendras_sk_zodi);
        Domenai_adresai(cout, url);
    }
    else if (Ivedimas == "f") {
        ofstream outFile1("Zodziai_pasikartojimu_skacius.txt");
        ZodziuSkaicius(outFile1, bendras_sk_zodi);
        outFile1.close();

        ofstream outFile2("Zodziu_pasikartojimu_eilutes.txt");
        ZodisVieta(outFile2, sk_zodi, bendras_sk_zodi);
        outFile2.close();

        ofstream outFile3("Domenai_web_adresai.txt");
        Domenai_adresai(outFile3, url);
        outFile3.close();
    }
    return 0;
}


