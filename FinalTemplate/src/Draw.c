#include "Draw.h"
#include "ADC.h"

void draw_paddle(paddle my_paddle){
	printf("\033[%u;%uH\033[;47m ", my_paddle.first_y, my_paddle.first_x);
	printf("\033[%u;%uH\033[;47m ", my_paddle.second_y, my_paddle.second_x);
	printf("\033[%u;%uH\033[;47m ", my_paddle.third_y, my_paddle.third_x);
	printf("\033[%u;%uH\033[;47m ", my_paddle.fourth_y, my_paddle.fourth_x);
	fflush(stdout);
};

paddle update_paddle(paddle my_paddle, char state){
	if (state == UP){
		if ((my_paddle.first_y - 1) > MIN_ROW){
			printf("\033[%u;%uH\033[;40m ", my_paddle.fourth_y, my_paddle.fourth_x);
			fflush(stdout);
			my_paddle.first_y -= 1;
			my_paddle.second_y -= 1;
			my_paddle.third_y -= 1;
			my_paddle.fourth_y -= 1;
		}
	}
	else if (state == DOWN){
		if ((my_paddle.fourth_y + 1) < MAX_ROW){
			printf("\033[%u;%uH\033[;40m ", my_paddle.first_y, my_paddle.first_x);
			fflush(stdout);
			my_paddle.first_y += 1;
			my_paddle.second_y += 1;
			my_paddle.third_y += 1;
			my_paddle.fourth_y += 1;
		}
	}
	return my_paddle;
};

void draw_ball(ball test_ball){
	printf("\033[%u;%uH\033[;40m ", test_ball.last_y, test_ball.last_x); //Erase Last Ball Position
	printf("\033[%u;%uH\033[;43m ", test_ball.y_row, test_ball.x_column);
	fflush(stdout);
};

ball update_ball(ball test_ball){
	//printf("\033c\033[20;20H%u\r\n", (test_ball.orientation == POS_NEG));
	test_ball.last_y = test_ball.y_row;
	test_ball.last_x = test_ball.x_column;

	if ((test_ball.y_speed > 0) && (test_ball.x_speed > 0)){ //POS, POS
		test_ball.y_row += 1;
		test_ball.x_column += 1;
	}
	else if ((test_ball.y_speed > 0) && (test_ball.x_speed < 0)){ //POS, NEG
		test_ball.y_row -= 1;
		test_ball.x_column += 1;
	}
	else if ((test_ball.y_speed < 0) && (test_ball.x_speed > 0)){ //NEG, POS
		test_ball.y_row += 1;
		test_ball.x_column -= 1;
	}
	else if ((test_ball.y_speed < 0) && (test_ball.x_speed < 0)){ //NEG, NEG
		test_ball.y_row -= 1;
		test_ball.x_column -= 1;
	}

	return test_ball;
};

ball check_bounce(ball test_ball, paddle my_paddle, paddle opp_paddle){
	if (test_ball.y_row <= MIN_ROW){
		//Bounce, Top
		test_ball.y_row = MIN_ROW + 2;
		test_ball.x_speed *= -1;
	}
	else if (test_ball.y_row >= MAX_ROW){
		//Bounce, Bottom
		test_ball.y_row = MAX_ROW - 2;
		test_ball.x_speed *= -1;
	}

	else if (test_ball.x_column - 1 == MIN_COL){
		//Check if Paddle here, else p2 scores (left)
		if (test_ball.y_row == my_paddle.first_y){\
			test_ball.x_column = MIN_COL + 1;
			test_ball.y_speed *= -1;
		}
		else if (test_ball.y_row == my_paddle.second_y){
			test_ball.x_column = MIN_COL + 1;
			test_ball.y_speed *= -1;
		}
		else if (test_ball.y_row == my_paddle.third_y){
			test_ball.x_column = MIN_COL + 1;
			test_ball.y_speed *= -1;
		}
		else if (test_ball.y_row == my_paddle.fourth_y){
			test_ball.x_column = MIN_COL + 1;
			test_ball.y_speed *= -1;
		}
		else{
			test_ball.x_column -= 1;
			draw_ball(test_ball);
			HAL_Delay(20);
			printf("\033[%u;%uH\033[;40m ", test_ball.y_row, test_ball.x_column); //Erase Last Ball Position
			fflush(stdout);
			game_reset();
			return play_ball;
		}
	}

	else if (test_ball.x_column + 1 == MAX_COL){
		//Check if Paddle here, else p1 scores (right)
		if (test_ball.y_row == opp_paddle.first_y){
			test_ball.x_column = MAX_COL - 1;
			test_ball.y_speed *= -1;
		}
		else if (test_ball.y_row == opp_paddle.second_y){
			test_ball.x_column = MAX_COL - 1;
			test_ball.y_speed *= -1;
		}
		else if (test_ball.y_row == opp_paddle.third_y){
			test_ball.x_column = MAX_COL - 1;
			test_ball.y_speed *= -1;
		}
		else if (test_ball.y_row == opp_paddle.fourth_y){
			test_ball.x_column = MAX_COL - 1;
			test_ball.y_speed *= -1;
		}
		else{
			test_ball.x_column += 1;
			draw_ball(test_ball);
			HAL_Delay(20);
			printf("\033[%u;%uH\033[;40m ", test_ball.y_row, test_ball.x_column); //Erase Last Ball Position
			fflush(stdout);
			game_reset();
			return play_ball;
		}
	}
	return test_ball;
};

void update_score_and_time(player p1, player p2, uint32_t current_time){
	printf("hello3\r\n");
};

void game_reset(void){
	play_ball.x_column = 30;
	play_ball.y_row = 15;
	play_ball.last_x = 0;
	play_ball.last_y = 0;

	play_ball.x_speed = 1;
	play_ball.y_speed = 1;
};

