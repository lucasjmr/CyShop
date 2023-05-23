#include "input.h"

typedef struct
{
    char name[16];
    int number;
    int quantity;
    int price;
    int size;
} GPU;

GPU *create_GPU_array();
int number_of_GPU(void);
void print_lowstock(GPU *GPUarray);
void search_refill_stock(GPU *GPUarray);
void overwrite_stock(GPU *GPUarray);