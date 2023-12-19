#include <algorithm>
#include "Naudotojas.h"
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

void Domenai(set<string>& domenas) {
    ifstream Domenai_failas("domenai.txt");
    string tld; // tld auksiausiolygiodomenai angliskai topleveldomens 

    if (Domenai_failas.is_open()) {
        while (getline(Domenai_failas, tld)) {
            if (!tld.empty()) {
                domenas.insert(tld);
            }
        }
        Domenai_failas.close();
    }
    else {
        cerr << "Nepavyko atidaryti." << endl;
    }
}

void Adresas(const string& tekstas, const set<string>& domenas, vector<string>& urls) {
    string tldRegexPart;
    for (const auto& tld : domenas) {
        if (!tldRegexPart.empty()) {
            tldRegexPart += "|";
        }
        tldRegexPart += "\\." + tld;
    }

    regex urlRegex("(https?://)?[a-zA-Z0-9\\-\\.]+(" + tldRegexPart + ")\\b");
    auto zodis_pradzia = sregex_iterator(tekstas.begin(), tekstas.end(), urlRegex);
    auto zodis_pabaiga = sregex_iterator();

    for (sregex_iterator i = zodis_pradzia; i != zodis_pabaiga; ++i) {
        urls.push_back((*i).str());
    }
}



void Skaiciuoti_Zodzius(ifstream& failas, map<string, map<int, int>>& sk_zodi) {
    string eilute, zodis;
    int eilutes_nr = 1;
    while (getline(failas, eilute)) {
        istringstream ss(eilute);
        map<string, int> eilutes_zodziai;
        while (ss >> zodis) {
            zodis.erase(remove_if(zodis.begin(), zodis.end(),
                [](char c) { return !isalpha(static_cast<unsigned char>(c)); }), zodis.end());
            transform(zodis.begin(), zodis.end(), zodis.begin(), ::tolower);
            if (!zodis.empty()) {
                eilutes_zodziai[zodis]++;
            }
        }
        for (const auto& zodzio_pora : eilutes_zodziai) {
            sk_zodi[zodzio_pora.first][eilutes_nr] = zodzio_pora.second;
        }
        eilutes_nr++;
    }
}

using PrintFunction = function<void(ostream&)>;

void Isvedimas(const string& Ivedimas, const vector<PrintFunction>& printFunctions, const vector<string>& filenames) {
    if (Ivedimas == "e") {
        for (const auto& printFunction : printFunctions) {
            printFunction(cout);
        }
    }
    else if (Ivedimas == "f") {
        for (size_t i = 0; i < printFunctions.size(); ++i) {
            ofstream outFile(filenames[i]);
            if (outFile.is_open()) {
                printFunctions[i](outFile);
            }
            outFile.close();
        }
    }
}


void ZodziuSkaicius(ostream& os, const map<string, int>& wordCounts) {
    os << "|---------------------------------------------------------------------|" << endl;
    os << "| Zodis                  | Pasikartojimu skaicius                     |" << endl;
    os << "|---------------------------------------------------------------------|" << endl;
    for (const auto& zodzio_pora : wordCounts) {
        if (zodzio_pora.second > 1) {
            os << "| " << left << setw(25) << zodzio_pora.first << "| " << right << setw(25) << zodzio_pora.second << " kart." << " |" << endl;
        }
    }
    os << "-----------------------------------------------------------------------" << endl;
}

void ZodisVieta(ostream& os, const map<string, map<int, int>>& wordLocations, const map<string, int>& totalWordCounts) {
    os << "Pasikartojantys zodziai ir ju eilutes:" << endl;
    os << "|-----------------------------------------------------------------------------------------------------------------------------------------------------------|" << endl;
    os << "| Zodis                  | Eilute (Kartai(k))                                                                                                               |" << endl;
    os << "|-----------------------------------------------------------------------------------------------------------------------------------------------------------|" << endl;
    for (const auto& zodzio_pora : wordLocations) {
        if (totalWordCounts.at(zodzio_pora.first) > 1) {
            os << "| " << left << setw(25) << zodzio_pora.first << "| ";

            stringstream eilutes_srautas;
            for (const auto& eilutes_pora : zodzio_pora.second) {
                eilutes_srautas << eilutes_pora.first << "(" << eilutes_pora.second << " k.) ";
            }

            string eilute = eilutes_srautas.str();
            os << eilute;
            os << "|" << endl;
        }
    }
    os << "|------------------------------------------------------------------------------------------------------------------------------------------------------------|" << endl;
}

void Domenai_adresai(ostream& os, const vector<string>& urls) {
    os << "|                       Domenai                     |" << endl;
    os << "------------------------------------------------------" << endl;
    for (const auto& u : urls) {
        os << "| " << setw(50) << left << u << "|" << endl;
    }
    os << "------------------------------------------------------" << endl;
}

