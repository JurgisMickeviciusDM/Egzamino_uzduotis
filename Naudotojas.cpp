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
#include <codecvt>
#include <locale>



using namespace std;
/*
void toLowercase(string& str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
}
*/
void toLowercase(string& str) {
    
    map<char, char> lithuanianChars = {
        {'À', 'à'}, {'È', 'è'}, {'Æ', 'æ'}, {'Ë', 'ë'},
        {'Á', 'á'}, {'Ð', 'ð'}, {'Ø', 'ø'}, {'Û', 'û'}, {'Þ', 'þ'}
    };

    transform(str.begin(), str.end(), str.begin(),
        [&lithuanianChars](unsigned char c) -> char {
            auto it = lithuanianChars.find(c);
            if (it != lithuanianChars.end()) return it->second;
            return std::tolower(c);
        });
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
    ifstream Domenai_failas;
    string tld;
    string failoPavadinimas = "domenai.txt";

    while (true) {
        Domenai_failas.open(failoPavadinimas);
        if (Domenai_failas.is_open()) {
            while (getline(Domenai_failas, tld)) {
                if (!tld.empty()) {
                    domenas.insert(tld);
                }
            }
            Domenai_failas.close();
            break;
        }
        else {
            cerr << "Nepavyko atidaryti failo, iveskite pavadinima failo " << failoPavadinimas << ". Áveskite naujà pavadinimà: ";
            cin >> failoPavadinimas; 
            
            Domenai_failas.clear();
            Domenai_failas.seekg(0, ios::beg);
        }
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


/*
void Skaiciuoti_Zodzius(ifstream& failas, map<string, map<int, int>>& sk_zodi) {
    string eilute, zodis;
    int eilutes_nr = 1;
    while (getline(failas, eilute)) {
        istringstream ss(eilute);
        map<string, int> eilutes_zodziai;
        while (ss >> zodis) {
            // Custom logic to remove non-alphabetic characters
            zodis.erase(remove_if(zodis.begin(), zodis.end(),
                [](unsigned char c) { return !isalpha(c) && c < 128; }), zodis.end());
            toLowercase(zodis); // Use the custom toLowercase function
            if (!zodis.empty()) {
                eilutes_zodziai[zodis]++;
            }
        }
        for (const auto& zodzio_pora : eilutes_zodziai) {
            sk_zodi[zodzio_pora.first][eilutes_nr] = zodzio_pora.second;
        }
        eilutes_nr++;
    }
}*/
void Skaiciuoti_Zodzius(ifstream& failas, map<string, map<int, int>>& sk_zodi) {
    string eilute, zodis;
    int eilutes_nr = 1;
    while (getline(failas, eilute)) {
        istringstream ss(eilute);
        map<string, int> eilutes_zodziai;
        while (ss >> zodis) {
            // Custom logic to remove non-alphabetic characters
            zodis.erase(remove_if(zodis.begin(), zodis.end(),
                [](unsigned char c) { return !isalpha(c) && c < 128; }), zodis.end());
            toLowercase(zodis); // Use the custom toLowercase function
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


void ZodziuSkaicius(ostream& os, const map<string, int>& zodziu__viso) {
    os << "|---------------------------------------------------------------------|" << endl;
    os << "| Zodis                    | Pasikartojimu skaicius                   |" << endl;
    os << "|---------------------------------------------------------------------|" << endl;
    for (const auto& zodzio_pora : zodziu__viso) {
        if (zodzio_pora.second > 1) {
            os << "| " << left << setw(25) << zodzio_pora.first << "| " << left << setw(34) << zodzio_pora.second << " kart." << " |" << endl;
        }
    }
    os << "-----------------------------------------------------------------------" << endl;
}

void ZodisVieta(ostream& os, const map<string, map<int, int>>& zod_vieta, const map<string, int>& zod_viso_sk) {
    os << "Pasikartojantys zodziai ir ju eilutes:" << endl;
    os << "|-----------------------------------------------------------------------------------------------------------------------------------------------------------|" << endl;
    os << "| Zodis                    | Eilute (Kartai(k))                                                                                                             |" << endl;
    os << "|-----------------------------------------------------------------------------------------------------------------------------------------------------------|" << endl;
    for (const auto& zodzio_pora : zod_vieta) {
        if (zod_viso_sk.at(zodzio_pora.first) > 1) {
            os << "| " << left << setw(25) << zodzio_pora.first << "| ";

            stringstream eilutes_srautas;
            for (const auto& eilutes_pora : zodzio_pora.second) {
                eilutes_srautas << eilutes_pora.first << "(" << eilutes_pora.second << " k.) ";
            }

            string eilute = eilutes_srautas.str();
            os << eilute;
            os << right << setw(120) << "|" << endl;
        }
    }
    os << "|-----------------------------------------------------------------------------------------------------------------------------------------------------------|" << endl;
}

void Domenai_adresai(ostream& os, const vector<string>& urls) {
    os << "Domentai:" << endl;
    os << "|                       Domenai                     |" << endl;
    os << "----------------------------------------------------" << endl;
    for (const auto& u : urls) {
        os << "| " << setw(50) << left << u << "|" << endl;
    }
    os << "----------------------------------------------------" << endl;
}

