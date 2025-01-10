#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

typedef struct
{
    char guess;
    char result[256];
    int lives;
    int game_over;
} Message;

#endif
