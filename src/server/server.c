#include "../../include/server.h"
#include "../../include/globals.h"
#include "load_words.c"
#include "handle_client.c"
#include "game_utils.c"

void free_words()
{
    for (int i = 0; i < GLOBALS.word_count; i++)
    {
        free(GLOBALS.word_list[i]);
    }
    free(GLOBALS.word_list);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Použitie: %s <s|j|e> [port]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char option = argv[1][0];
    int port = PORT;

    if (option == 's')
    {
        if (argc < 3)
        {
            fprintf(stderr, "Použitie: %s s <port>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        port = atoi(argv[2]);
    }
    else if (option == 'j')
    {
        if (argc < 3)
        {
            fprintf(stderr, "Použitie: %s j <port>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        port = atoi(argv[2]);
    }
    else if (option == 'e')
    {
        printf("Ukončujem program.\n");
        exit(EXIT_SUCCESS);
    }
    else
    {
        fprintf(stderr, "Neplatná možnosť: %c\n", option);
        exit(EXIT_FAILURE);
    }

    initialize_globals();
    load_words("/home/robko/HANGMAN/words.txt");

    int server_socket, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Vytvorenie socketu zlyhalo");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    {
        perror("Nastavenie socketu zlyhalo");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

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

    printf("Server beží na porte %d...\n", port);

    while (1)
    {
        printf("Napíšte 'start' na spustenie hry: ");
        char command[10];
        while (1)
        {
            scanf("%s", command);
            if (strcmp(command, "start") == 0)
            {
                pthread_mutex_lock(&GLOBALS.game_start_lock);
                GLOBALS.game_started = 1;
                pthread_cond_broadcast(&GLOBALS.game_start_cond);
                pthread_mutex_unlock(&GLOBALS.game_start_lock);
                initialize_game();
                printf("Hra sa začala!\n");
                break;
            }
        }

        while (1)
        {
            pthread_mutex_lock(&GLOBALS.game_over_lock);
            if (GLOBALS.game_over_flag)
            {
                pthread_mutex_unlock(&GLOBALS.game_over_lock);
                for (int i = 0; i < GLOBALS.current_players; i++)
                {
                    if (GLOBALS.client_sockets[i] != -1)
                    {
                        close(GLOBALS.client_sockets[i]);
                        GLOBALS.client_sockets[i] = -1;
                    }
                }
                printf("Hra skončila, všetky spojenia sú uzavreté.\n");
                reset_game_state();
                break;
            }
            pthread_mutex_unlock(&GLOBALS.game_over_lock);

            if ((new_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
            {
                perror("Prijatie pripojenia zlyhalo");
                continue;
            }

            int *client_socket = malloc(sizeof(int));
            *client_socket = new_socket;

            pthread_t thread_id;
            pthread_create(&thread_id, NULL, handle_client, client_socket);
            pthread_detach(thread_id);
        }
    }

    close(server_socket);
    free_words();
    printf("Server ukončený a port uvoľnený.\n");
    return 0;
}
