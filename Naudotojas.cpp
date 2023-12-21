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

void mazosios_raides(string& str) {
    
    map<char, char> LTU = { // apsirasau lt raides 

        {'À', 'à'}, {'È', 'è'}, {'Æ', 'æ'}, {'Ë', 'ë'},
        {'Á', 'á'}, {'Ð', 'ð'}, {'Ø', 'ø'}, {'Û', 'û'}, {'Þ', 'þ'}
    };

    transform(str.begin(), str.end(), str.begin(),// visas  raides á maþàsias

        [&LTU](unsigned char c) -> char {
            auto it = LTU.find(c); //iesko lt raidziu
            if (it != LTU.end()) return it->second;// lt raidziu keitimnas 
            return std::tolower(c);//visu raidzsiu keitimas i mazasias 
        });
}

void naudotojas(string& Ivedimas) {
    do {
        cout << "Pasirinkite ar duomenis isves i faila ar i ekrana tiesiog. F-failas, E-ekranas ";
        cin >> Ivedimas;
        mazosios_raides(Ivedimas);
        if (Ivedimas != "f" && Ivedimas != "e") {
            cout << "Neteisingai parasete! Bandykite dar karta." << endl;
        }
    } while (Ivedimas != "f" && Ivedimas != "e");

            

        cin.ignore(); // leis per nauj ivesti 
    
}
//set string asociatyvus konteineris 
void Domenai(set<string>& domenas) { // domenu  galunes sudedam i set 
    //set auto surusiuoja irgi asociatyvus
    ifstream Domenai_failas;
    string tld; //saugos domeno pabaiga 
    string failoPavadinimas = "domenai.txt";

    while (true) {
        Domenai_failas.open(failoPavadinimas);
        if (Domenai_failas.is_open()) {
            while (getline(Domenai_failas, tld)) {//skaitymas ie eiles 
                if (!tld.empty()) {
                    domenas.insert(tld);// jei randa domeno galune pridedame i set string konteineri
                }
            }
            Domenai_failas.close();
            break;
        }
        else {
            cerr << "Nepavyko atidaryti failo, iveskite pavadinima failo " << failoPavadinimas << ". Áveskite naujà pavadinimà: ";
            cin >> failoPavadinimas; 
            
            Domenai_failas.clear();//isvalome ir vel praso ivesti
            
        }
    }
}

void Adresas(const string& tekstas, const set<string>& domenas, vector<string>& urls) {
    string tldreguliari_israiska;//reguliari israiska
    for (const auto& tld : domenas) {
        if (!tldreguliari_israiska.empty()) {
            tldreguliari_israiska += "|";
        }
        tldreguliari_israiska += "\\." + tld;
    }

    regex urlRegex("(https?://)?[a-zA-Z0-9\\-\\.]+(" + tldreguliari_israiska + ")\\b");
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
            mazosios_raides(zodis); // Use the custom mazosios_raides function
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
    //du map konteineriai saugo zodi ir eilutes numeri  automatiskai  zodzius surusiuoja
    string eilute, zodis;
    int eilutes_nr = 1;
    while (getline(failas, eilute)) {// skaitymas is failo prasideda 
        istringstream ss(eilute);//srautas þodþiø atskyrimui eilutëje
        map<string, int> eilutes_zodziai;// Laikinas map asociatybus þodþiø pasikartojimui eilutëje saugoti

        while (ss >> zodis) {//skaitymas 
           
            zodis.erase(remove_if(zodis.begin(), zodis.end(),
                [](unsigned char c) { return !isalpha(c) && c < 128; }), zodis.end());// pasalina 
            //jei c nera raide arba is ascii lenteles 
            mazosios_raides(zodis);// i mazasias pakeiciame raides  

            if (!zodis.empty() && zodis.find_first_of(".,-„“–“”") == string::npos) {
                eilutes_zodziai[zodis]++;// tikriname ar netuscias zodis ir ar nera zenklu
                //jei tenkins saugome laikinam map eilutes_zodziai
            }
        }
        for (const auto& zodzio_pora : eilutes_zodziai) {
            sk_zodi[zodzio_pora.first][eilutes_nr] = zodzio_pora.second;// saugome i map sk_zodi 
        }
        eilutes_nr++;//padidiname eilute nr ir einame i kita 
    }
}

using Sp_funkcija = function<void(ostream&)>;

