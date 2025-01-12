#ifndef GLOBALS_H
#define GLOBALS_H

#include <pthread.h>
#include "globals.h"
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char **word_list;
    int word_count;
    int current_players;
    int *client_sockets;
    pthread_mutex_t player_count_lock;
    int game_started;
    pthread_mutex_t game_start_lock;
    pthread_cond_t game_start_cond;
    int current_turn;
    pthread_mutex_t game_state_lock;
    char current_state[256];
    char guessed_letters[256];
    int guessed_count;
    int *lives;
    char *target_word;
    pthread_mutex_t guessed_letters_lock;
    int game_over_flag;
    pthread_mutex_t game_over_lock;
} GameGlobals;

#define GLOBALS (globals)

extern GameGlobals globals;

void initialize_globals();

#endif