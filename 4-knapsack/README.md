# BIT - Úkol 4

## Instalace

* Rozbalte zip a v root složce proveďte příkaz:
```
cmake .; make
```
* Pro spuštění si připravte parametry dle zadání, např.:
```
./4-knapsack input.txt 31 105
```

## Popsání řešení

* Kdykoli se cokoli nepodaří vlivem špatných parametrů, program vyhodí std::runtime_error, čímž se zastaví.

* Program nejdříve zkontroluje vložené parametry.
  * Nesoudělnost p a q
  * Zda je privátní klíč super rostoucí posloupnost
  * q musí být větší než sum(privátní klíč)

* Pokusí se o kontrukci objektu Knapsack, kde zároveň se snaží vygenerovat privátní i veřejný klíč.

* Následuje načítání input souboru, kde se zjistí zda jde o .txt příponu.
  * Pokud input není .txt, považuje se za binární.

* V metodě main následuje volání metody _cypher_ z Knapsack objektu.

* Analogicky následuje i metoda _decypher_.
  * V obou metodách jsou zakomponované výpisy do output souboru používající std::ofstream.

* Na konci proběhne check zda-li se jedná o binární soubor.
  * Pokud ano, binárně se vypíše rozšifrovaný soubor do nového s prefixem _knapsack\__.

* Program v průběhu oznamuje svůj dosavadní progres zprávami do konzole.

## Omezení

* Metoda getInvP používá brute-force pro výpočet p^-1
  
* Testovací soubory fungují bez problému a zátěžové testy jsem nepodnikal, tudíž nemohu usoudit jak by aplikace fungovala s většími soubory.

* Jelikož při načítání souborů používám std::size_t (unsigned long) který má velikost na 32 bitů, usuzuji, že soubor o velikosti větší než 4 GB bude působit potíže.