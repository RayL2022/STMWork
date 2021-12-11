#include "init.h"
#include "Draw.h"
#include "ADC.h"
#include "ConfUart.h"
#include "Timer.h"


int main(void){
	//System and HAL Initializations
	Sys_Init();
	HAL_Init();
	Init_Timer(); //Initial Timer
	Init_GPIO(); //Intial GPIO for Pause Button
	configureADC(); //Configure the ADC for inputs
	configureUart(); //Configure UART for transmission

	time_screen(); //Print the choose time screen
	HAL_ADC_Start_DMA(&hadc1, adc_value, 1); //Start getting ADC Readings
	while(1)
	{
		//Halt if the pause button was triggered or time ran out
		while (elapsed == 0);
		HAL_Delay(75); //Delay to ensure enough time for values to be updated
		p1_paddle = update_paddle(p1_paddle, my_current_state); //try to update my paddle
		p2_paddle = update_paddle(p2_paddle, opponent_current_state); //try to update opponent paddle
		//draw both paddles
		draw_paddle(p1_paddle);
		draw_paddle(p2_paddle);
		if (Flag == 1){//Timer sets this when it is time to check ball position
			play_ball = update_ball(play_ball); //update ball location
			play_ball = check_bounce(play_ball, p1_paddle, p2_paddle); //check for bounce
			Flag = 0; //reset flag
		}
		draw_ball(play_ball); //draw the current ball position
		if (Flag2 == 1){ //Time has to be updated
			update_score_and_time(p1, p2, minute, second);
		}
	}
}

