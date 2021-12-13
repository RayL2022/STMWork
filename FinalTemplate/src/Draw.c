/*
 * Draw.c
 *
 *  Created on: Nov 30, 2021
 *      Author: Ray & Shayne
 */

#include "Draw.h"

//This Function will draw a paddle struct based on current coordinates
void draw_paddle(paddle my_paddle){
	//For each coordinate, go to stored row and column, color that position white
	printf("\033[%u;%uH\033[;47m ", my_paddle.first_y, my_paddle.first_x);
	printf("\033[%u;%uH\033[;47m ", my_paddle.second_y, my_paddle.second_x);
	printf("\033[%u;%uH\033[;47m ", my_paddle.third_y, my_paddle.third_x);
	printf("\033[%u;%uH\033[;47m ", my_paddle.fourth_y, my_paddle.fourth_x);
	fflush(stdout); //Using prints without \r\n
};

//This function will update paddle location based on state and return it
//This is done with assembly instruction math
paddle update_paddle(paddle my_paddle, char state){
	uint8_t value = 1; //variable that stores 1, used for asm math
	if (state == UP){ //The state read is up
		if ((my_paddle.first_y - 1) > MIN_ROW){ //Ensure we stay on screen and don't print to top row
			//Only need to erase fourth coordinate as all coordinates move up by one,
			//Erase simply turns the coordinate color to black
			printf("\033[%u;%uH\033[;40m ", my_paddle.fourth_y, my_paddle.fourth_x);
			fflush(stdout);

			//Subtract all paddle coordinates by 1 to go up
			asm volatile ("SUB %[result], %[oper1], %[oper2]"
					: [result] "+r" (my_paddle.first_y) //Read and Write to paddle variable
					: [oper1] "r" (my_paddle.first_y), [oper2] "r" (value)); //subtract 1
			asm volatile ("SUB %[result], %[oper1], %[oper2]"
					: [result] "+r" (my_paddle.second_y) //Read and Write to paddle variable
					: [oper1] "r" (my_paddle.second_y), [oper2] "r" (value)); //subtract 1
			asm volatile ("SUB %[result], %[oper1], %[oper2]"
					: [result] "+r" (my_paddle.third_y) //Read and Write to paddle variable
					: [oper1] "r" (my_paddle.third_y), [oper2] "r" (value)); //subtract 1
			asm volatile ("SUB %[result], %[oper1], %[oper2]"
					: [result] "+r" (my_paddle.fourth_y) //Read and Write to paddle variable
					: [oper1] "r" (my_paddle.fourth_y), [oper2] "r" (value)); //subtract 1
		}
	}
	else if (state == DOWN){ //The state read is down
		if ((my_paddle.fourth_y + 1) < MAX_ROW){ //Ensure we stay on screen and don't print to bottom row
			//Only need to erase fourth coordinate as all coordinates move down by one,
			printf("\033[%u;%uH\033[;40m ", my_paddle.first_y, my_paddle.first_x);
			fflush(stdout);

			//ADD all paddle coordinates by 1 to go down
			asm volatile ("ADD %[result], %[oper1], %[oper2]"
					: [result] "+r" (my_paddle.first_y) //Read and Write to paddle variable
					: [oper1] "r" (my_paddle.first_y), [oper2] "r" (value)); //subtract 1
			asm volatile ("ADD %[result], %[oper1], %[oper2]"
					: [result] "+r" (my_paddle.second_y) //Read and Write to paddle variable
					: [oper1] "r" (my_paddle.second_y), [oper2] "r" (value)); //subtract 1
			asm volatile ("ADD %[result], %[oper1], %[oper2]"
					: [result] "+r" (my_paddle.third_y) //Read and Write to paddle variable
					: [oper1] "r" (my_paddle.third_y), [oper2] "r" (value)); //subtract 1
			asm volatile ("ADD %[result], %[oper1], %[oper2]"
					: [result] "+r" (my_paddle.fourth_y) //Read and Write to paddle variable
					: [oper1] "r" (my_paddle.fourth_y), [oper2] "r" (value)); //subtract 1
		}
	}
	return my_paddle; //return updated paddle
};

