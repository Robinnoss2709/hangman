#include "../../include/server.h"
#include "../../include/globals.h"

void load_words(const char *filename)
{
    printf("\033[1;1H\033[2J");
    printf("Načítavam súbor: %s\n", filename);
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Nepodarilo sa otvoriť súbor so slovami");
        exit(EXIT_FAILURE);
    }

    GLOBALS.word_list = malloc(MAX_WORDS * sizeof(char *));
    if (!GLOBALS.word_list)
    {
        perror("Chyba pri alokácii pamäte pre zoznam slov");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_WORD_LENGTH];
    GLOBALS.word_count = 0;

    while (fgets(buffer, sizeof(buffer), file))
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        GLOBALS.word_list[GLOBALS.word_count] = strdup(buffer);
        if (!GLOBALS.word_list[GLOBALS.word_count])
        {
            perror("Chyba pri alokácii pamäte pre slovo");
            fclose(file);
            exit(EXIT_FAILURE);
        }
        GLOBALS.word_count++;
        if (GLOBALS.word_count >= MAX_WORDS)
        {
            printf("Varovanie: Maximálny počet slov (%d) dosiahnutý.\n", MAX_WORDS);
            break;
        }
    }

    fclose(file);

    if (GLOBALS.word_count == 0)
    {
        fprintf(stderr, "Súbor neobsahuje žiadne slová.\n");
        exit(EXIT_FAILURE);
    }

    printf("Načítaných slov: %d\n", GLOBALS.word_count);
}
