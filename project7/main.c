#include <stdio.h>
#include "translator.h"

int main()
{
    int c;
    while((c = fgetc(stdin)) != EOF)
    {
        printf("%c", c);
    }

    return 0;
}