//Erase last ball location and draw current location of ball
void draw_ball(ball test_ball){
	printf("\033[%u;%uH\033[;40m ", test_ball.last_y, test_ball.last_x); //Erase Last Ball Position
	printf("\033[%u;%uH\033[;43m ", test_ball.y_row, test_ball.x_column); //Draw current ball location (yellow)
	fflush(stdout);
};

//Update the ball location based on direction
ball update_ball(ball test_ball){
	//Update the last position of the ball before movement
	test_ball.last_y = test_ball.y_row;
	test_ball.last_x = test_ball.x_column;

	//Direction is bottom right: Positive, Positive, increment x and y accordingly
	if ((test_ball.y_dir > 0) && (test_ball.x_dir > 0)){
		test_ball.y_row += 1;
		test_ball.x_column += 1;
	}
	//Direction is top right: Positive, Negative, increment x and y accordingly
	else if ((test_ball.y_dir > 0) && (test_ball.x_dir < 0)){
		test_ball.y_row -= 1;
		test_ball.x_column += 1;
	}
	//Direction is bottom left: Negative, Positive, increment x and y accordingly
	else if ((test_ball.y_dir < 0) && (test_ball.x_dir > 0)){
		test_ball.y_row += 1;
		test_ball.x_column -= 1;
	}
	//Direction is top left: Negative, Negative, increment x and y accordingly
	else if ((test_ball.y_dir < 0) && (test_ball.x_dir < 0)){
		test_ball.y_row -= 1;
		test_ball.x_column -= 1;
	}

	return test_ball; //return updated ball struct
};

//Check if the ball should bounce before drawing
ball check_bounce(ball test_ball, paddle my_paddle, paddle opp_paddle){
	//Bounce on top of screen
	if (test_ball.y_row <= MIN_ROW){
		test_ball.y_row = MIN_ROW + 2; //Push the ball back some coordinates to bounce
		//Negative speed and direction x (columns)
		test_ball.x_speed *= -1;
		test_ball.x_dir *= -1;
	}
	//Bounce on bottom of screen
	else if (test_ball.y_row >= MAX_ROW){ //Push the ball back some coordinates to bounce
		test_ball.y_row = MAX_ROW - 2;
		//Negative speed and direction x (columns)
		test_ball.x_speed *= -1;
		test_ball.x_dir *= -1;
	}
	//Check left-side if my Paddle here, else p2 scores
	else if (test_ball.x_column - 1 == MIN_COL){ //Check 1 coordinate away from left edge
		//If the rows are equal then it should bounce
		//Check all paddle coordinates
		if (test_ball.y_row == my_paddle.first_y){
			test_ball.x_column = MIN_COL + 1; //Bounce by setting column forward
			//Negative speed and direction y (rows)
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
		else{//p2 scores because paddle coordinates weren't by the ball
			//Update the ball to be on the edge of the screen and draw it
			test_ball.x_column -= 1;
			draw_ball(test_ball);
			HAL_Delay(20);
			printf("\033[%u;%uH\033[;40m ", test_ball.y_row, test_ball.x_column); //Erase Last Ball Position
			fflush(stdout);
			p2.score += 1; //p2 scores is increased
			ball_reset(); //reset the ball
			return play_ball; //return updated ball
		}
	}

	//Check right-side if my Paddle here, else p2 scores
	else if (test_ball.x_column + 1 >= MAX_COL){ //Check 1 coordinate away from right edge
		//If the rows are equal then it should bounce
		//Check all paddle coordinates
		if (test_ball.y_row == opp_paddle.first_y){
			test_ball.x_column = MAX_COL - 1; //Bounce by setting column forward
			//Negative speed and direction y (rows)
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
		else{//p2 scores because paddle coordinates weren't by the ball
			//Update the ball to be on the edge of the screen and draw it
			test_ball.x_column += 1;
			draw_ball(test_ball);
			HAL_Delay(20);
			printf("\033[%u;%uH\033[;40m ", test_ball.y_row, test_ball.x_column); //Erase Last Ball Position
			fflush(stdout);
			p1.score += 1; //p2 scores is increased
			ball_reset(); //reset the ball
			return play_ball; //return updated ball
		}
	}
	return test_ball; //return updated ball
};

//For printing score and time
void update_score_and_time(player p1, player p2, uint8_t front_time, uint8_t back_time){
	//If the score is less than 10, pad a 0, place this at the top left corner
	if (p1.score < 10){
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, MIN_COL, 0); //pad 0
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, MIN_COL + 1, p1.score); //print p1 score
	}
	else{//Greater than 10, we can just print it
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, MIN_COL, p1.score);
	}
	//If the score is less than 10, pad a 0, place this at the top right corner
	if (p2.score < 10){
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, MAX_COL - 1, 0); //pad 0
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, MAX_COL, p2.score); //print p2 score
	}
	else{//Greater than 10, we can just print it
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, MAX_COL - 1, p2.score);
	}
	//Pad a zero if the minutes is less than 10, placed in the center of screen
	if (front_time < 10){
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, (MAX_COL/2) - 1, 0); //pad zero
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, (MAX_COL/2), front_time); //Minutes
	}
	else{//Just print the minutes, greater than 10
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, (MAX_COL/2) - 1, front_time);
	}
	//print the colon for time
	printf("\033[%u;%uH\033[;40m:", MIN_ROW, (MAX_COL/2) + 1);

	//If seconds is less than 10 pad a zero,centered
	if (back_time < 10){
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, (MAX_COL/2) + 2, 0); //pad zero
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, (MAX_COL/2) + 3, back_time); //seconds
	}
	else{ //Just print the seconds, greater than 10
		printf("\033[%u;%uH\033[;40m%u", MIN_ROW, (MAX_COL/2) + 2, back_time);
	}
};

