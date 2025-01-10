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

// Makro pre maximálny počet slov a dĺžku slova
#define MAX_WORDS 100
#define MAX_WORD_LENGTH 256

// Globálny zoznam slov pre hru
extern char **word_list;
extern int word_count;

// Funkcia pre načítanie slov zo súboru
void load_words(const char *filename);

// Funkcia pre spracovanie klienta
void *handle_client(void *arg);

#endif // SERVER_H
