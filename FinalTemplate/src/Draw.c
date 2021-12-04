#include "Draw.h"

void draw_paddle(paddle test_paddle){
	printf("hello\r\n");
};
void draw_ball(ball test_ball){
	printf("\033[%u;%uH\033[;40m ", test_ball.last_x, test_ball.last_y);
	printf("\033[%u;%uH\033[;43m ", test_ball.x_row, test_ball.y_column);
	fflush(stdout);

};

void update_score_and_time(player p1, player p2, uint32_t current_time){
	printf("hello3\r\n");
};

void game_reset(void){
	printf("hello4\r\n");
};
