#include "Draw.h"
#include "ADC.h"
#include "Timer.h"
#include <stdio.h>
#include <stdlib.h>

void draw_paddle(paddle my_paddle){
	printf("\033[%u;%uH\033[;47m ", my_paddle.first_y, my_paddle.first_x);
	printf("\033[%u;%uH\033[;47m ", my_paddle.second_y, my_paddle.second_x);
	printf("\033[%u;%uH\033[;47m ", my_paddle.third_y, my_paddle.third_x);
	printf("\033[%u;%uH\033[;47m ", my_paddle.fourth_y, my_paddle.fourth_x);
	fflush(stdout);
};

paddle update_paddle(paddle my_paddle, char state){
	uint8_t one = 1;
	if (state == UP){
		if ((my_paddle.first_y - 1) > MIN_ROW){
			printf("\033[%u;%uH\033[;40m ", my_paddle.fourth_y, my_paddle.fourth_x);
			fflush(stdout);

			asm volatile ("SUB %[result], %[oper1], %[oper2]"
					: [result] "+r" (my_paddle.first_y) //Read and Write to paddle variable
					: [oper1] "r" (my_paddle.first_y), [oper2] "r" (one)); //subtract 1
			asm volatile ("SUB %[result], %[oper1], %[oper2]"
					: [result] "+r" (my_paddle.second_y) //Read and Write to paddle variable
					: [oper1] "r" (my_paddle.second_y), [oper2] "r" (one)); //subtract 1
			asm volatile ("SUB %[result], %[oper1], %[oper2]"
					: [result] "+r" (my_paddle.third_y) //Read and Write to paddle variable
					: [oper1] "r" (my_paddle.third_y), [oper2] "r" (one)); //subtract 1
			asm volatile ("SUB %[result], %[oper1], %[oper2]"
					: [result] "+r" (my_paddle.fourth_y) //Read and Write to paddle variable
					: [oper1] "r" (my_paddle.fourth_y), [oper2] "r" (one)); //subtract 1
			/*
			my_paddle.first_y -= 1;
			my_paddle.second_y -= 1;
			my_paddle.third_y -= 1;
			my_paddle.fourth_y -= 1;
			*/
		}
	}
	else if (state == DOWN){
		if ((my_paddle.fourth_y + 1) < MAX_ROW){
			printf("\033[%u;%uH\033[;40m ", my_paddle.first_y, my_paddle.first_x);
			fflush(stdout);

			asm volatile ("ADD %[result], %[oper1], %[oper2]"
					: [result] "+r" (my_paddle.first_y) //Read and Write to paddle variable
					: [oper1] "r" (my_paddle.first_y), [oper2] "r" (one)); //subtract 1
			asm volatile ("ADD %[result], %[oper1], %[oper2]"
					: [result] "+r" (my_paddle.second_y) //Read and Write to paddle variable
					: [oper1] "r" (my_paddle.second_y), [oper2] "r" (one)); //subtract 1
			asm volatile ("ADD %[result], %[oper1], %[oper2]"
					: [result] "+r" (my_paddle.third_y) //Read and Write to paddle variable
					: [oper1] "r" (my_paddle.third_y), [oper2] "r" (one)); //subtract 1
			asm volatile ("ADD %[result], %[oper1], %[oper2]"
					: [result] "+r" (my_paddle.fourth_y) //Read and Write to paddle variable
					: [oper1] "r" (my_paddle.fourth_y), [oper2] "r" (one)); //subtract 1
			/*
			my_paddle.first_y += 1;
			my_paddle.second_y += 1;
			my_paddle.third_y += 1;
			my_paddle.fourth_y += 1;
			*/
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
	test_ball.last_y = test_ball.y_row;
	test_ball.last_x = test_ball.x_column;

	if ((test_ball.y_dir > 0) && (test_ball.x_dir > 0)){ //POS, POS
		test_ball.y_row += 1;
		test_ball.x_column += 1;
	}
	else if ((test_ball.y_dir > 0) && (test_ball.x_dir < 0)){ //POS, NEG
		test_ball.y_row -= 1;
		test_ball.x_column += 1;
	}
	else if ((test_ball.y_dir < 0) && (test_ball.x_dir > 0)){ //NEG, POS
		test_ball.y_row += 1;
		test_ball.x_column -= 1;
	}
	else if ((test_ball.y_dir < 0) && (test_ball.x_dir < 0)){ //NEG, NEG
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
		test_ball.x_dir *= -1;
	}
	else if (test_ball.y_row >= MAX_ROW){
		//Bounce, Bottom
		test_ball.y_row = MAX_ROW - 2;
		test_ball.x_speed *= -1;
		test_ball.x_dir *= -1;
	}

	else if (test_ball.x_column - 1 == MIN_COL){
		//Check if Paddle here, else p2 scores (left)
		if (test_ball.y_row == my_paddle.first_y){\
			test_ball.x_column = MIN_COL + 1;
			test_ball.y_speed *= -1;
			test_ball.y_dir *= -1;
		}
		else if (test_ball.y_row == my_paddle.second_y){
			test_ball.x_column = MIN_COL + 1;
			test_ball.y_speed *= -1;
			test_ball.y_dir *= -1;
		}
		else if (test_ball.y_row == my_paddle.third_y){
			test_ball.x_column = MIN_COL + 1;
			test_ball.y_speed *= -1;
			test_ball.y_dir *= -1;
		}
		else if (test_ball.y_row == my_paddle.fourth_y){
			test_ball.x_column = MIN_COL + 1;
			test_ball.y_speed *= -1;
			test_ball.y_dir *= -1;
		}
		else{
			test_ball.x_column -= 1;
			draw_ball(test_ball);
			HAL_Delay(20);
			printf("\033[%u;%uH\033[;40m ", test_ball.y_row, test_ball.x_column); //Erase Last Ball Position
			fflush(stdout);
			p2.score += 1;
			//update_score_and_time(p1, p2, 4, 56);
			game_reset();
			return play_ball;
		}
	}

	else if (test_ball.x_column + 1 >= MAX_COL){
		//Check if Paddle here, else p1 scores (right)
		if (test_ball.y_row == opp_paddle.first_y){
			test_ball.x_column = MAX_COL - 1;
			test_ball.y_speed *= -1;
			test_ball.y_dir *= -1;
		}
		else if (test_ball.y_row == opp_paddle.second_y){
			test_ball.x_column = MAX_COL - 1;
			test_ball.y_speed *= -1;
			test_ball.y_dir *= -1;
		}
		else if (test_ball.y_row == opp_paddle.third_y){
			test_ball.x_column = MAX_COL - 1;
			test_ball.y_speed *= -1;
			test_ball.y_dir *= -1;
		}
		else if (test_ball.y_row == opp_paddle.fourth_y){
			test_ball.x_column = MAX_COL - 1;
			test_ball.y_speed *= -1;
			test_ball.y_dir *= -1;
		}
		else{
			test_ball.x_column += 1;
			draw_ball(test_ball);
			HAL_Delay(20);
			printf("\033[%u;%uH\033[;40m ", test_ball.y_row, test_ball.x_column); //Erase Last Ball Position
			fflush(stdout);
			p1.score += 1;
			//update_score_and_time(p1, p2, 4, 56);
			game_reset();
			return play_ball;
		}
	}
	return test_ball;
};

void update_score_and_time(player p1, player p2, uint8_t front_time, uint8_t back_time){
	if (p1.score < 10){
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, MIN_COL, 0);
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, MIN_COL + 1, p1.score);
	}
	else{
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, MIN_COL, p1.score);
	}

	if (p2.score < 10){
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, MAX_COL - 1, 0);
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, MAX_COL, p2.score);
	}
	else{
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, MAX_COL - 1, p2.score);
	}
	if (front_time < 10){
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, (MAX_COL/2) - 1, 0);
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, (MAX_COL/2), front_time);
	}
	else{
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, (MAX_COL/2) - 1, front_time);
	}

	printf("\033[%u;%uH\033[;40m:", MIN_ROW, (MAX_COL/2) + 1);

	if (back_time < 10){
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, (MAX_COL/2) + 2, 0);
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, (MAX_COL/2) + 3, back_time);
	}
	else{
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, (MAX_COL/2) + 2, back_time);
	}
};

void game_reset(void){
	play_ball.x_column = 29;
	play_ball.y_row = (rand() % ((MAX_ROW - 1) - (MIN_ROW + 1) + 1)) + (MIN_ROW + 1);
	play_ball.last_x = 0;
	play_ball.last_y = 0;

	//printf("\033c\033[18;18H%u, %u\r\n", play_ball.x_column, play_ball.y_row);

	play_ball.x_speed = 0;
	play_ball.y_speed = 0;
	while ((play_ball.x_speed == 0) || (play_ball.x_speed == 0)){
		play_ball.x_speed = (rand() % (MAX_X_SPEED - MIN_X_SPEED + 1)) + MIN_X_SPEED;
		play_ball.y_speed = (rand() % (MAX_Y_SPEED - MIN_Y_SPEED + 1)) + MIN_Y_SPEED;
	}

	//printf("\033[20;20H%d, %d\r\n", play_ball.x_speed, play_ball.y_speed);
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

};

