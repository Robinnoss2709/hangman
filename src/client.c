#include "../include/client.h"

// Funkcia na kontrolu, či je znak písmeno
int is_valid_letter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void play_game(int socket)
{
    Message msg;
    char guessed_letters[256] = {0}; // Zoznam hádaných písmen
    int guessed_count = 0;           // Počet unikátnych hádaných písmen

    // Počiatočný stav slova
    if (recv(socket, &msg, sizeof(msg), 0) <= 0)
    {
        printf("Spojenie so serverom bolo ukončené.\n");
        return;
    }
    printf("=============================================\n");
    printf("Slovo: %s\n", msg.result);

    while (1)
    {
        printf("Váš tip (1 znak): ");
        scanf(" %c", &msg.guess);

        // Kontrola, či zadaný znak je platné písmeno
        if (!is_valid_letter(msg.guess))
        {
            printf("Chyba: Zadajte iba písmená z abecedy!\n");
            continue;
        }

        // Konvertovanie na malé písmeno pre jednotnosť (voliteľné)
        msg.guess = tolower(msg.guess);

        // Kontrola, či už bolo písmeno hádané
        int already_guessed = 0;
        for (int i = 0; i < guessed_count; i++)
        {
            if (guessed_letters[i] == msg.guess)
            {
                already_guessed = 1;
                break;
            }
        }

        if (already_guessed)
        {
            printf("Písmeno '%c' už bolo hádané. Skúste iné.\n", msg.guess);
            continue; // Neposielame písmeno na server
        }

        // Uloženie písmena medzi hádané
        guessed_letters[guessed_count++] = msg.guess;

        // Odoslanie písmena na server
        if (send(socket, &msg, sizeof(msg), 0) <= 0)
        {
            perror("Odosielanie správy zlyhalo");
            break;
        }

        // Prijatie odpovede od servera
        if (recv(socket, &msg, sizeof(msg), 0) <= 0)
        {
            printf("Spojenie so serverom bolo ukončené.\n");
            break;
        }

        printf("Server odpovedal: %s\n", msg.result);
        printf("Počet zostávajúcich životov: %d\n", msg.lives);

        // Zobrazenie hádaných písmen s medzerou
        printf("Hádané písmená: ");
        for (int i = 0; i < guessed_count; i++)
        {
            printf("%c ", guessed_letters[i]);
        }
        printf("\n");
        printf("=============================================\n");

        if (msg.game_over)
        {
            break;
        }
    }
}

int main()
{
    int client_socket;
    struct sockaddr_in server_address;

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Vytvorenie socketu zlyhalo");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0)
    {
        perror("Neplatná adresa alebo adresa nepodporovaná");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Pripojenie k serveru zlyhalo");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Pripojený k serveru.\n");
    play_game(client_socket);

    close(client_socket);
    return 0;
}
