//------------------------------------
// Lab 3 - Task 1
//------------------------------------
//
#include "init.h"

UART_HandleTypeDef U6; //Handle type structure for USART6'
//For storing user inputs
char end_message[] = "Program Has Halted\r\n";
char input;
int end();
int halt;
// Main Execution Loop
int main(void) {
	//Initialize the system
	Sys_Init();

	//initUart(&USB_UART, 115200, USART1);
	//HAL_UART_MspInit(&USB_UART);

	fflush(stdout);

	initUart(&U6, 38400, USART6); //Initialize UART with 38400 Baud and Instance USART6

	//HAL_UART_MspInit(&U6); //Initialize GPIOs for USART6
	input = 0;
	while (1){
		HAL_UART_Receive(&USB_UART, (uint8_t*) &input, 1, 10); //My User
		if (input){
			halt = end();
			if (halt == 1) {
				break;
			}
			HAL_UART_Transmit(&U6, (uint8_t*) &input, 1, 10);
			HAL_UART_Transmit(&USB_UART, (uint8_t*) &input, 1, 2);
			input = 0;
		}

		HAL_UART_Receive(&U6, (uint8_t*) &input, 1, 10); //Other User
		if (input){
			halt = end();
			if (halt == 1){
				break;
			}
			HAL_UART_Transmit(&USB_UART, (uint8_t*) &input, 1, 10);
			input = 0;
		}
	}
}

int end(){
	if (input == '\033'){ //When Escape key is pressed
		HAL_UART_Transmit(&U6, (uint8_t*) end_message, strlen((const char*)end_message), 1000);
		HAL_UART_Transmit(&USB_UART, (uint8_t*) end_message, strlen((const char*)end_message), 1000);
		return 1;
	}
	return 0;
}
