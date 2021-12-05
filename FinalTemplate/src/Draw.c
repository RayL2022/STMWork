#include "Draw.h"
#include "ADC.h"

void draw_paddle(paddle test_paddle){
	printf("\033[%u;%uH\033[;47m ", test_paddle.first_x, test_paddle.first_y);
	printf("\033[%u;%uH\033[;47m ", test_paddle.second_x, test_paddle.second_y);
	printf("\033[%u;%uH\033[;47m ", test_paddle.third_x, test_paddle.third_y);
	printf("\033[%u;%uH\033[;47m ", test_paddle.fourth_x, test_paddle.fourth_y);
	fflush(stdout);
};

paddle update_paddle(paddle test_paddle, char state){
	if (state == UP){
		if (test_paddle.first_x - 1 >= 2){
			printf("\033[%u;%uH\033[;40m ", test_paddle.fourth_x, test_paddle.fourth_y);
			test_paddle.first_x -= 1;
			test_paddle.second_x -= 1;
			test_paddle.third_x -= 1;
			test_paddle.fourth_x -= 1;
		}
	}
	else if (state == DOWN){
		if (test_paddle.fourth_x + 1 <= 23){
			printf("\033[%u;%uH\033[;40m ", test_paddle.first_x, test_paddle.first_y);
			test_paddle.first_x += 1;
			test_paddle.second_x += 1;
			test_paddle.third_x += 1;
			test_paddle.fourth_x +=1;
		}
	}
	return test_paddle;
};

void draw_ball(ball test_ball){
	printf("\033[%u;%uH\033[;40m ", test_ball.last_x, test_ball.last_y); //Erase Last Ball Position
	printf("\033[%u;%uH\033[;43m ", test_ball.x_row, test_ball.y_column);
	fflush(stdout);
};

void update_ball(ball test_ball){

};

void update_score_and_time(player p1, player p2, uint32_t current_time){
	printf("hello3\r\n");
};

void game_reset(void){
	printf("hello4\r\n");
};

