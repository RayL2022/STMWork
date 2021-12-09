/*
 * Timer.c
 *
 *  Created on: Nov 30, 2021
 *      Author: Shayne
 */
#include "Timer.h"
#include "Draw.h"

void Init_Timer() {
	__HAL_RCC_TIM7_CLK_ENABLE(); //Enable the TIM7 peripheral

	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 999;
	htim7.Init.Period = 10799; //0.1sec

	HAL_NVIC_EnableIRQ(TIM7_IRQn);

	HAL_TIM_Base_Init(&htim7); //Configure the timer
	HAL_TIM_Base_Start_IT(&htim7); //Start the timer
}


// -- User Functions -------------
//
void select_time() {
	while (input2 == 0){
		input2 = getchar();
	}
	if (input2 == '5'){
		elapsed = 300;
		start = 1;
	}

	if (input2 == '3'){
		elapsed = 180;
		start = 1;
	}

	if (input2 == '1'){
		elapsed = 60;
		start = 1;
	}
}

void count_down(){
	minute = elapsed/60;
	second = (elapsed%60);
	Flag2 = 1;
}

// -- ISRs (IRQs) -------------
//
void TIM7_IRQHandler() {
	HAL_TIM_IRQHandler(&htim7);
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
// This callback is automatically called by the HAL on the UEV event
	if(htim->Instance == TIM7){
		if ((start == 1) && (D5_button == 0)){
			one++;
			Flag = 1;
			if ((one%10 == 0) && elapsed != 0){
				elapsed--;  //Decrement elapsed
				count_down();
			}
		}
	}
}


void Init_GPIO(void) {
    __HAL_RCC_GPIOC_CLK_ENABLE(); //Enable Clock for Port C (HAL)

    //Configure D5(PC8) as input, with pull-up resistors enabled (HAL)
    GPIO_InitTypeDef D5;
    D5.Pin = GPIO_PIN_8; //On pin 8
	D5.Mode = GPIO_MODE_IT_RISING; //Set Mode to Interrupt on Rising Edge
	D5.Pull = GPIO_PULLUP; //Enable pull-up
	HAL_GPIO_Init(GPIOC, &D5); //Configure this to Port C

	//Set interrupt enable for EXTI8 (included in 9_5)
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

//HAL - GPIO/EXTI Handler
void EXTI9_5_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8); //Pin 8 is the interrupt trigger
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if (D5_button == 0){
		HAL_TIM_Base_Stop_IT(&htim7); //Start the timer
		D5_button = 1; //Button is on
		user_input = 0;
		while (user_input == 0){
			HAL_UART_Receive(&USB_UART, (uint8_t*) &user_input, 1, HAL_MAX_DELAY); //Trigger receiving input for U6
		}
		play_ball.x_speed = 0;
		play_ball.y_speed = 0;

		while ((play_ball.x_speed == 0) || (play_ball.x_speed == 0)){
			play_ball.x_speed = (rand() % (MAX_X_SPEED - MIN_X_SPEED + 1)) + MIN_X_SPEED;
			play_ball.y_speed = (rand() % (MAX_Y_SPEED - MIN_Y_SPEED + 1)) + MIN_Y_SPEED;
		}

		if (play_ball.x_speed > 0){
			play_ball.x_dir = 1;
		}else{
			play_ball.x_dir = -1;
		}

		if (play_ball.y_speed > 0){
			play_ball.y_dir = 1;
		}else{
			play_ball.y_dir = -1;
		}

		D5_button = 0;
		HAL_TIM_Base_Start_IT(&htim7); //Start the timer
	}

}


void HAL_TIMEx_BreakCallback(TIM_HandleTypeDef *htim){}
void HAL_TIMEx_CommutationCallback(TIM_HandleTypeDef *htim){}

