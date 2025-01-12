#include "../../include/server.h"
#include "../../include/globals.h"

void *handle_client(void *arg)
{
    int client_socket = *(int *)arg;
    free(arg);

    pthread_mutex_lock(&GLOBALS.player_count_lock);
    GLOBALS.client_sockets = realloc(GLOBALS.client_sockets, (GLOBALS.current_players + 1) * sizeof(int));
    GLOBALS.lives = realloc(GLOBALS.lives, (GLOBALS.current_players + 1) * sizeof(int));
    GLOBALS.client_sockets[GLOBALS.current_players] = client_socket;
    GLOBALS.lives[GLOBALS.current_players] = 10;
    int player_id = GLOBALS.current_players;
    GLOBALS.current_players++;
    pthread_mutex_unlock(&GLOBALS.player_count_lock);

    pthread_mutex_lock(&GLOBALS.game_start_lock);
    while (!GLOBALS.game_started)
    {
        pthread_cond_wait(&GLOBALS.game_start_cond, &GLOBALS.game_start_lock);
    }
    pthread_mutex_unlock(&GLOBALS.game_start_lock);

    Message msg;

    while (1)
    {
        pthread_mutex_lock(&GLOBALS.game_state_lock);

        pthread_mutex_lock(&GLOBALS.guessed_letters_lock);
        snprintf(msg.result, sizeof(msg.result), "%s", GLOBALS.current_state);
        msg.lives = GLOBALS.lives[player_id];
        msg.game_over = 0;
        strcpy(msg.guessed_letters, GLOBALS.guessed_letters);
        pthread_mutex_unlock(&GLOBALS.guessed_letters_lock);

        msg.guess = (GLOBALS.current_turn == player_id);

        if (send(client_socket, &msg, sizeof(msg), 0) <= 0)
        {
            printf("Chyba pri odosielaní údajov hráčovi %d.\n", player_id);
            pthread_mutex_unlock(&GLOBALS.game_state_lock);
            break;
        }

        pthread_mutex_lock(&GLOBALS.game_over_lock);
        if (GLOBALS.game_over_flag)
        {
            pthread_mutex_unlock(&GLOBALS.game_over_lock);
            pthread_mutex_unlock(&GLOBALS.game_state_lock);
            break;
        }
        pthread_mutex_unlock(&GLOBALS.game_over_lock);

        if (msg.guess == 0)
        {
            pthread_mutex_unlock(&GLOBALS.game_state_lock);
            usleep(1000);
            continue;
        }

        ssize_t bytes_received = recv(client_socket, &msg, sizeof(msg), 0);
        if (bytes_received <= 0)
        {
            printf("Hráč %d ukončil spojenie.\n", player_id);
            break;
        }

        if (strlen(msg.result) > 0)
        {
            if (strcasecmp(msg.result, GLOBALS.target_word) == 0)
            {
                pthread_mutex_lock(&GLOBALS.game_over_lock);
                GLOBALS.game_over_flag = 1;
                pthread_mutex_unlock(&GLOBALS.game_over_lock);
                msg.game_over = 1;
                snprintf(msg.result, sizeof(msg.result), "%s", GLOBALS.target_word);

                for (int i = 0; i < GLOBALS.current_players; i++)
                {
                    if (GLOBALS.client_sockets[i] != -1)
                    {
                        send(GLOBALS.client_sockets[i], &msg, sizeof(msg), 0);
                    }
                }
                pthread_mutex_unlock(&GLOBALS.game_state_lock);
                break;
            }
            else
            {
                GLOBALS.lives[player_id]--;
                if (GLOBALS.lives[player_id] <= 0)
                {
                    pthread_mutex_lock(&GLOBALS.game_over_lock);
                    GLOBALS.game_over_flag = 1;
                    pthread_mutex_unlock(&GLOBALS.game_over_lock);

                    msg.game_over = 1;
                    snprintf(msg.result, sizeof(msg.result), "%s", GLOBALS.target_word);
                }
            }
        }
        else
        {
            char guessed_letter = msg.guess;
            int already_guessed = 0;

            for (int i = 0; i < GLOBALS.guessed_count; i++)
            {
                if (GLOBALS.guessed_letters[i] == guessed_letter)
                {
                    already_guessed = 1;
                    break;
                }
            }
            if (!already_guessed)
            {
                pthread_mutex_lock(&GLOBALS.guessed_letters_lock);
                GLOBALS.guessed_letters[GLOBALS.guessed_count++] = guessed_letter;
                GLOBALS.guessed_letters[GLOBALS.guessed_count] = '\0';
                pthread_mutex_unlock(&GLOBALS.guessed_letters_lock);

                int correct_guess = 0;
                for (int i = 0; i < strlen(GLOBALS.target_word); i++)
                {
                    if (GLOBALS.target_word[i] == guessed_letter && GLOBALS.current_state[i] == '_')
                    {
                        GLOBALS.current_state[i] = guessed_letter;
                        correct_guess = 1;
                    }
                }

                if (!correct_guess)
                {
                    GLOBALS.lives[player_id]--;
                }
            }
        }

        if (strcmp(GLOBALS.current_state, GLOBALS.target_word) == 0 || GLOBALS.lives[player_id] <= 0)
        {
            pthread_mutex_lock(&GLOBALS.game_over_lock);
            GLOBALS.game_over_flag = 1;
            pthread_mutex_unlock(&GLOBALS.game_over_lock);

            msg.game_over = 1;
        }

        msg.lives = GLOBALS.lives[player_id];
        snprintf(msg.result, sizeof(msg.result), "%s", GLOBALS.current_state);

        if (msg.game_over)
        {
            for (int i = 0; i < GLOBALS.current_players; i++)
            {
                if (GLOBALS.client_sockets[i] != -1)
                {
                    send(GLOBALS.client_sockets[i], &msg, sizeof(msg), 0);
                }
            }
            pthread_mutex_unlock(&GLOBALS.game_state_lock);
            break;
        }

        GLOBALS.current_turn = (GLOBALS.current_turn + 1) % GLOBALS.current_players;
        pthread_mutex_unlock(&GLOBALS.game_state_lock);
    }

    close(client_socket);
    pthread_mutex_lock(&GLOBALS.player_count_lock);
    GLOBALS.client_sockets[player_id] = -1;
    GLOBALS.current_players--;
    pthread_mutex_unlock(&GLOBALS.player_count_lock);
    printf("Spojenie s hráčom %d bolo ukončené.\n", player_id);
    return NULL;
}
