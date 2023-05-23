#include "managecustomer.h"

int main()
{
	//Create the GPU array
	GPU* array = create_GPU_array();

	// Choose mode
	char gestion_or_buy[16];
	int answered = 0;
	do
	{
		ask_string("Would you like to enter in gestion or buy mode ? : ", sizeof(gestion_or_buy), gestion_or_buy);
		if (!strcmp("gestion", gestion_or_buy))
		{
			print_lowstock(array);
			search_refill_stock(array);
			answered = 1;
		}
		else if (!strcmp("buy", gestion_or_buy))
		{
			int client_number;
			client_number = is_client();
			print_last_buy(client_number);
			search_and_buy(array, client_number);
			answered = 1;
		}
		else
		{
			printf("Please enter 'gestion' or 'buy'\n");
		}
	} while (answered == 0);

	free(array);
	return 0;
}