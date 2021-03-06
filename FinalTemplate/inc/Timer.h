#ifndef TIMER_H_
#define	TIMER_H_

//Files needed for Timer.c
#include "stm32f769xx.h"
#include "stm32f7xx_hal.h"
#include "uart.h"
#include "Draw.h" //Ball Struct
//Used for Random function
#include <stdio.h>
#include <stdlib.h>

//Globals
TIM_HandleTypeDef htim7;
volatile uint32_t elapsed;
volatile uint32_t one;

char minute;
char second;
volatile char input2;
volatile char start;
uint8_t Flag;
uint8_t Flag2;
volatile int user_input; //Interrupt variable to check for user input pressed

//Function Prototypes
void Init_Timer(void);
void select_time(void);
void count_down(void);
void Init_GPIO(void);

#endif
