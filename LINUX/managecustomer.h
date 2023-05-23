#include "managestock.h"

int client_number_to_assign();
int is_client();
void create_client(char *name, int age, int client_number);
void delete_client(int client_number);
int number_of_buys(int client_number);
void print_last_buy(int client_number);
void search_and_buy(GPU *GPUarray, int client_number);
void client_gpu_append(char *string, int client_number);