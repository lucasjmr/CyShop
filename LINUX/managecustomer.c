#include "managecustomer.h"

int client_number_to_assign()
{
    int number_client = 0;
    int valid = 1;

    FILE *file = fopen("shop-data/number_of_client.txt", "r");
    if (file == NULL)
    {
        printf("\033[41m\033[1mCoudn't open number_of_client.txt file : %s\033[0m\n", strerror(errno));
        exit(1);
    }

    valid &= (fscanf(file, "%d", &number_client) == 1);
    if (valid == 0)
    {
        printf("\033[41m\033[1mCouldn't read number_of_client.txt file.\033[0m\n");
        exit(1);
    }

    fclose(file);
    return number_client + 1;
}

int is_client()
{
    char answer[16];
    char name[64];
    int age = 0;
    int number = client_number_to_assign();
    int test = 0;
    int valid = 0;
    int known_number = 0;

    while (1)
    {
        ask_string("Are you a new client ? (\033[32my\033[0m/\033[31mn\033[0m) : ", sizeof(answer), answer);
        if (!strcmp("y", answer))
        {
            // gets the name without the spaces
            do
            {
                ask_string("Enter your \033[0;32mname\033[0m (without spaces) : ", sizeof(name), name);
                if (strchr(name, ' ') == NULL) // Useless because scanf doesnt store data after the spaces. Still check ...
                {
                    test = 1;
                }
                else
                {
                    printf("Nice try Mr.Grignon !\n");
                }

            } while (test == 0);

            // gets other info :
            ask_int("Enter your \033[0;32mage\033[0m : ", &age);

            // creates client
            create_client(name, age, number);
            printf("Your client number is \033[1;33m%d\033[0m. \033[0;31mKeep it in mind !\033[0m\n", number);
            // Adds 1 to the number_of_client.txt file. So future clients will get different number.
            FILE *file = fopen("shop-data/number_of_client.txt", "w");
            if (file == NULL)
            {
                printf("\033[41m\033[1mFailed to open number_of_client.txt file.\033[0m\n");
                exit(1);
            }

            valid = fprintf(file, "%d", number);
            if (valid != 1)
            {
                printf("\033[41m\033[1mWriting in number_of_client.txt failed.\033[0m\n");
                exit(1);
            }
            fclose(file);

            return number;
        }
        else if (!strcmp("n", answer))
        {
            ask_int("Please provide your \033[1;33mclient number\033[0m : ", &known_number);
            return known_number;
        }
        else
        {
            printf("\033[1mYou must enter '\033[32my\033[0m' or '\033[41mn\033[0m'.\n");
        }
    }
}

void create_client(char *name, int age, int client_number) // Creates a file for a client
{
    if (name == NULL || age <= 0 || client_number < 0)
    {
        printf("\033[41m\033[1mParameters error in create_client\033[0m\n");
        exit(1);
    }

    int check;
    char format[32];

    // Create the string with the custom value
    sprintf(format, "shop-data/client-%d.txt", client_number);

    // Open file and check for errors
    FILE *file = NULL;
    file = fopen(format, "w");
    if (file == NULL)
    {
        printf("\033[41m\033[1mFailed to create client file : %s\033[0m\n", strerror(errno));
        exit(1);
    }

    // Write the values

    check = fprintf(file, "%s %d %d\n", name, age, client_number); // Dont forget \n to append values easier later
    if (check < 0)
    {
        printf("\033[41m\033[1mError while trying to write string in client file.\033[0m\n");
        exit(1);
    }
    fclose(file);
}

void delete_client(int client_number) // Deletes client file
{
    if (client_number < 0)
    {
        printf("\033[41m\033[1mParameters error in delete_client.\033[0m\n");
        exit(1);
    }

    int check = 1;
    char format[32];

    // Create string with file name
    sprintf(format, "shop-data/client-%d.txt", client_number);

    check = remove(format);
    if (check != 0)
    {
        printf("\033[41m\033[1mFailed to delete client file.\033[0m\n");
        exit(1);
    }
}

