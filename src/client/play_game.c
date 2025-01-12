#include "../../include/client.h"

void initialize_game(int socket)
{
    Message msg;

    if (recv(socket, &msg, sizeof(msg), 0) <= 0)
    {
        printf("Spojenie so serverom bolo ukončené.\n");
        return;
    }
}

void play_game(int socket)
{
    Message msg;

    while (1)
    {
        if (recv(socket, &msg, sizeof(msg), 0) <= 0)
        {
            printf("Spojenie so serverom bolo ukončené.\n");
            break;
        }

        printf("\033[1;1H\033[2J");
        printf("=======================================\n");
        printf("Slovo: %s\n", msg.result);
        printf("Počet zostávajúcich životov: %d\n", msg.lives);
        printf("Globálny zoznam hádaných písmen: %s\n", msg.guessed_letters);

        if (msg.game_over)
        {
            if (msg.lives > 0)
            {
                printf("Gratulujeme! Uhádli ste slovo: %s\n", msg.result);
            }
            else
            {
                printf("Prehrali ste. Správne slovo bolo: %s\n", msg.result);
            }
            break;
        }

        if (!msg.guess)
        {
            printf("Nie ste na rade, čakajte...\n");
            continue;
        }

        char input[256];
        printf("Váš tip (písmeno alebo celé slovo): ");
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("Chyba pri čítaní vstupu. Skúste znova.\n");
            continue;
        }

        input[strcspn(input, "\n")] = '\0';

        if (!is_valid_input(input))
        {
            printf("Chyba: Zadajte iba písmená z abecedy!\n");
            continue;
        }

        if (strlen(input) == 1)
        {
            msg.guess = input[0];
            msg.result[0] = '\0';
        }
        else
        {
            strncpy(msg.result, input, sizeof(msg.result));
            msg.guess = '\0';
        }

        if (send(socket, &msg, sizeof(msg), 0) <= 0)
        {
            perror("Odosielanie správy zlyhalo");
            break;
        }
    }

    printf("Hra skončila. Ukončujem klienta.\n");
    close(socket);
    exit(EXIT_SUCCESS);
}
