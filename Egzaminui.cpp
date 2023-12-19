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

void naudotojas(std::string& Ivedimas, std::string& Pasirinkiams);


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


int main() {
    string Ivedimas, pasirinkimas;
    naudotojas(Ivedimas, pasirinkimas);


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

    //1 uzd lentele
    cout << "\nZodziai, kurie pasikartojo daugiau negu viena karta, ir ju pasikartojimu skaicius:" << endl;
    cout << "|---------------------------------------------------------------------|" << endl;
    cout << "| Zodis                  | Pasikartojimu skaicius                     |" << endl;
    cout << "|---------------------------------------------------------------------|" << endl;
    for (const auto& zodzio_pora : bendras_sk_zodi) {
        if (zodzio_pora.second > 1) {
            cout << "| " << zodzio_pora.first;
            cout.width(25 - zodzio_pora.first.length());
            cout << "| " << zodzio_pora.second << " kart.";
            cout.width(38 - to_string(zodzio_pora.second).length());
            cout << "|" << endl;
        }
    }
    cout << "-----------------------------------------------------------------------" << endl;


    //2 uzd lentele
    cout << "Pasikartojantys zodziai ir ju eilutes:" << endl;
    cout << "|-----------------------------------------------------------------------------------------------------------------------------------------------------------|" << endl;
    cout << "| Zodis                  | Eilute (Kartai(k))                                                                                                               |" << endl;
    cout << "|-----------------------------------------------------------------------------------------------------------------------------------------------------------|" << endl;
    for (const auto& zodzio_pora : sk_zodi) {
        if (bendras_sk_zodi[zodzio_pora.first] > 1) {
            cout << "| " << zodzio_pora.first;
            cout.width(25 - zodzio_pora.first.length());
            cout << "| ";

            stringstream eilutes_srautas;
            for (const auto& eilutes_pora : zodzio_pora.second) {
                eilutes_srautas << eilutes_pora.first << "(" << eilutes_pora.second << " k.) ";
            }

            string eilute = eilutes_srautas.str();
            cout << eilute;
            cout.width(130 - eilute.length());
            cout << "|" << endl;
        }
    }
    cout << "|------------------------------------------------------------------------------------------------------------------------------------------------------------|" << endl;
    
    
    //3 uzd lentele 
    cout << "|                       Domenai                     |" << endl;
    cout<<"------------------------------------------------------" << endl;
    Adresas(tekstas, domenas, url);
   
    for (const auto& u : url) {
        cout << "| "<< setw(50) <<left  << u << "|" << endl;
    }
    cout << "------------------------------------------------------" << endl;
    return 0;
}




