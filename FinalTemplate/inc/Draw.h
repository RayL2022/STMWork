#ifndef DRAW_H_
#define	DRAW_H_

#include "stm32f769xx.h"
#include "stm32f7xx_hal.h"
#include "uart.h"

#define MIN_ROW 1
#define MAX_ROW 30
#define MIN_COL 1
#define MAX_COL 57

//We can round that x speed is 2 times faster than x speed
#define MIN_X_SPEED -10
#define MAX_X_SPEED 20
#define MIN_Y_SPEED -5
#define MAX_Y_SPEED 10

//Right now we don't handle if X or Y is zero


typedef struct{
	uint8_t x_column, y_row, last_x, last_y;
	int x_speed, y_speed, x_dir, y_dir;
} ball;

typedef struct{
	uint8_t
	first_x,
	first_y,

	second_x,
	second_y,

	third_x,
	third_y,

	fourth_x,
	fourth_y;

} paddle;

typedef struct{
	uint8_t score;
} player;

paddle p1_paddle;
paddle p2_paddle;
ball play_ball;
player p1;
player p2;

//Function Prototypes
void draw_paddle(paddle my_paddle);
paddle update_paddle(paddle my_paddle, char state);
void draw_ball(ball test_ball);
ball update_ball(ball test_ball);
ball check_bounce(ball test_ball, paddle my_paddle, paddle opp_paddle);
void update_score_and_time(player p1, player p2, uint8_t front_time, uint8_t back_time);
void ball_reset(void);
void time_screen(void);
void init_game(void);

#endif