//Reset the ball attributes
void ball_reset(void){

	//Make the ball start at a random row (y) but a fixed column the center (29)
	play_ball.x_column = (MAX_COL/2) + 1;
	play_ball.y_row = (rand() % ((MAX_ROW - 1) - (MIN_ROW + 1) + 1)) + (MIN_ROW + 1);
	play_ball.last_x = 0;
	play_ball.last_y = 0;

	//generate random speeds for x and y, ensure it is not 0
	play_ball.x_speed = 0;
	play_ball.y_speed = 0;
	while ((play_ball.x_speed == 0) || (play_ball.x_speed == 0)){
		play_ball.x_speed = (rand() % (MAX_X_SPEED - MIN_X_SPEED + 1)) + MIN_X_SPEED;
		play_ball.y_speed = (rand() % (MAX_Y_SPEED - MIN_Y_SPEED + 1)) + MIN_Y_SPEED;
	}

	//configure directions based on polarity of x and y speeds
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

//The printing of the choose time screen
void time_screen(void){
	printf("\033c\033[2J"); fflush(stdout); //Clear screen
	printf("\033[15;11H"); fflush(stdout); //Print on given row and column (15,11)
	printf("Select Game Time By Pressing: 1, 3, 5"); fflush(stdout); //Display message
	select_time(); //Choose time
	init_game(); //Initialize other game parameters
}

//Initializations of other game parameters
void init_game(void){
	my_current_state = 0; //Analog state reset
	int column = 1; //Used for formatting
	printf("\033c\033[2J"); fflush(stdout); //Clear screen

	//Format the top and bottom lines with dashes
	while (column < MAX_COL + 1){
		printf("\033[%u;%uH-", MIN_ROW, column); fflush(stdout);
		printf("\033[%u;%uH-", MAX_ROW, column); fflush(stdout);
		column += 1;
	}

	//Reset player scores
	p1.score = 0;
	p2.score = 0;

	ball_reset(); //Reset ball attributes

	//Initialize p1 and p2 paddles to be on the left or rightmost side, centered
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

}
