#include "../../include/globals.h"

GameGlobals globals = {
    .word_list = NULL,
    .word_count = 0,
    .current_players = 0,
    .client_sockets = NULL,
    .player_count_lock = PTHREAD_MUTEX_INITIALIZER,
    .game_started = 0,
    .game_start_lock = PTHREAD_MUTEX_INITIALIZER,
    .game_start_cond = PTHREAD_COND_INITIALIZER,
    .current_turn = 0,
    .game_state_lock = PTHREAD_MUTEX_INITIALIZER,
    .current_state = {0},
    .guessed_letters = {0},
    .guessed_count = 0,
    .lives = NULL,
    .target_word = NULL,
    .guessed_letters_lock = PTHREAD_MUTEX_INITIALIZER,
    .game_over_flag = 0,
    .game_over_lock = PTHREAD_MUTEX_INITIALIZER};

void initialize_globals()
{
    globals.word_list = NULL;
    globals.word_count = 0;
    globals.current_players = 0;
    globals.client_sockets = NULL;
    pthread_mutex_init(&globals.player_count_lock, NULL);
    globals.game_started = 0;
    pthread_mutex_init(&globals.game_start_lock, NULL);
    pthread_cond_init(&globals.game_start_cond, NULL);
    globals.current_turn = 0;
    pthread_mutex_init(&globals.game_state_lock, NULL);
    memset(globals.current_state, 0, sizeof(globals.current_state));
    memset(globals.guessed_letters, 0, sizeof(globals.guessed_letters));
    globals.guessed_count = 0;
    globals.lives = NULL;
    globals.target_word = NULL;
    pthread_mutex_init(&globals.guessed_letters_lock, NULL);
    globals.game_over_flag = 0;
    pthread_mutex_init(&globals.game_over_lock, NULL);
}
