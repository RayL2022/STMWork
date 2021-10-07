//------------------------------------
// Lab 3 - Task 1
//------------------------------------
//
#include "init.h"

UART_HandleTypeDef U6; //Handle type structure for USART6
char input; //For storing user inputs
//char end_message[] = "Program Has Halted\r\n";

// Main Execution Loop
int main(void) {
	//Initialize the system
	Sys_Init();
	HAL_Init();

	initUart(&USB_UART, 115200, USART1);
	HAL_UART_MspInit(&USB_UART);

	initUart(&U6, 38400, USART6); //Initialize UART with 38400 Baud and Instance USART6
	HAL_UART_MspInit(&U6); //Initialize GPIOs for USART6
	//HAL_UART_Receive(&U6, (uint8_t*) &input, 1, 1000);
	//HAL_UART_Transmit(&U6, (uint8_t*) &input, 1, 1000);
	while (1){
		HAL_Delay(1000);
		HAL_UART_Receive(&USB_UART, (uint8_t*) &input, 1, 1000);
		if (input){
			HAL_UART_Transmit(&U6, (uint8_t*) &input, 1, 1000);
			input = 0;
		}


		HAL_UART_Receive(&U6, (uint8_t*) &input, 1, 1000);
		if (input){
			HAL_UART_Transmit(&USB_UART, (uint8_t*) &input, 1, 1000);
			input = 0;
		}


}
