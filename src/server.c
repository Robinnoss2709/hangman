#include "../include/server.h"

// Globálne premenné pre zoznam slov a ich počet
char **word_list = NULL;
int word_count = 0;

// Funkcia na načítanie slov zo súboru
void load_words(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Nepodarilo sa otvoriť súbor so slovami");
        exit(EXIT_FAILURE);
    }

    // Alokácia pamäte pre zoznam slov
    word_list = malloc(MAX_WORDS * sizeof(char *));
    if (!word_list)
    {
        perror("Chyba pri alokácii pamäte pre zoznam slov");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_WORD_LENGTH];
    word_count = 0;

    while (fgets(buffer, sizeof(buffer), file))
    {
        // Odstránenie nového riadka '\n'
        buffer[strcspn(buffer, "\n")] = '\0';

        // Alokácia a kopírovanie slova do zoznamu
        word_list[word_count] = strdup(buffer);
        if (!word_list[word_count])
        {
            perror("Chyba pri alokácii pamäte pre slovo");
            fclose(file);
            exit(EXIT_FAILURE);
        }

        word_count++;
        if (word_count >= MAX_WORDS)
        {
            printf("Varovanie: Maximálny počet slov (%d) dosiahnutý.\n", MAX_WORDS);
            break;
        }
    }

    fclose(file);

    if (word_count == 0)
    {
        fprintf(stderr, "Súbor neobsahuje žiadne slová.\n");
        exit(EXIT_FAILURE);
    }

    printf("Načítaných slov: %d\n", word_count);
}

void *handle_client(void *arg)
{
    int client_socket = *(int *)arg;
    free(arg); // Uvoľnenie pamäte pre argument

    srand(time(NULL));
    char *target_word = word_list[rand() % word_count];
    int word_length = strlen(target_word);
    printf("Cieľové slovo na hádanie: %s\n", target_word);

    char current_state[256];
    for (int i = 0; i < word_length; i++)
    {
        current_state[i] = '_';
    }
    current_state[word_length] = '\0';

    int lives = 10;
    int game_over = 0;
    char guessed_letters[256] = {0};
    int guessed_count = 0;

    Message msg;

    snprintf(msg.result, sizeof(msg.result), "%s", current_state);
    msg.lives = lives;
    msg.game_over = 0;
    send(client_socket, &msg, sizeof(msg), 0);

    while (1)
    {
        ssize_t bytes_received = recv(client_socket, &msg, sizeof(msg), 0);
        if (bytes_received <= 0)
        {
            printf("Klient ukončil spojenie.\n");
            break;
        }

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
            snprintf(msg.result, sizeof(msg.result), "Písmeno '%c' už bolo hádané!", msg.guess);
        }
        else
        {
            guessed_letters[guessed_count++] = msg.guess;

            int correct_guess = 0;
            for (int i = 0; i < word_length; i++)
            {
                if (target_word[i] == msg.guess && current_state[i] == '_')
                {
                    current_state[i] = msg.guess;
                    correct_guess = 1;
                }
            }

            if (!correct_guess)
            {
                lives--;
            }

            snprintf(msg.result, sizeof(msg.result), "%s", current_state);
            msg.lives = lives;

            if (lives <= 0)
            {
                msg.game_over = 1;
                snprintf(msg.result, sizeof(msg.result), "Prehrali ste! Slovo bolo: %s", target_word);
            }
            else if (strcmp(current_state, target_word) == 0)
            {
                msg.game_over = 1;
                snprintf(msg.result, sizeof(msg.result), "Vyhrali ste! Slovo bolo: %s", target_word);
            }
        }

        if (send(client_socket, &msg, sizeof(msg), 0) <= 0)
        {
            perror("Odosielanie správy zlyhalo");
            break;
        }

        if (msg.game_over)
        {
            break;
        }
    }

    close(client_socket); // Uzavretie spojenia pre aktuálneho klienta
    printf("Spojenie s klientom bolo ukončené.\n");
    return NULL;
}

int main()
{
    load_words("words.txt");

    int server_socket, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Vytvorenie socketu zlyhalo");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("Nastavenie socketu zlyhalo");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Priradenie portu zlyhalo");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) < 0)
    {
        perror("Počúvanie zlyhalo");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server beží na porte %d...\n", PORT);

    while (1)
    {
        if ((new_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Prijatie pripojenia zlyhalo");
            continue;
        }

        printf("=============================================\n");
        printf("Nový hráč sa pripojil.\n");

        int *client_socket = malloc(sizeof(int));
        *client_socket = new_socket;

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, client_socket);
        pthread_detach(thread_id); // Oddelenie vlákna, aby sa automaticky ukončilo
    }

    close(server_socket);
    return 0;
}
