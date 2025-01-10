#include "game_logic.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

void initialize_game(char *word_list[], int word_count, char *target_word, char *current_state)
{
    srand(time(NULL));
    strcpy(target_word, word_list[rand() % word_count]);
    int word_length = strlen(target_word);
    for (int i = 0; i < word_length; i++)
    {
        current_state[i] = '_';
    }
    current_state[word_length] = '\0';
}

int process_guess(char guess, const char *target_word, char *current_state, int *lives)
{
    int correct_guess = 0;
    int word_length = strlen(target_word);

    for (int i = 0; i < word_length; i++)
    {
        if (tolower(target_word[i]) == tolower(guess) && current_state[i] == '_')
        {
            current_state[i] = target_word[i];
            correct_guess = 1;
        }
    }

    if (!correct_guess)
    {
        (*lives)--;
    }

    return correct_guess;
}
