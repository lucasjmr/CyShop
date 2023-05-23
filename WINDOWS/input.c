#include "input.h"

void ask_string(char *string, int sizeofanswer, char *answer)
{
    if (string == NULL || answer == NULL || sizeofanswer <= 0)
    {
        printf("Parameters error in ask_string\n");
        exit(1);
    }

    int valid = 0;
    int c = 0;
    char format[16]; // format is the string wich will have the custom value. 16 is more than enough to create a string : " %[number]s"

    do
    {
        printf("%s", string);

        // valid = scanf(" %15s", answer); takes only the first 15 bytes (not 16 because there is \0 at the end). It is working only for 16 bytes answers.
        // We need to dynamically change this value depending of sizeofanswer. Example : scanf(" %31s", answer); for 32 bytes answer
        // Problem : The value is IN a string... And we can't use %d in scanf with the way we want.
        // So we will create a string with sizeofanswer value. We will use snprintf();

        snprintf(format, sizeof(format), " %%%ds", sizeofanswer - 1); // snprintf and not sprintf to avoid shit in the buffer
        // %% is read as regular % character. This is why there are 3 % and not 2
        valid = scanf(format, answer);

        while ((c = getchar()) != '\n' && c != EOF) // Clears the buffer
        {
        }
        if (valid != 1) // No idea how a string input couldn't be valid but I still check
        {
            printf("The input is not valid.\n");
        }
    } while (valid != 1);
}

void ask_int(char *string, int *answer)
{
    if (string == NULL || answer == NULL)
    {
        printf("Parameters error in ask_int\n");
        exit(1);
    }

    int valid = 0;
    int c = 0;

    do
    {
        printf("%s", string);
        valid = scanf(" %d", answer);
        if (valid != 1)
        {
            printf("The number isnt valid.\n");
            while (c != EOF && c != '\n')
            {
                c = getchar();
            }
        }
    } while (valid != 1);
}