int number_of_buys(int client_number) // Returns the number of lines of shop-data/client-%d.txt = number of lines - 1
{
    if (client_number < 0)
    {
        printf("\033[41m\033[1mParameters error in number_of_buys.\033[0m\n");
        exit(1);
    }

    char format[32];
    int c;
    int flag = 0;
    int count = 0;

    sprintf(format, "shop-data/client-%d.txt", client_number);

    // Open file (read)
    FILE *file = NULL;
    file = fopen(format, "r");
    if (file == NULL)
    {
        printf("\033[41m\033[1mCould not open client file : %s/wrong client_number\033[0m\n", strerror(errno));
        exit(1);
    }

    // Counts the number of lines
    for (c = fgetc(file); c != EOF; c = fgetc(file))
    {

        if (c == '\n') // If the char is a '\n' and if the line contains a char different from a space, tabulation...
        {
            if (flag)
            {
                count++;
                flag = 0;
            }
        }
        else if (c != ' ' && c != '\t' && c != '\r') // If there is caracter in the line != space, tabulation, carriage return
        {
            flag = 1;
        }
    }
    if (flag) // If the last line doesnt contain a '\n' at the end but isn't empty -> count++
    {
        count++;
    }

    fclose(file);

    return count - 1;
}

void print_last_buy(int client_number)
{
    if (client_number < 0)
    {
        printf("\033[41m\033[1mParameters error in print_last_buy.\033[0m\n");
        exit(1);
    }

    int buys = number_of_buys(client_number);
    char format[32];
    char buy1[32];
    char buy2[32];
    char buy3[32];
    int valid = 1;

    sprintf(format, "shop-data/client-%d.txt", client_number);

    FILE *file = NULL;
    file = fopen(format, "r");
    if (file == NULL)
    {
        printf("\033[41m\033[1mError while trying to open client file : %s\033[0m\n", strerror(errno));
        exit(1);
    }

    // Notes for below :
    // %[^\n] gets all characters until \n. %*[^\n] -> the '*' indicates caracters will be read but not stored
    // %*c It reads a character (still not stored, so discarded -> the \n (due to what we wrote before)).
    // The combinaison -> reads all characters until \n (not stored). Then it reads the \n with %*c (still not stored).

    switch (buys)
    {
    case 1:
        valid &= (fscanf(file, "%*[^\n]%*c") == 0); // Skip first line
        valid &= (fscanf(file, "%s", buy1) == 1);
        if (valid == 0)
        {
            printf("\033[41m\033[1mError while trying to read the last GPU bought.\033[0m\n");
            exit(1);
        }
        printf("Welcome back. You bought \033[0;36m %s \033[0m before.\n", buy1);
        break;
    case 2:
        valid &= (fscanf(file, "%*[^\n]%*c") == 0); // Skip first line
        valid &= (fscanf(file, "%s", buy1) == 1);
        valid &= (fscanf(file, "%s", buy2) == 1);
        if (valid == 0)
        {
            printf("\033[41m\033[1mError while trying to read the last GPU bought.\033[0m\n");
            exit(1);
        }
        printf("Welcome back. You bought \033[0;36m%s\033[0m and \033[0;36m%s\033[0m before.\n", buy1, buy2);
        break;
    case 3:
        valid &= (fscanf(file, "%*[^\n]%*c") == 0); // Skip first line
        valid &= (fscanf(file, "%s", buy1) == 1);
        valid &= (fscanf(file, "%s", buy2) == 1);
        valid &= (fscanf(file, "%s", buy3) == 1);
        if (valid == 0)
        {
            printf("\033[41m\033[1mError while trying to read the last GPU bought.\033[0m\n");
            exit(1);
        }
        printf("Welcome back. You bought \033[0;36m%s\033[0m, \033[0;36m%s\033[0m, and \033[0;36m%s\033[0m before <3.\n", buy1, buy2, buy3);
    default:
        if (buys > 3)
        {
            valid &= (fscanf(file, "%*[^\n]%*c") == 0); // Skips first line
            for (int i = 0; i < buys - 3; i++)
            {
                valid &= (fscanf(file, "%*[^\n]%*c") == 0); // Skips more lines : [number of gpus bought] - 3
            }
            valid &= (fscanf(file, "%s", buy1) == 1);
            valid &= (fscanf(file, "%s", buy2) == 1);
            valid &= (fscanf(file, "%s", buy3) == 1);
            if (valid == 0)
            {
                printf("\033[41m\033[1mError while trying to read the last GPU bought.\033[0m\n");
                exit(1);
            }
            printf("Welcome back. You bought \033[0;36m%s\033[0m, \033[0;36m%s\033[0m, and \033[0;36m%s\033[0m before <3.\n", buy1, buy2, buy3);
        }
    }

    fclose(file);
}

