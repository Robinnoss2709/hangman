#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_WORDS 100
#define MAX_WORD_LENGTH 256

typedef struct
{
    char guess;
    char result[256];
    int lives;
    int game_over;
    char guessed_letters[256];
} Message;

#endif