#include <stdio.h>
#include <stdlib.h>

int readbyte(void)
{
    int value = getchar();
    if (value == EOF && ferror(stdin)) {
        perror("stdin");
        exit(1);
    }
    return value;
}
