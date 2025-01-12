#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <time.h>
#include "game.h"
#include "globals.h"

#define MAX_WORDS 100
#define MAX_WORD_LENGTH 256
#define PORT 8080

void load_words(const char *filename);

void *handle_client(void *arg);

int can_continue_game();

#endif