void client_gpu_append(char *string, int client_number) // Appends gpu name in client file (list buys)
{
    if (string == NULL || client_number < 0)
    {
        printf("\033[41m\033[1mParameters error in client_gpu_append\033[0m\n");
        exit(1);
    }

    char format[16];
    int check = 1;
    sprintf(format, "shop-data/client-%d.txt", client_number);

    FILE *file = NULL;
    file = fopen(format, "a");
    if (file == NULL)
    {
        printf("\033[41m\033[1mError while trying to open client file : %s\033[0m\n", strerror(errno));
        exit(1);
    }
    check = fprintf(file, "%s\n", string); // appends gpu name to the client file
    if (check < 0)
    {
        printf("\033[41m\033[1mError while trying to append GPU name in client file.\033[0m\n");
        exit(1);
    }

    fclose(file);
}

void search_and_buy(GPU *GPUarray, int client_number)
{
    if (GPUarray == NULL || client_number < 0)
    {
        printf("\033[41m\033[1mParameters error in search_and_buy\033[0m\n");
        exit(1);
    }

    int size = number_of_GPU();
    int found = 0;
    char WantedGPU[16];
    int answered = 0;
    char y_or_n[16];

    do
    {
        ask_string("Enter the name of the GPU you want : ", sizeof(WantedGPU), WantedGPU);

        for (int i = 0; i < size; i++) // search for GPU with wanted name or wanted number
        {
            if (!strcmp(GPUarray[i].name, WantedGPU))
            {
                found = 1;
                if (GPUarray[i].quantity == 0)
                {
                    printf("The \033[1;31m%s\033[0m isn't in stock :(\n", GPUarray[i].name);

                    do
                    {
                        ask_string("If you are not satisfied, you can unsubsribe from our database (\033[32my\033[0m/\033[31mn\033[0m) : ", sizeof(y_or_n), y_or_n);
                        if (!strcmp("y", y_or_n))
                        {
                            delete_client(client_number);
                            printf("\033[1mYou have been deleted from database.\033[0m\n");
                            answered = 1;
                        }
                        else if (!strcmp("n", y_or_n))
                        {
                            printf("Thank you ! We will try to restock as soon as possible.\n");
                            answered = 1;
                        }
                        else
                        {
                            printf("\033[1mYou need to enter '\033[32my\033[0m' or '\033[31mn\033[0m'.\n");
                        }
                    } while (answered == 0);
                }
                else
                {
                    do
                    {
                        printf("\033[1m%d\033[1;36m %s\033[0m are in stock.", GPUarray[i].quantity, GPUarray[i].name);

                        ask_string("Would you like to buy one ? (\033[32my\033[0m/\033[31mn\033[0m) : ", sizeof(y_or_n), y_or_n);
                        if (!strcmp("n", y_or_n))
                        {
                            printf("You are right, Nvidia Gpus are way too expensive for the performance. AMD is better ;)\n");
                            answered = 1;
                        }
                        else if (!strcmp("y", y_or_n))
                        {
                            client_gpu_append(WantedGPU, client_number);
                            GPUarray[i].quantity -= 1;
                            overwrite_stock(GPUarray);
                            printf("You successfully bought it for \033[0;32m%d\033[0m, \xF8o\xF8\n", GPUarray[i].price);
                            answered = 1;
                        }
                        else
                        {
                            printf("\033[1mYou need to enter '\033[32my\033[0m' or '\033[31mn\033[0m'.\n");
                        }
                    } while (answered == 0);
                }
            }
        }
        if (found == 0)
        {
            printf("\033[1mNo GPU with this name have been found.\033[0m\n");
        }
    } while (found == 0);
}
