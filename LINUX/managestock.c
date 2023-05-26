#include "managestock.h"

void overwrite_stock(GPU *GPUarray) // Deletes all the lines in file and write new ones with the GPUs strings in GPU array
{
    if (GPUarray == NULL)
    {
        printf("\033[41m\033[1mParameters error in overwrite_stock\033[0m\n");
        exit(1);
    }

    int size = number_of_GPU();
    int check;

    // Recreate the file with the GPUarray values in parameter
    FILE *file = fopen("shop-data/stocks.txt", "w"); // Write access
    if (file == NULL)                                // Check for errors while opening file
    {
        printf("\033[41m\033[1mCould not open stocks file while trying to overwrite it : %s\033[0m\n", strerror(errno));
        exit(1);
    }

    for (int i = 0; i < size; i++)
    {
        check = fprintf(file, "%s %d %d %d %d\n", GPUarray[i].name, GPUarray[i].number, GPUarray[i].quantity, GPUarray[i].price, GPUarray[i].size); // Writes the lines in file
        if (check < 0)
        {
            printf("\033[0mError while trying to overwrite stocks file.\033[0m\n");
            exit(1);
        }
    }
    fclose(file);
}

int number_of_GPU(void) // Returns the number of lines of stocks.txt = number of products/GPU
{
    FILE *file = NULL;
    file = fopen("shop-data/stocks.txt", "r");
    if (file == NULL)
    {
        printf("\033[0mCould not open stocks file : %s\033[0m\n", strerror(errno));
        exit(1);
    }

    int c;
    int flag = 0;
    int count = 0;

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
    return count;
}

GPU *create_GPU_array(void) // Returns an array of all GPUs
{
    int nbrGPU = number_of_GPU();
    int valid = 1;

    GPU *array = NULL;
    array = malloc(nbrGPU * sizeof(GPU));
    if (array == NULL)
    {
        printf("\033[41m\033[1mMemory allocation failed (GPU-ARRAY)\033[0m\n");
        exit(1);
    }

    FILE *file = NULL;
    file = fopen("shop-data/stocks.txt", "r");
    if (file == NULL)
    {
        printf("\033[0mCould not open stocks file : %s\033[0m\n", strerror(errno));
        exit(1);
    }

    for (int i = 0; i < nbrGPU; i++)
    {
        valid &= (fscanf(file, "%s %d %d %d %d", array[i].name, &array[i].number, &array[i].quantity, &array[i].price, &array[i].size) == 5);
    }
    if (valid == 0)
    {
        printf("\033[41m\033[1mError while reading values from stocks.txt.\033[0m\n");
        exit(1);
    }

    fclose(file);
    return array;
}

void print_lowstock(GPU *GPUarray)
{
    if (GPUarray == NULL)
    {
        printf("\033[41m\033[1mParameters error in print_lowstock\033[0m\n");
        exit(1);
    }

    // Create GPUarraycopy to not overwrite file with sorted array
    int size = number_of_GPU();
    int sorted = 0;
    int SizeToSort = size - 1;
    GPU temp;
    int NumberPrinted = 0;
    int taken_space = 0;

    GPU *GPUarraycopy = malloc(sizeof(GPU) * size);
    if (GPUarraycopy == NULL)
    {
        printf("\033[41m\033[1mMemory allocation while trying to copy GPUarray failed.\033[0m\n");
        exit(1);
    }
    memcpy(GPUarraycopy, GPUarray, sizeof(GPU) * size); // memcpy(destination, source, number of bytes to copy);

    // Sort GPUs by their quantity in array

    while (SizeToSort > 0 && sorted == 0)
    {
        sorted = 1;
        for (int i = 0; i < SizeToSort; i++)
        {
            if (GPUarraycopy[i].quantity > GPUarraycopy[i + 1].quantity)
            {
                temp = GPUarraycopy[i]; // Swap values
                GPUarraycopy[i] = GPUarraycopy[i + 1];
                GPUarraycopy[i + 1] = temp;
                sorted = 0;
            }
        }
        SizeToSort -= 1;
    }

    // Print all GPUs which are not in stock and the 5 with the less quantity in stock
    for (int i = 0; i < size; i++)
    {
        if (NumberPrinted == 5)
        {
            break;
        }
        else if (GPUarraycopy[i].quantity == 0)
        {
            printf("The \033[0;36m%s\033[0m needs to be refilled : \033[0;31m0\033[0m in stock !\n", GPUarraycopy[i].name);
        }
        else
        {
            printf("The \033[0;36m%s\033[0m needs to be refilled :  only \033[0;33m%d\033[0m in stock !\n", GPUarraycopy[i].name, GPUarraycopy[i].quantity);
            NumberPrinted += 1;
        }
    }

    // Print the remaining space in stock
    for (int i = 0; i < size; i++)
    {
        taken_space += GPUarray[i].size * GPUarray[i].quantity;
    }
    if (taken_space <= 400)
    {
        printf("Space : \033[0;32m%d/400\033[0m is taken by GPUs. \033[0;32m%d\033[0m space remaining.\n", taken_space, 400 - taken_space);
    }
    else if (taken_space > 400)
    {
        printf("Something is wrong : the space taken in stock is more than the shop can handle. It seems to be a sabotage attempt by Mr.Grignon !\n");
    }

    free(GPUarraycopy);
}

