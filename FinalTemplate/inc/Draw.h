#ifndef DRAW_H_
#define	DRAW_H_

/* Includes */
#include "stm32f769xx.h"
#include "stm32f7xx_hal.h"
#include "uart.h"
#include "ADC.h" //Need my state
#include "Timer.h" //Need to call to start timer
#include "ConfUart.h" //Need opponent's state
//Used for Random function
#include <stdio.h>
#include <stdlib.h>

/* Defines */
//Define a resolution for the terminal board output
#define MIN_ROW 1 //Topmost row
#define MAX_ROW 30 //Bottom row
#define MIN_COL 1 //Leftmost column
#define MAX_COL 57 //Rightmost column

//We can assume that x speed (columns) is 2 times faster than y speed (rows)
//This is based on the approximate dimensions of board
#define MIN_X_SPEED -10
#define MAX_X_SPEED 20
#define MIN_Y_SPEED -5
#define MAX_Y_SPEED 10

/* Typedef structs */
//This ball struct keeps track of current x and y location and last x and y
//Y is defined as the rows and x is defined as the columns
//Keep track of x and y speeds and there direction
typedef struct{
	uint8_t x_column, y_row, last_x, last_y;
	int x_speed, y_speed, x_dir, y_dir;
} ball;

//This paddle struct keeps track of the individual paddle coordinates (x, y)
typedef struct{
	uint8_t
	first_x, //First coordinate
	first_y,

	second_x, //Second coordinate
	second_y,

	third_x, //Third coordinate
	third_y,

	fourth_x, //Fourth coordinate
	fourth_y;

} paddle;

//This player struct keeps track of player score, other variables regarding player
//could be added
typedef struct{
	uint8_t score;
} player;

/* Global Variables */
paddle p1_paddle; //Player 1's paddle (left)
paddle p2_paddle; //Player 2's paddle (right)
ball play_ball; //The ball in play
player p1; //Player 1
player p2; //Player 2

/* Function Prototypes */
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
