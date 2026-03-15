#include <err.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"

char *alloc_string(void)
{
   char *string = malloc(sizeof(char)); 
   if (!string) err(EXIT_FAILURE, "Error allocating string");

   string[0] = '\0';

   return string;
}

char *append_char(char *str, char c)
{
    size_t len = strlen(str);
    char *new_string = realloc(str, len + 2);
    if(!new_string) err(EXIT_FAILURE, "Error reallocating string");

    new_string[len] = c;
    new_string[len+1] = '\0';

    return new_string;
}

char *append_string(char *location, char *str)
{
    size_t size = strlen(location) + strlen(str) + 1;
    char *new_string = realloc(location, size);
    if(!new_string) err(EXIT_FAILURE, "Error reallocating string");

    strcat(new_string, str);
    return new_string;
}

void number_to_bits(char *bits, short value)
{
    const short ASCII_OFFSET = '0';
    int i = strlen(bits) - 1; // -1 because MSB defines instruction type
    while(i) {
        bits[i] = (value % 2) + ASCII_OFFSET;
        value = value / 2;
        i--;
    }
}
