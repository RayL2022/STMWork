#ifndef ADC_H_
#define	ADC_H_

#include "stm32f769xx.h"
#include "stm32f7xx_hal.h"
#include "uart.h"

#define DOWN '1'
#define UP '2'
#define NEUTRAL '3'

ADC_HandleTypeDef hadc1;
ADC_ChannelConfTypeDef sConfig;

DMA_HandleTypeDef hdma1; //DMA HandleType

uint32_t adc_value[1]; //ADC value
char my_current_state;
volatile uint8_t D5_button; //Button connected to D5
volatile int user_input;

//Function Prototypes
void configureADC(void);
void pollADC(void);
void configureDMA(void); //DMA Configurations Needed
void Init_GPIO(void);

#endif
