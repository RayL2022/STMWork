#ifndef TIMER_H_
#define	TIMER_H_

#include "stm32f769xx.h"
#include "stm32f7xx_hal.h"
#include "uart.h"
#include "init.h"

TIM_HandleTypeDef htim7;
volatile uint32_t elapsed;
volatile uint32_t one;
volatile uint32_t saved;

char minute;
char second;
char input2;
char start;
char counts;
char score;
char speedx;
char speedy;
uint8_t Flag;
uint8_t Flag2;

//Function Prototypes
void Init_Timer();
void select_time();
void speed();

#endif
