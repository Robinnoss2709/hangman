#include "../../include/server.h"
#include "../../include/globals.h"

void initialize_game()
{
    srand(time(NULL));
    GLOBALS.target_word = GLOBALS.word_list[rand() % GLOBALS.word_count];
    int word_length = strlen(GLOBALS.target_word);

    for (int i = 0; i < word_length; i++)
    {
        GLOBALS.current_state[i] = '_';
    }
    GLOBALS.current_state[word_length] = '\0';

    memset(GLOBALS.guessed_letters, 0, sizeof(GLOBALS.guessed_letters));
    GLOBALS.guessed_count = 0;
    GLOBALS.current_turn = 0;

    pthread_mutex_lock(&GLOBALS.game_over_lock);
    GLOBALS.game_over_flag = 0;
    pthread_mutex_unlock(&GLOBALS.game_over_lock);

    GLOBALS.lives = realloc(GLOBALS.lives, GLOBALS.current_players * sizeof(int));
    for (int i = 0; i < GLOBALS.current_players; i++)
    {
        GLOBALS.lives[i] = 10;
    }

    printf("Hra inicializovaná. Cieľové slovo: %s\n", GLOBALS.target_word);
}

void reset_game_state()
{
    memset(GLOBALS.current_state, 0, sizeof(GLOBALS.current_state));
    memset(GLOBALS.guessed_letters, 0, sizeof(GLOBALS.guessed_letters));
    GLOBALS.guessed_count = 0;
    GLOBALS.current_turn = 0;
    GLOBALS.game_started = 0;
    GLOBALS.game_over_flag = 0;
    if (GLOBALS.lives != NULL)
    {
        free(GLOBALS.lives);
        GLOBALS.lives = NULL;
    }
    if (GLOBALS.client_sockets != NULL)
    {
        free(GLOBALS.client_sockets);
        GLOBALS.client_sockets = NULL;
    }
    GLOBALS.current_players = 0;
}
