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
	int column = 1;
	printf("\033c\033[2J"); fflush(stdout);

	while (column < 81){
		printf("\033[%u;%uH-", 1, column); fflush(stdout);
		printf("\033[%u;%uH-", 25, column); fflush(stdout);
		column += 1;
	}

	play_ball.x_row = 2;
	play_ball.y_column = 2;
	play_ball.last_x = 2;
	play_ball.last_y = 2;


	p1_paddle.first_x = 10;
	p2_paddle.first_x = 10;
	p1_paddle.first_y = 1;
	p2_paddle.first_y = 80;

	p1_paddle.second_x = 11;
	p2_paddle.second_x = 11;
	p1_paddle.second_y = 1;
	p2_paddle.second_y = 80;

	p1_paddle.third_x = 12;
	p2_paddle.third_x = 12;
	p1_paddle.third_y = 1;
	p2_paddle.third_y = 80;

	p1_paddle.fourth_x = 13;
	p2_paddle.fourth_x = 13;
	p1_paddle.fourth_y = 1;
	p2_paddle.fourth_y = 80;

	while(1)
	{
		HAL_Delay(500);
		//pollADC();
		my_current_state = UP;
		p1_paddle = update_paddle(p1_paddle, my_current_state);
		p2_paddle = update_paddle(p2_paddle, opponent_current_state);
		draw_paddle(p1_paddle);
		draw_paddle(p2_paddle);
		draw_ball(play_ball);
		/*
		play_ball.x_row += 1;
		if (play_ball.x_row > 25){
			play_ball.x_row = 25;
		}

		play_ball.last_y = play_ball.y_column;
		play_ball.y_column += 1;
		if (play_ball.y_column > 80){
			play_ball.y_column = 0;
		}
		*/

	}
}


//ADD other player's input variable
