# BIT - Úkol 3

## Popsání řešení

* Úlohu řeším načtením celého souboru do paměti do struktury ArrayList<char[]>, kde char[] je vždy o velikosti 8. Každá položka v listu reprezentuje osm bitů na každý byte načtený z input souboru. 

* Klíče načtu do LinkedList<String>, kde k ním přistupuji v nahraném pořadí při kódování a v opačném při rozkódování. 

* Jednotlivé char[] poté rozpůlím v metodě encipher na dvě poloviny a začnu iterovat skrze síť standardním způsobem. Po zašifrování zapíši výsledek do output.txt souboru na první řádku.

* Metoda decipher funguje stejně, akorát naopak. ArrayList blocks vyprázdním, neboť ho naplním až ke konci. Data čtu z output.txt po 2 bytech, z kterých tvořím jednotlivé elementy char[] pro blocks. Následuje rozkódování obdobným způsobem, jako zakódování (obrácené pořadí klíčů). Výsledek v hexadecimální podobě uložím na druhou řádku output.txt.

* Ke konci rozkódování ověřím příponu input souboru. Pokud se rovná .txt, rozšifrovaný obsah přidám na třetí řádku output.txt, v opačném případě do souboru feistel_<název_input_souboru>. 

* Při práci s bloky bitů používám vlastní metody:
    * hashRightSide, kde se nachází hashovací funkce
    * get8PlaceBin, která doplní nulové bity doprava pro osm bitů
    * getByte, která převede pole osmi charů (0 nebo 1) na jeden byte
        * tato metoda používá Math.pow pro vyhnutí se bitovým operacím v Javě, které se mi zdají, že se chovají oproti c/c++ jinak.

## Omezení

* Úloha je koncipovaná pouze na délku bloku 8. 

* Při rozkódování dwarf_small.bmp dochází k chybě při práci s instancí Scanner, kterou bohužel nejsem schopen opravit. 