void search_refill_stock(GPU *GPUarray)
{
    if (GPUarray == NULL)
    {
        printf("\033[41m\033[1mParameters error in search_refill_stock\033[0m\n");
        exit(1);
    }

    int size = number_of_GPU();
    char input[16];
    int found = 0;
    char int_to_string[16];
    int answered = 0;
    char yes_or_no[16];
    int taken_space = 0;
    int GPUtoadd = 0;

    do
    {
        ask_string("Enter the name or the number of the GPU whose stocks you want to know/modify : ", sizeof(input), input);

        for (int i = 0; i < size; i++) // search for matched GPU name or number
        {
            snprintf(int_to_string, sizeof(int_to_string), "%d", GPUarray[i].number); // convert GPU's number to string

            if (!strcmp(int_to_string, input) || !strcmp(GPUarray[i].name, input)) // search for GPU with wanted name or wanted number
            {
                printf("\033[0;36m%s\033[0m FOUND \xF8o\xF8, \033[1m%d\033[0m in stock\n", GPUarray[i].name, GPUarray[i].quantity);
                found = 1;

                do
                {
                    ask_string("Do you want to modify the value in stock ? (\033[32my\033[0m/\033[31mn\033[0m) : ", sizeof(yes_or_no), yes_or_no);
                    if (!strcmp(yes_or_no, "y"))
                    {
                        for (int j = 0; j < size; j++) // Calculates the space taken by all GPUs in stock
                        {
                            taken_space += GPUarray[j].size * GPUarray[j].quantity;
                        }
                        do
                        {
                            ask_int("How much GPU do you want to add to the stock ? : ", &GPUtoadd);
                            if (GPUtoadd < 1 || GPUtoadd > 100)
                            {
                                printf("\033[41m\033[1mExiting program, sabotage attempt detected. The next time it happens, I crash your computer using infinite loop memory allocation.\n\033[0m");
                                exit(1);
                            }
                            else
                            {
                                if ((GPUtoadd * GPUarray[i].size) + taken_space > 400)
                                {
                                    printf("The number of GPUs you're trying to restock is \033[0;31m too high \033[0m !\n");
                                }
                                else
                                {
                                    GPUarray[i].quantity += GPUtoadd;
                                    overwrite_stock(GPUarray); // Rewrites the files with the new stock value above
                                    printf("The new stocks values are updated in the file !\n");
                                    answered = 1;
                                }
                            }
                        } while (answered == 0);
                    }
                    else if (!strcmp(yes_or_no, "n"))
                    {
                        printf("The GPU stock will not be modified.\n");
                        answered = 1;
                    }
                    else
                    {
                        printf("\033[1mYou need to enter '\033[32my\033[0m' or '\033[31mn\033[0m'.\n"); // Backslashes are before " to include them in the string : \"
                    }
                } while (answered == 0);
            }
        }
        if (!found)
        {
            printf("\033[1mNo GPU with the name or the number you gave have been found.\n\033[0m");
        }
    } while (found == 0);
}
