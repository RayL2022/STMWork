#ifndef DRAW_H_
#define	DRAW_H_

#include "stm32f769xx.h"
#include "stm32f7xx_hal.h"
#include "uart.h"

typedef struct{
	uint8_t x_row, y_column, x_speed, y_speed, last_x, last_y;
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
	uint8_t score, orientation;
} player;

paddle p1_paddle;
ball play_ball;
player p1;
player p2;

//Function Prototypes
void draw_paddle(paddle test_paddle);
void draw_ball(ball test_ball);
void update_score_and_time(player p1, player p2, uint32_t current_time);
void game_reset(void);

#endif
