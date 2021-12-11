/*
 * ConfUart.c
 *
 *  Created on: Nov 30, 2021
 *      Author: Ray & Shayne
 */
#include "ConfUart.h"

void configureUart(void){
	initUart(&U6, 38400, USART6); //Initialize UART with 38400 Baud and Instance USART6
	HAL_NVIC_EnableIRQ(USART6_IRQn); //Enable interrupt

	HAL_UART_Receive_IT (&U6, (uint8_t*) &opponent_current_state, 1); //Trigger receiving input for U6
}

//Handler for USART6
void USART6_IRQHandler(void) {
	HAL_UART_IRQHandler(&U6);
}

//Callback function
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == USART6){ //Associated with USART6
		HAL_UART_Receive_IT (&U6, (uint8_t*) &opponent_current_state, 1); //Trigger receiving input for U6
	}
}
