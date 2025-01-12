# Hangman Multiplayer

Hangman Multiplayer je aplikácia typu server-klient vyvinutá v jazyku C, ktorá umožňuje viacerým hráčom súčasne hrať klasickú hru na hádanie slov. Server spravuje stav hry, zatiaľ čo klienti komunikujú so serverom, aby zadávali svoje hádania. Hra využíva súbežné spracovanie, správu pamäte a komunikačné protokoly na zabezpečenie plynulého multiplayerového zážitku.

## Kompilácia

Na kompiláciu serverovej a klientskej aplikácie využite CMake. Postupujte podľa nasledujúcich krokov:

1. Vytvorte súbor `CMakeLists.txt` s nasledovným obsahom:
   ```cmake
   cmake_minimum_required(VERSION 3.10)

   project(HangmanGame)

   set(CMAKE_C_STANDARD 11)

   add_executable(server src/server/server.c src/server/globals.c)
   add_executable(client src/client/client.c src/client/play_game.c src/client/utils.c)

   target_link_libraries(server pthread)
   
2. Vytvorte adresár pre build:
   ```bash
   mkdir build

3. Prejdite do adresára build:
   ```bash
    cd build

4. Vygenerujte build súbory:
   ```bash
    cmake ..

5. Kompilujte aplikáciu:
   ```bash
    make

## Spustenie hry
Spustenie servera
Server môžete spustiť dvoma spôsobmi:
1. Priamo cez server:
   ```bash
    ./server s <port>
2. Cez klienta
   ```bash
    ./client s <port>
## Pripojenie klienta do hry
3. Klient sa môže pripojiť k serveru pomocou príkazu:
   ```bash
    ./client j <port>

## Ukončenie aplikácie
4. Na ukončenie aplikácie použite možnosť e:
   ```bash
    ./server e

## Herný proces
Server čaká na príkaz start na začatie hry. Hráči sa striedajú v hádaní písmen alebo slov. Server synchronizuje stav hry a informuje všetkých hráčov o aktualizáciách. Hra končí, keď hráč uhádne slovo alebo stratí všetky životy.

## Ošetrenie chýb
Chyby pri práci so súbormi: Server sa ukončí, ak nie je možné otvoriť súbor so zoznamom slov.
Alokácia pamäte: Kontroluje sa úspešnosť alokácie pamäte pre každú operáciu.