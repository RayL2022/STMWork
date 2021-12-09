#include "init.h"
#include "Draw.h"
#include "ADC.h"
#include "ConfUart.h"
#include "Timer.h"


int main(void){
	Sys_Init();
	HAL_Init();
	Init_Timer();
	Init_GPIO();
	configureADC();
	configureUart();

	HAL_ADC_Start(&hadc1);

	time_screen();
	while(1)
	{
		while (D5_button == 1 || elapsed == 0);
		pollADC();
		HAL_Delay(75);
		p1_paddle = update_paddle(p1_paddle, my_current_state);
		p2_paddle = update_paddle(p2_paddle, opponent_current_state);
		draw_paddle(p1_paddle);
		draw_paddle(p2_paddle);
		if (Flag == 1){
			play_ball = update_ball(play_ball);
			play_ball = check_bounce(play_ball, p1_paddle, p2_paddle);
			Flag=0;
		}
		draw_ball(play_ball);
		if (Flag2 == 1){
			update_score_and_time(p1, p2, minute, second);
		}
	}
}

