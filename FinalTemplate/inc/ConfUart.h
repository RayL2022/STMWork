#ifndef CONFUART_H_
#define	CONFUART_H_

#include "stm32f769xx.h"
#include "stm32f7xx_hal.h"
#include "uart.h"
#include "ADC.h"

UART_HandleTypeDef U6; //Handle type structure for USART6

char input;
char opponent_current_state;

//Function Prototypes
void configureUart(void);

#endif
