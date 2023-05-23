#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__
    #include <errno.h>
#endif

void ask_string(char *string, int sizeofanswer, char *answer);
void ask_int(char *string, int *answer);