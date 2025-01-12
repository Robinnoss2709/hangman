#include "../../include/client.h"

int is_valid_input(const char *input)
{
    for (int i = 0; input[i] != '\0'; i++)
    {
        if (!isalpha(input[i]))
            return 0;
    }
    return 1;
}
