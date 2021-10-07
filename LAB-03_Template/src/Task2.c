//------------------------------------
// Lab 3 - Task 2
//------------------------------------
//
#include "init.h"

UART_HandleTypeDef U6; //Handle type structure for USART6
char input = 0;
int end();
int halt;
// Main Execution Loop
int main(void) {
	//Initialize the system
	Sys_Init();

	initUart(&UART6, 38400, USART6); //Initialize UART with 38400 Baud and Instance USART6

	HAL_NVIC_EnableIRQ(USART1_IRQn);
	HAL_NVIC_EnableIRQ(USART6_IRQn);

	while(1);

}
int end(){
	if (input == '\033'){ //When Escape key is pressed
		HAL_UART_Transmit(&U6, (uint8_t*) end_message, strlen((const char*)end_message), 10);
		HAL_UART_Transmit(&USB_UART, (uint8_t*) end_message, strlen((const char*)end_message), 10);
		return 1;
	}
	return 0;
}


void USART1_IRQHandler() {
	HAL_UART_IRQHandler(&USB_UART);
}

void USART6_IRQHandler() {
	HAL_UART_IRQHandler(&U6);
}

HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == USART1){
		HAL_UART_Receive_IT(&USB_UART, (uint8_t*) &input, 1, 10); //My User
		if (input){
			halt = end();
			if (halt == 1) {
				break;
			}
			HAL_UART_Transmit_IT(&U6, (uint8_t*) &input, 1, 10);
			HAL_UART_Transmit_IT(&USB_UART, (uint8_t*) &input, 1, 2);
			input = 0;
		}
	}

	if (huart->Instance == USART6){
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

