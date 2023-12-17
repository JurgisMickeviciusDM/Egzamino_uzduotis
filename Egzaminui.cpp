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

using namespace std;

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
/*
void Skaiciuoti_Zodzius(const string& tekstas, map<string, int>& sk_zodi) {
    istringstream ss(tekstas);
    string zodis;
    while (ss >> zodis) {
        zodis.erase(remove_if(zodis.begin(), zodis.end(),
            [](char c) { return !isalpha(static_cast<unsigned char>(c)); }), zodis.end());
        if (!zodis.empty()) {
            sk_zodi[zodis]++;
        }
    }
}*/


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


int main() {
    ifstream failas("seimas.txt");
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

    failas.open("seimas.txt");
    if (!failas.is_open()) {
        cerr << "Nepavyko atidaryti failo antrą kartą." << endl;
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

    cout << "Pasikartojantys zodziai ir ju eilutes:" << endl;
    for (const auto& zodzio_pora : sk_zodi) {
        if (bendras_sk_zodi[zodzio_pora.first] > 1) {
            cout << zodzio_pora.first << ": ";
            for (const auto& eilutes_pora : zodzio_pora.second) {
                cout << eilutes_pora.first << "(" << eilutes_pora.second << " kartus) ";
            }
            cout << endl;
        }
    }

    cout << "\nZodziai, kurie pasikartojo daugiau negu viena karta, ir ju pasikartojimu skaicius:" << endl;
    for (const auto& zodzio_pora : bendras_sk_zodi) {
        if (zodzio_pora.second > 1) {
            cout << zodzio_pora.first << ": " << zodzio_pora.second << " kartus" << endl;
        }
    }


    /*  cout << "Pasikartojantys zodziai:" << endl;
    for (const auto& pair : sk_zodi) {
        if (pair.second > 1) {
            cout << pair.first << ": " << pair.second << endl;
        }
    }*/
    Adresas(tekstas, domenas, url);
    cout << "\nDomenai:" << endl;
    for (const auto& u : url) {
        cout << u << endl;
    }

    return 0;
}