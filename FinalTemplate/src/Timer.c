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
	printf("\033[12;40HEnter: \n\r");
	while (input2 == 0){
		input2 = getchar();
		printf("%d\n\r", input2);
	}
	printf("Input: %c\n\r", input2);
	if (input2 == '5'){
		printf("\033[2J"); fflush(stdout);
		elapsed = 300;
		start = 1;
		counts = 1;
	}

	if (input2 == '3'){
		printf("\033[2J"); fflush(stdout);
		elapsed = 180;
		start = 1;
		counts = 1;
	}

	if (input2 == '1'){
		printf("\033[2J"); fflush(stdout);
		elapsed = 60;
		start = 1;
		counts = 1;
	}
}

void count_down(){
	minute = elapsed/60;
	second = (elapsed%60);
	update_score_and_time(p1, p2, minute, second);
}

void speed(){
	//Needs edits the values are not correct. Thats how many pixel/s/10 but each pixel movement must be seen
	if ((one-saved) > 20){
		saved = one;
		if (play_ball.x_speed < 0){
			if(play_ball.x_speed > MIN_X_SPEED){
				play_ball.x_speed --;
			}
			int speedUP = -1000/play_ball.x.speed;
			HAL_Delay(speedUP);
			play_ball = update_ball(play_ball);
		}
		else if (play_ball.x_speed > 0){
			if(play_ball.x_speed < MAX_X_SPEED){
				play_ball.x_speed ++;
			}
			HAL_Delay(1000/play_ball.x_speed);
			play_ball = update_ball(play_ball);
		}
	}

}
// -- ISRs (IRQs) -------------
//
void TIM7_IRQHandler() {
	HAL_TIM_IRQHandler(&htim7);
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
// This callback is automatically called by the HAL on the UEV event
	if(htim->Instance == TIM7){
		if ((start == 1)){
			if (one == 0){
				score1 = p1.score;
				score2 = p2.score;
				printf("\033[13;38H\033[;40m "); fflush(stdout);
				printf("%ld:00", elapsed/60); fflush(stdout);
			}
			one++;
			if (p1.score != score1){
				saved = one;
				score1=p1.score;
			}
			if (p2.score != score2){
				saved = one;
				score2=p2.score;
			}
			speed();
			if ((one%10 == 0)){
				elapsed--;  //Decrement elapsed
				count_down();
			}
		}
	}
}

void HAL_TIMEx_BreakCallback(TIM_HandleTypeDef *htim){}
void HAL_TIMEx_CommutationCallback(TIM_HandleTypeDef *htim){}

