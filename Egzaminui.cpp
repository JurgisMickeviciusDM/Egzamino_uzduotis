#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <regex>
#include <locale>
#include <set>
#include <cctype> 


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
}


int main() {
    ifstream failas("seimas.txt");
    string eilute, tekstas;

    while (getline(failas, eilute)) {
        tekstas += eilute + " ";
    }
    failas.close();

    set<string> domenas;
    Domenai(domenas);

    map<string, int> sk_zodi;
    vector<string> url;

    Skaiciuoti_Zodzius(tekstas, sk_zodi);
    Adresas(tekstas, domenas, url);

    cout << "Pasikartojantys zodziai:" << endl;
    for (const auto& pair : sk_zodi) {
        if (pair.second > 1) {
            cout << pair.first << ": " << pair.second << endl;
        }
    }

    cout << "\nDomenai:" << endl;
    for (const auto& url : url) {
        cout << url << endl;
    }

    return 0;
}
