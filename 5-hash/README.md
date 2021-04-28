# BIT - Úkol 5

## Instalace

* Rozbalte zip a v root složce proveďte příkaz:

```
cmake .; make
```

* Pro spuštění si připravte parametry dle zadání, např.:

```
./5-hash input.txt
```

## Popsání řešení

### Transformace inicializačního vektoru

Probíhá při zavolání konstruktoru _Hasher_ instance. Vektor je uložen v _Hasher.h_ kde se dá staticky 
měnit před spuštěním programu.
_std::vector<u\_int16\_t> ABCD_ je attribut třídy _Hasher_, kde je uložen inicializační vektor, jehož prvky 
jsou překonvertované z 16 _bool_ na jeden _u\_int16\_t_.

### Načítání vstupního souboru

Při každé iteraci hashe načtu skrze _std::ifstream_ do dvoubytového _uint16\_t_ (_unsigned short_)
přesně dva znaky přetypované na _char *_. Jelikož se byty načítají odzadu, musím je přehodit.
Např.: vstup _AB_ (0b0100000101000010) se mi načte jako 0b0100001001000001, čili pokud 
proměnnou "roztrhnu" na dva po sobě jdoucí chary, dostanu _BA_.
Přehozené byty uložím do druhé proměnné _uint16\_t_, kterou posílám do hashovací funkce.

### třída Hasher

Stará se o jednotlivé cykly hashování a konstrukci incializačního vektoru do stavu použitelného v hashování.

### třída HF

Jméno Hash Functions. Třída plná jen statických funkcí. Obsahuje funkce reprezentující složitější části hashové
funkce ze zadání, včetně sudé a liché funkce _f_.


## Omezení

Pro lichý počet znaků v inputu, program kontroluje po čtecím while cyklu zda-li v bufferu není něco jiné než 0 (přesnějí
0x0000). Pokud je, provede se ještě jedna iterace pro finální znak. Z toho ale plyne, že pokud poslední lichý přijatý
znak bude 0x0000, program ho bude ignorovat.