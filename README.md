# Egzamino_užduotis
Šio projekto tikslas yra sukurti programą, kuri geba iš teksto išrinkti žodžius kurie kartojasi, daugiau, negu vieną kartą. Antra užduoties dalis, iš tų žodžių kurie kartojasi daugiau negu vieną kartąpadaryti lentele, kurioje būtų žodis ir nurodoma vieta, kur randasi žodis, tai yra eilutė. Trečia užduotis yra pateikti domenus, kurie yra tekste. Visos lentelės gali būti bendrai išvestos į failą arba į ekraną, priklausomai nuo vartotojo pasirinkimo.
# Tekstas
Analizuojamas teksta yra paimtas iš https://lt.wikipedia.org/wiki/Eurovizijos_dain%C5%B3_konkursas_2023. Jame yra pateikaiam informacija lietuviu kalba apie 2023 metų euroviziją. Pats tekstas nukopijuotas ir patalpintas į tekstinį failą Tekstas.txt. Taip pat į tekstinį failą yra įkelti visi domenia, kurie buvo pateikti Wikipedia 2023 metų eurovizija puslapio apačioje. 
|:----------------------------:	|:-------------:	|
|         Žodžių skaičiu      	|      1244     	| 
|:----------------------------:	|:-------------:	|
 # Projekto aprašas
Pirmoje užduoties dalyje yra sukuriama lentelė, kurioje pateikiama informacija apie žodžius, kurie kartjasi daugiau, negu vieną kartą. Lentelę sudaro 2 stulpeliai ,,Žodis'' ir ,,Pasikartojimu Skaicius'', zodziai yra surašomi nuo a iki z.
Į failą išvedimo fragmentas:
![image](https://github.com/JurgisMickeviciusDM/Egzamino_uzduotis/assets/144474535/05eca3ff-c27d-43e3-8593-e9bcd9332fb7)
Antroje užduoties dalyje yra pateikaiam lentelė, kuri išveda duomenis kuriose eilutėse yra pasikartojantys žodžiai, jeigu žodis toje eilutėje kartojasi daugiau, negu vieną kartą, tai nurodomas pasiakrtojimų skaičius.
Į failą išvedimo fragmentas:
![image](https://github.com/JurgisMickeviciusDM/Egzamino_uzduotis/assets/144474535/64483856-970c-4a69-9f56-0b512f4147b7)
Trečioje užduoties dalyje yra pateikiama lentelė, kurioje išvedama informacija apie domenus:
Į failą išvedimo fragmentas:
![image](https://github.com/JurgisMickeviciusDM/Egzamino_uzduotis/assets/144474535/b97816c1-3fe6-4be0-957f-a924e5f61aa8)
# Domenai
Tam, kad būtų tinakmai išskiriami adresai, naudojau sąrašą domenų galunių, dar vadinamų TLD(Top-level domain) https://en.wikipedia.org/wiki/List_of_Internet_top-level_domains, tačiau juos ėmiau iš svetainės: https://data.iana.org/TLD/tlds-alpha-by-domain.txt
(žiūrėjimo data 2023-12-18, buvo 1454 domenai, tai yra galūnės). Tokį principą nustatinėjant url adresus pasirinkau, nes kitu atveju programa galėtų išvesti ir tokius adressu, kaip PVZ: www.universitetas.vilnius ar www.universitetas.vu, nors tokie domenai vilnius,vu nėra tikros domenų galūnės. Jie yra saugomi faile Domenai.txt ir paleidus programa iš ten imami.
# Lietuviškas tekstas ir lygiavimas lentelėse:
Kadangi naudojant lietuvišką tekstą, tam, kad nekiltų problemų reikia naudoti UTF-8 koduotę
![image](https://github.com/JurgisMickeviciusDM/Egzamino_uzduotis/assets/144474535/dda88bf3-9554-406f-86d7-3e9c8808cdf3)
https://en.cppreference.com/w/cpp/language/ascii
