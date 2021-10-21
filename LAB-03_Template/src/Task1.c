//------------------------------------
// Lab 3 - Task 1
//------------------------------------
//The purpose of this lab to to use the polling method with UART communication

#include "init.h"

UART_HandleTypeDef U6; //Handle type structure for USART6

char end_message[] = "\r\nProgram Has Halted"; //Termination Message
char input; //For storing user inputs
char end = 0; //Looping Variable

// Main Execution Loop
int main(void) {
	//Initialize the system
	Sys_Init();

	fflush(stdout);

	initUart(&U6, 38400, USART6); //Initialize UART with 38400 Baud and Instance USART6

	//HAL_UART_MspInit(&U6); //Initialize GPIOs for USART6
	input = 0;
	while (end == 0){ //Continue while looping variable is equal to 0

		HAL_UART_Receive(&USB_UART, (uint8_t*) &input, 1, 10); //Receive input from my user
		if (input){ //We got an input
			if (input == '\033'){ //Input was escape
				end = 1; //End loop
				//print end message on my terminal
				HAL_UART_Transmit(&USB_UART, (uint8_t*) end_message, strlen((const char*)end_message), 10);
			}
			HAL_UART_Transmit(&U6, (uint8_t*) &input, 1, 10); //transmit input to other device
			HAL_UART_Transmit(&USB_UART, (uint8_t*) &input, 1, 2); //transmit input to this device
			input = 0; //clear input
		}

		HAL_UART_Receive(&U6, (uint8_t*) &input, 1, 10); //Other User receives input
		if (input){ //We got an input
			if (input == '\033'){ //Input was escape
				end = 1; //End loop
				//print end message on the terminal
				HAL_UART_Transmit(&USB_UART, (uint8_t*) end_message, strlen((const char*)end_message), 10);
			}
			HAL_UART_Transmit(&USB_UART, (uint8_t*) &input, 1, 10); //transmit input on device
			input = 0; //clear input
		}
	}
}

