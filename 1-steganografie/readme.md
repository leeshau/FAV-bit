#Analýza

* K ukrytí obrázku je třeba modifikovat jednotlivé bity potřebném počtu bytů zprava. 
* Když v bytu pro červenou barvu, který má hodnotu 1111 1111 (tj. 255) vezmu první bit zprava a budu zde potřebovat uložit 0 kvůli datům, lidské oko nepozná rozdíl mezi sytostí 255 a 254.
* Počet změnených bytů záleží na velikosti obrázku, který potřebujeme uložit a také do jaké míry se obrázek změní (pokud změníme 7 bitů z 8, původní obrázek bude nejspíše k nepoznání). 
* V .bmp formátu najdeme přechod mezi hlavičkou (která nesmí býti porušena) a daty na 54. bytu. Od této pozice tedy můžeme bity upravovat.


#Řešení

* Pokud upravíme v každém "data" bytu poslední 4 bity, původní obrázek ztratí na kvalitě, ale ne drasticky. Zároveň můžeme do obrázku uložit jiný obrázek o velikosti poloviny původního - 54 bytů (konec hlavičky).
* Pro pozdější rozložení do dvou souborů potřebujeme uchovávat velikost původního obrázku, který jsme vkopírovali. Tuto velikost v jednotkách bytů uložíme na celé poslední čtyři byty (unsigned long) původního obrázku, do kterého vkopírujeme menší obrázek i s jeho velikostí. Tato velikost se při začátku extrakce zjišťuje posledními 4 byty.
* Krom souborů a jejich otevření pro jednotlivé fstreamy, se potřebuje také kontrolovat jejich velikost (viz. první krok v řešení).
* Pro implementaci řešení je použita třída std::fstream. Pro obě procedury (merge a extract) jsou vytvořeny dvě velmi podobné funkce s cykly, které procházejí oba soubory (source po dvou bytech, latter po jednom).
	* jako source se označuje větší file, do kterého se vkopíruje (nebo ze kterého se extrahuje) druhý soubor latter
	* latter je menší soubor, který neztratí na kvalitě, neboť se uplně celý vloží do souboru source, pokud projde validací velikosti
* Pokud bychom chtěli ukrývat aspoň přibližně stejně velký latter (menší alespoň o hlavičku source), musel by se latter zmenšit na polovinu, stejným způsobem jako source, tj. ignorovaní posledních 4 bitů v každém bytu.

#Použití

##Překlad

* Je potřeba mít cmake a make programy. Verze cmake se dá nastavit v CMakeLists.txt dle aktuální verze na stroji.
* V adresáři root (ten s CMakeLists.txt) spusťte v linuxovém terminálu s potřebnými programy (viz. výše) příkazy: **cmake .; make;**.
* V adresáři se vytvoří soubor _1-steganografie_, který se spustí příkazem **.1_steganografie**.
	* Pokud nepůjde spustit, budete nejspíše potřebovat super user práva, či nastavit pomocí příkazu _chmod_ vygenerovaný soubor jako executable. 
* Program potřebuje parametry:
	* první parametr je mód
		* MERGE pro vložení latter do source
		* EXTRACT pro vytvoření nového souboru latter a vkopírování obrázku ze source (musí být provedeno až po merge procedůře)
	* druhý parametr je jméno source obrázku
	* třetí parametr je jméno latter obrázku

