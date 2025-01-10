#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#define MAX_WORD_LENGTH 256
#define MAX_GUESSES 256

typedef struct
{
    char result[MAX_WORD_LENGTH];
    int lives;
    int game_over;
    char guess;
} Message;

// Funkcia na inicializáciu hádaného slova
void initialize_game(char *word_list[], int word_count, char *target_word, char *current_state);

// Funkcia na spracovanie hádania písmena
int process_guess(char guess, const char *target_word, char *current_state, int *lives);

#endif
