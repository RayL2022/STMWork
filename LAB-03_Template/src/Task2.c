//------------------------------------
// Lab 3 - Task 2
//------------------------------------
//
#include "init.h"

UART_HandleTypeDef U6; //Handle type structure for USART6
char input[4];
int end = 0;
// Main Execution Loop
int main(void) {
	//Initialize the system
	Sys_Init();
	HAL_Init();

	initUart(&U6, 38400, USART6); //Initialize UART with 38400 Baud and Instance USART6

	HAL_NVIC_EnableIRQ(USART1_IRQn);
	HAL_NVIC_EnableIRQ(USART6_IRQn);

	HAL_UART_Receive_IT (&USB_UART, (uint8_t*) input, 1);
	HAL_UART_Receive_IT (&U6, (uint8_t*) input, 1);

	while(1){
		if (end == 1){
			printf("Carried out");
			break;
		}
	}

	printf ("Program Has Halted"); fflush(stdout);

}


void USART1_IRQHandler() {
	//printf("IRQ1\r\n");
	HAL_UART_IRQHandler(&USB_UART);
}

void USART6_IRQHandler() {
	//printf("IRQ6\r\n");
	HAL_UART_IRQHandler(&U6);
	//printf("out\n\r");
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	printf("callback");
	if (huart->Instance == USART1){
		HAL_UART_Receive_IT(&USB_UART, (uint8_t*) &input, 1); //My User
		if (input[0] == '\033'){
			end = 1;
		}
		else if (input[0]){
			HAL_UART_Transmit(&U6, (uint8_t*) input, 1, 10);
			HAL_UART_Transmit(&USB_UART, (uint8_t*) &input, 1, 10);
			input[0] = 0;
		}
	}

	if (huart->Instance == USART6){
		HAL_UART_Receive_IT(&U6, (uint8_t*) &input, 1); //Other User
		if (input[0] == '\033'){
			end = 1;
		}
		else if (input[0]){
			HAL_UART_Transmit(&USB_UART, (uint8_t*) &input, 1, 10);
			input[0] = 0;
		}
	}
}
