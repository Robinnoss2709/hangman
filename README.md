1.	Úvod
Hangman Multiplayer je aplikácia typu server-klient vyvinutá v jazyku C, ktorá umožňuje viacerým hráčom súčasne hrať klasickú hru na hádanie slov. Server spravuje stav hry, zatiaľ čo klienti komunikujú so serverom, aby zadávali svoje hádania. Hra využíva súbežné spracovanie, správu pamäte a komunikačné protokoly na zabezpečenie plynulého multiplayerového zážitku.
2.	Kompilácia
Na kompiláciu serverovej a klientskej aplikácie využite CMake:
2.1.	Vytvorenie súboru CMakeLists.txt
Vytvorte súbor CMakeLists.txt s nasledovným obsahom:
cmake_minimum_required(VERSION 3.10)

project(HangmanGame)

set(CMAKE_C_STANDARD 11)

add_executable(server src/server/server.c src/server/globals.c)
add_executable(client src/client/client.c src/client/play_game.c src/client/utils.c)

target_link_libraries(server pthread)
2.2.	Generovanie build súborov a kompilácia
1.	mkdir build
2.	cd build
3.	cmake ..
4.	make 
3.	Spustenie hry
3.1.	Spustenie servera:
3.1.1.	 Možnosť 1 – priamo cez server
 
Obr.č.1.: Príprava hry
•	./server s <port>
o	Príklad: ./server s 8080
3.1.2.	 Možnosť 2 – cez klienta
•	./klient s <port>
o	Príklad: ./klient s 8080
3.1.3.	Pripojenie klienta do hry:
•	./client j <port>
o	Príklad: ./client j 8080
3.1.4.	Ukončenie aplikácie:
Použite možnosť e na ukončenie servera alebo klienta:
./server e
./client e 
4.	Herný proces
•	Server čaká na príkaz "start" na začatie hry.
 
Obr.č.2.: Spustenie hry
•	Hráči sa striedajú v hádaní písmen alebo slov.
 
Obr.č.3.:Striedanie hráčov medzi kolami
•	Server synchronizuje stav hry a informuje všetkých hráčov o aktualizáciách.
•	Hra končí, keď hráč uhádne slovo alebo stratí všetky životy.
 
Obr.č.4.: Koniec hry po úspešnom uhádnutí 
5.	Ošetrenie chýb
•	Chyby pri práci so súbormi: Server sa ukončí, ak nie je možné otvoriť súbor so zoznamom slov.
•	Alokácia pamäte: Kontroluje sa úspešná alokácia pamäte; ak nastane chyba, aplikácia sa ukončí.
•	Chyby socketov: Ošetrujú sa chyby pri vytváraní, pripájaní a komunikácii cez sockety.
•	Validácia vstupu: Overuje sa, či je vstup hráča platný (iba písmená alebo slová).
6.	Budúce vylepšenia
1.	Škálovateľnosť:
o	Optimalizácia využitia pamäte pre veľké zoznamy slov.
o	Vylepšenie podpory pre vysoký počet súčasných hráčov.
2.	Herné funkcie:
o	Pridanie nápovedy alebo úrovní obtiažnosti.
o	Implementácia tabuľky víťazov.
3.	Vylepšenia klienta:
o	Zavedenie grafického používateľského rozhrania.
o	Podpora sieťových hier cez internet.
4.	Bezpečnosť:
o	Šifrovanie komunikácie medzi serverom a klientmi.
o	Pridanie autentifikácie hráčov.

