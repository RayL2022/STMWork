#include "init.h"
#include "Draw.h"
#include "ADC.h"
#include "ConfUart.h"


int main(void){
	Sys_Init();
	HAL_Init();

	configureADC();
	configureUart();

	HAL_ADC_Start(&hadc1);

	my_current_state = 0; //1 for Down, 2 for Up, 3 for Neutral
	input = 0;
	play_ball.x_row = 0;
	play_ball.y_column = 0;
	play_ball.last_x = 0;
	play_ball.last_y = 0;
	while(1)
	{
		HAL_Delay(50);
		draw_ball(play_ball);
		/*
		play_ball.x_row += 1;
		if (play_ball.x_row > 25){
			play_ball.x_row = 25;
		}
		*/
		play_ball.last_y = play_ball.y_column;
		play_ball.y_column += 1;
		if (play_ball.y_column > 80){
			play_ball.y_column = 0;
		}

		//pollADC();

	}
}


//ADD other player's input variable
