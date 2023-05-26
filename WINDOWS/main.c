#include "managecustomer.h"

int main()
{
	printf("\033[1m .d8888b. Y88b   d88P      .d8888b.  888    888  .d88888b.  8888888b.  \n");
    printf("d88P  Y88b Y88b d88P      d88P  Y88b 888    888 d88P\" \"Y88b 888   Y88b \n");
    printf("888    888  Y88o88P       Y88b.      888    888 888     888 888    888 \n");
    printf("888          Y888P         \"Y888b.   8888888888 888     888 888   d88P \n");
    printf("888           888    8888     \"Y88b. 888    888 888     888 8888888P\"  \n");
    printf("888    888    888               \"888 888    888 888     888 888        \n");
    printf("Y88b  d88P    888         Y88b  d88P 888    888 Y88b. .d88P 888        \n");
    printf(" \"Y8888P\"     888          \"Y8888P\"  888    888  \"Y88888P\"  888        \033[0m\n");
	printf("                                                        \033[1m\033[34m\033[3mLRD Industries\033[0m\n");
	// Create the GPU array
	GPU *array = create_GPU_array();
	if (array == NULL)
	{
		printf("\033[41m\033[1mGPU *array error.\033[0m\n");
	}

	// Choose mode
	char gestion_or_buy[16];
	int answered = 0;
	int client_number = 0;
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
			client_number = is_client();
			print_last_buy(client_number);
			search_and_buy(array, client_number);
			answered = 1;
		}
		else
		{
			printf("\033[1mPlease enter 'gestion' or 'buy'\033[0m\n");
		}
	} while (answered == 0);

	free(array);
	return 0;
}
