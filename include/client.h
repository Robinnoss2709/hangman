#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "game.h"

// Funkcia na kontrolu, či je znak písmeno
int is_valid_letter(char c);

// Funkcia na hranie hry (hlavná herná logika klienta)
void play_game(int socket);

#endif // CLIENT_H
