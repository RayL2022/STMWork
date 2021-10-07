//------------------------------------
// Lab 3 - Task 2
//------------------------------------
//
#include "init.h"

UART_HandleTypeDef UART6; //Handle type structure for USART6
UART_HandleTypeDef UART1; //Handle type structure for USART1

// Main Execution Loop
int main(void) {
	//Initialize the system
	Sys_Init();
	Hal_Init();
	initUart(&UART6, 38400, USART6); //Initialize UART with 38400 Baud and Instance USART6
	HAL_UART_MspInit(&UART6); //Initialize GPIOs for USART6

	initUart(&USB_UART, 115200, USART1); //Initialize UART with 38400 Baud and Instance USART1
	HAL_UART_MspInit(&UART6); //Initialize GPIOs for USART1

	HAL_NVIC_EnableIRQ(USART1_IRQn);

	printf("Test!\r\n");

	while(1);
	// Lost? Check out uart.c!

}


void USART1_IRQHandler() {
	HAL_UART_IRQHandler(&USB_UART);
	printf("Test1\n\r");
}

void USART6_IRQHandler() {
	printf("Test2\n\r");
	HAL_UART_IRQHandler(&UART6);
}

HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == USART1){
		printf("TestUSART1\n\r");
		HAL_UART_Transmit_IT();
	}

	if (huart->Instance == USART6){
		printf("TestUSART6\n\r");
		HAL_UART_Receive_IT();
	}
}

