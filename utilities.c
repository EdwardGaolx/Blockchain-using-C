#include "utilities.h"
#include <stdlib.h>

char *randomString(int length)
{
    static char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    char *str = NULL;

    if (length)
    {
        str = malloc(sizeof(char) * (length + 1));

        if (str)
        {
            for (int n = 0; n < length; n++)
            {
                int key = rand() % (int)(sizeof(characters) - 1);
                str[n] = characters[key];
            }

            str[length] = '\0';
        }
    }
    return str;
}