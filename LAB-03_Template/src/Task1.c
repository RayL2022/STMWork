//------------------------------------
// Lab 3 - Task 1
//------------------------------------
//
#include "init.h"

UART_HandleTypeDef UART6; //Handle type structure for USART6
char input; //For storing user inputs

// Main Execution Loop
int main(void) {
	//Initialize the system
	Sys_Init();

	initUart(&UART6, 38400, USART6); //Initialize UART with 38400 Baud and Instance USART6
	HAL_UART_MspInit(&UART6); //Initialize GPIOs for USART6
	while (1){
		input = uart_getchar(&UART6, 1); //Get a char and echo the char

		printf("%c\r\n", input); //Print on this terminal
	}

}
