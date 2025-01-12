#include "../../include/client.h"

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

        pid_t pid = fork();
        if (pid == 0)
        {
            char port_str[10];
            snprintf(port_str, sizeof(port_str), "%d", port);
            execlp("./server", "server", "s", port_str, NULL);
            perror("Spustenie servera zlyhalo");
            exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
            perror("Vytvorenie procesu zlyhalo");
            exit(EXIT_FAILURE);
        }

        sleep(2);
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

    int client_socket;
    struct sockaddr_in server_address;

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Vytvorenie socketu zlyhalo");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

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
    printf("\033[1;1H\033[2J");

    printf("Pripojený k serveru.\n");
    printf("Čakejte kým server spustí hru...\n");

    play_game(client_socket);

    close(client_socket);
    return 0;
}
