#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "game.h"
#include "globals.h"

int is_valid_input(const char *input);

void play_game(int socket);

#endif