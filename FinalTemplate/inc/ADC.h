#ifndef ADC_H_
#define	ADC_H_

#include "stm32f769xx.h"
#include "stm32f7xx_hal.h"
#include "uart.h"

ADC_HandleTypeDef hadc1;
ADC_ChannelConfTypeDef sConfig;

uint16_t adc_value; //ADC value
char my_current_state;

//Function Prototypes
void configureADC(void);
void pollADC(void);

#endif
