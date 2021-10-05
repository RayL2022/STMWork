//------------------------------------
// Lab 3 - Task 1
//------------------------------------
//
#include "init.h"

UART_HandleTypeDef UART6; //Handle type structure for USART6
uint8_t input; //For storing user inputs
char end_message[] = "Program Has Halted\r\n";
// Main Execution Loop
int main(void) {
	//Initialize the system
	Sys_Init();

	initUart(&UART6, 38400, USART6); //Initialize UART with 38400 Baud and Instance USART6
	HAL_UART_MspInit(&UART6); //Initialize GPIOs for USART6
	while (1){
		HAL_UART_Receive(&UART6, &input, 1, 1000);
		HAL_UART_Transmit(&UART6, &input, 1, 1000);

		/*
		if (input == '\033'){ //When Escape key is pressed
			HAL_UART_Transmit(&UART6, (uint8_t*) end_message, strlen((const char*)end_message), 1000);
			printf("%s",end_message); //Print on controller end
			return 1;
		}
		*/
		//else{
			printf("%c\r\n", input); //Print on controller end
		//}


	}

}
