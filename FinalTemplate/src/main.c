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

	while (column < MAX_COL + 1){
		printf("\033[%u;%uH-", MIN_ROW, column); fflush(stdout);
		printf("\033[%u;%uH-", MAX_COL, column); fflush(stdout);
		column += 1;
	}

	play_ball.x_column = 30;
	play_ball.y_row = 16;
	play_ball.last_x = 30;
	play_ball.last_y = 16;

	play_ball.x_speed = -1;
	play_ball.y_speed = -1;

	p1_paddle.first_x = MIN_COL;
	p2_paddle.first_x = MAX_COL;
	p1_paddle.first_y = ((MAX_ROW - 2 - 4) / 2) + 2;
	p2_paddle.first_y = ((MAX_ROW - 2 - 4) / 2) + 2;

	p1_paddle.second_x = MIN_COL;
	p2_paddle.second_x = MAX_COL;
	p1_paddle.second_y = ((MAX_ROW - 2 - 4) / 2) + 3;
	p2_paddle.second_y = ((MAX_ROW - 2 - 4) / 2) + 3;

	p1_paddle.third_x = MIN_COL;
	p2_paddle.third_x = MAX_COL;
	p1_paddle.third_y = ((MAX_ROW - 2 - 4) / 2) + 4;
	p2_paddle.third_y = ((MAX_ROW - 2 - 4) / 2) + 4;

	p1_paddle.fourth_x = MIN_COL;
	p2_paddle.fourth_x = MAX_COL;
	p1_paddle.fourth_y = ((MAX_ROW - 2 - 4) / 2) + 5;
	p2_paddle.fourth_y = ((MAX_ROW - 2 - 4) / 2) + 5;

	while(1)
	{
		pollADC();
		HAL_Delay(150);
		//my_current_state = UP;
		//opponent_current_state = NEUTRAL;
		p1_paddle = update_paddle(p1_paddle, my_current_state);
		p2_paddle = update_paddle(p2_paddle, opponent_current_state);
		draw_paddle(p1_paddle);
		draw_paddle(p2_paddle);
		play_ball = update_ball(play_ball);
		play_ball = check_bounce(play_ball, p1_paddle, p2_paddle);
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