void Isvedimas(const string& Ivedimas, const vector<Sp_funkcija>& sp, const vector<string>& Failopavad) {
    if (Ivedimas == "e") {
        for (const auto& Sp_funkcija : sp) {
            Sp_funkcija(cout);
        }
    }
    else if (Ivedimas == "f") {
        for (size_t i = 0; i < sp.size(); ++i) {
            ofstream outFile(Failopavad[i]);// failui naujam 
            if (outFile.is_open()) {// ar atidarytas failas 
                sp[i](outFile);
            }
            outFile.close();
        }
    }
}

int UTF8lietuviskuraidziutolygumas(const std::string& str) {
    int count = 0;
    for (unsigned char c : str) {
        //per kiekviena eilue 

        // Skaièiuoja simboliu skaièiø UTF-8 eil.
        // UTF-8 kodavime  bitø seka 10 (0x80 hex)
        if ((c & 0xC0) != 0x80) {
            ++count;             // Jei baitas nëra UTF-8 antrinio baito dalis, naujo simbolio pradþia

        }
    }
    return count;
}


/*
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
*/

void ZodziuSkaicius(std::ostream& os, const std::map<std::string, int>& zodziu__viso) {
    os << "|---------------------------------------------------------------------|" << std::endl;
    os << "| Zodis                    | Pasikartojimu skaicius                   |" << std::endl;
    os << "|---------------------------------------------------------------------|" << std::endl;
    //  iteruojame per þodþiø map'à ir spausdina þodþius su jø pasikartojimo skaièiumi

    for (const auto& zodzio_pora : zodziu__viso) {
        if (zodzio_pora.first.find(".") != std::string::npos) {
            continue;//praleidiazme taskus 
        }

        if (zodzio_pora.second > 1) {//isvedimo plotis del  lt zenklu 
            // Tikrina, ar þodis pasikartoja daugiau nei vienà kart ir skaièiuoja, kiek baitø yra daugiau naudojama dël UTF-8 koduotø simboliø
            //kad butu grazus lygiavimas 
            // koreguojame atitinkamai 
            int ltpapildomai = zodzio_pora.first.size() - UTF8lietuviskuraidziutolygumas(zodzio_pora.first);
            os << "| " << std::left << std::setw(25 + ltpapildomai) << zodzio_pora.first
                << "| " << std::left << std::setw(34) << zodzio_pora.second << left<< " kart." << " |" << std::endl;
        }
    }

    os << "-----------------------------------------------------------------------" << std::endl;
}




void ZodisVieta(std::ostream& os, const std::map<std::string, std::map<int, int>>& zod_vieta, const std::map<std::string, int>& zod_viso_sk) {
    int plotisstulpelio1 = 25;  
    int plotisstulp = 180;  

    os << "Pasikartojantys zodziai ir ju eilutes:" << std::endl;
    os << "|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|" << std::endl;
    os << "|Zodis                     | Eilute(Kartai(k))                                                                                                                                                                   | " << std::endl;
    os << "|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|" << std::endl;

    for (const auto& zodzio_pora : zod_vieta) {    // Tikrina, ar þodis pasikartoja daugiau nei vienà kartà
        if (zod_viso_sk.at(zodzio_pora.first) > 1) {             // Koreguoja stulpelio plotá, pagal utf-8
            int Tolygumas = zodzio_pora.first.size() - UTF8lietuviskuraidziutolygumas(zodzio_pora.first);
            os << "| " << std::left << std::setw(plotisstulpelio1 + Tolygumas) << zodzio_pora.first << "| ";

            std::stringstream eilutes_srautas;
            for (const auto& eilutes_pora : zodzio_pora.second) {
                eilutes_srautas << eilutes_pora.first << "(" << eilutes_pora.second << " k.) ";//kiek kartu
            }

            std::string eilute = eilutes_srautas.str();
            os << std::left << std::setw(plotisstulp) << eilute << "|" << std::endl;
        }
    }
    os << "|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|" << std::endl;
}

void Domenai_adresai(ostream& os, const vector<string>& urls) {
    os << "Domentai:" << endl;
    os << "|                       Domenai                     |" << endl;
    os << "----------------------------------------------------" << endl;
    for (const auto& u : urls) {     // per url eis ir spasdins 
        os << "| " << setw(50) << left << u << "|" << endl;
    }
    os << "----------------------------------------------------" << endl;
}

