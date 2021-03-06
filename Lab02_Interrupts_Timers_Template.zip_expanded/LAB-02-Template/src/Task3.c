//----------------------------------
// Task3.c
//----------------------------------
// This code is to configure Timer 7 to count elapsed time
// in tenths of a second using HAL
//

//
// -- Imports ---------------
//
#include "init.h"

//
//
// -- Prototypes ------------
//
void Init_Timer(); //Configure Timer

double time_elapsed; //Variable to keep track of time elapsed (in tenths)
TIM_HandleTypeDef HTIM7; //Structure to store information for the Timer

//
// -- Code Body -------------
//


int main() {
	Sys_Init();
    HAL_Init(); //Initialize HAL
	Init_Timer(); //Initialize Timer 7
	time_elapsed = 0; //Clear Time
	while (1) {
	}
}

//
//
// -- Init Functions ----------
//
void Init_Timer() {

	//Enable Clock for Timer 7
	__HAL_RCC_TIM7_CLK_ENABLE();

	//Associate the timer for the structure as Timer 7
	HTIM7.Instance = TIM7;

	//Enable interrupts on NVIC
	HAL_NVIC_EnableIRQ(TIM7_IRQn);

	//Configure Timer 7
	HTIM7.Init.Prescaler = 24999; //Fixed Chosen Value
	HTIM7.Init.Period = 431; // Produces 10Hz Overflow

	HAL_TIM_Base_Init(&HTIM7); //Call the Base_init function

	HAL_TIM_Base_Start_IT(&HTIM7); //Start timer 7 in Interrupt Mode
}


//HAL - GPIO/EXTI Handler
void TIM7_IRQHandler(TIM_HandleTypeDef *htim) {
	HAL_TIM_IRQHandler(&HTIM7); //Pass a reference to the structure for which Timer
}

//HAL - Callback Function
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == TIM7){ //Test if the Timer Interrupt was trigger by Timer 7
		time_elapsed += 0.1; //A tenth of a second has passed
		printf("\033[2J\033[HTime Elapsed: %.1f seconds\r\n", time_elapsed); //Display Time
		//Formatted text is to clear screen and return to home position
	}
}

//These functions need to be included in name but don't need to be implemented
void HAL_TIMEx_BreakCallback(TIM_HandleTypeDef *htim){};
void HAL_TIMEx_CommutationCallback(TIM_HandleTypeDef *htim){};
