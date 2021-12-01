/*
 * Timer.c
 *
 *  Created on: Nov 30, 2021
 *      Author: Shayne
 */
#include "init.h"
#include "stm32f7xx_hal_conf.h"
void Init_Timer();

TIM_HandleTypeDef htim7;
volatile uint32_t elapsed = 0;

int main() {
	Sys_Init();
	HAL_Init();
	Init_Timer();

	fflush(stdout);
	while (1){
		printf("Elapsed: %ld\n\r", elapsed);
	}

}

void Init_Timer() {
	__HAL_RCC_TIM7_CLK_ENABLE(); //Enable the TIM7 peripheral

	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 499;
	htim7.Init.Period = 21599; //.1sec

	HAL_NVIC_EnableIRQ(TIM7_IRQn);

	HAL_TIM_Base_Init(&htim7); //Configure the timer
	HAL_TIM_Base_Start_IT(&htim7); //Start the timer
}


// -- ISRs (IRQs) -------------
//
void TIM7_IRQHandler() {
	elapsed++;  //increment the variable for time elapsed
	HAL_TIM_IRQHandler(&htim7);
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
// This callback is automatically called by the HAL on the UEV event
	if(htim->Instance == TIM7){
		elapsed++;  //increment the variable for time elapsed
	}

	}


void HAL_TIMEx_BreakCallback(TIM_HandleTypeDef *htim){}
void HAL_TIMEx_CommutationCallback(TIM_HandleTypeDef *htim){}
