//------------------------------------
// Lab 3 - Task 2
//------------------------------------
//The purpose of this lab is to use the interrupt method for UART communication
#include "init.h"

UART_HandleTypeDef U6; //Handle type structure for USART6

char input; //For storing user inputs
char end_message[] = "\r\nProgram Has Halted"; //Termination Message
char end = 0; //Looping Variable

// Main Execution Loop
int main(void) {
	//Initialize the system
	Sys_Init();
	HAL_Init();

	initUart(&U6, 38400, USART6); //Initialize UART with 38400 Baud and Instance USART6

	HAL_NVIC_EnableIRQ(USART1_IRQn); //Enable interrupt
	HAL_NVIC_EnableIRQ(USART6_IRQn); //Enable interrupt

	HAL_UART_Receive_IT (&USB_UART, (uint8_t*) &input, 1); //Trigger receiving input for USB
	HAL_UART_Receive_IT (&U6, (uint8_t*) &input, 1); //Trigger receiving input for U6

	while(end == 0); //Loop while looping variable is 0
}

//Handler for USART1
void USART1_IRQHandler() {
	HAL_UART_IRQHandler(&USB_UART);
}

//Handler for USART6
void USART6_IRQHandler() {
	HAL_UART_IRQHandler(&U6);
}

//Callback function
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == USART1){ //Associated with USART1
		HAL_UART_Receive_IT(&USB_UART, (uint8_t*) &input, 1); //My User tries to receive input
		if (input){ //We received input
			if (input == '\033'){ //Input was escape
				end = 1; //End loop
				//Transmit termination message to this device
				HAL_UART_Transmit(&USB_UART, (uint8_t*) end_message, strlen((const char*)end_message), 10);
			}
			HAL_UART_Transmit(&U6, (uint8_t*) &input, 1, 10); //Transmit input to other device
			HAL_UART_Transmit(&USB_UART, (uint8_t*) &input, 1, 10); //Transmit input to this device
			input = 0; //Clear input
		}
	}

	if (huart->Instance == USART6){ //Associated with USART6
		HAL_UART_Receive_IT(&U6, (uint8_t*) &input, 1); //Other User tries to receive input
		if (input){ //Received input
			if (input == '\033'){ //Input was escape
				end = 1; //End loop
				//Transmit termination message to this device
				HAL_UART_Transmit(&USB_UART, (uint8_t*) end_message, strlen((const char*)end_message), 10);
			}
			HAL_UART_Transmit(&USB_UART, (uint8_t*) &input, 1, 10);  //Transmit input to device
			input = 0; //Clear input
			}
	}
}
