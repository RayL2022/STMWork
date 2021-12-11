#ifndef ADC_H_
#define	ADC_H_

/* Includes */
#include "stm32f769xx.h"
#include "stm32f7xx_hal.h"
#include "uart.h"
#include "ConfUart.h" //Need Transmit My State

/* Defines */
//Define our possible states of the paddle (Down, Up, or Neutral)
#define DOWN '1'
#define UP '2'
#define NEUTRAL '3'

/* Global Variables */
ADC_HandleTypeDef hadc1; //ADC handle type
ADC_ChannelConfTypeDef sConfig; //ADC channel type
DMA_HandleTypeDef hdma1; //DMA HandleType
uint32_t adc_value[1]; //ADC input value stored
char my_current_state; //Current state read

/* Function Prototypes */
void configureADC(void); //ADC configurations needed
void configureDMA(void); //DMA Configurations Needed

#endif
