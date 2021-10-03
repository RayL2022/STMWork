//------------------------------------------------------------------------------------
// Task4.c
//------------------------------------------------------------------------------------
/* This code is creating a minimum of a 10x10 maze, created any way,
The maze should be have visibly different walls and free spaces,
the user's location must be a different variation from walls however
ie can't just be a simple color change, A LED should turn on upon completion
and the blue push button will reset the maze over, implicit functionality -
the user can't go out of the maze, or move (WASD) through a wall

*/


//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------
#include "stm32f769xx.h"
#include "hello.h"

#include<stdint.h>

void make_maze(); //Make and print maze
void configure_ports(); //Configure necessary ports for input and output pins
void make_move();

int current_row = 1; //Keep track of current row
int current_col = 7; //Keep track of current column
int test_col = 0; //Test a valid horizontal move
int test_row = 0; //Test a valid vertical move
char input;

int matrix[11][17] = {//Matrix that outlines the maze, 1's are walls, 0's are valid moves
{1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1},
{1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},
{1,0,0,0,1,0,0,0,1,1,1,1,1,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1},
{1,0,0,0,1,1,1,1,1,0,0,0,1,0,0,0,0},
{1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

//------------------------------------------------------------------------------------
// MAIN Routine
//------------------------------------------------------------------------------------
int main(void)
 {
    Sys_Init(); // This always goes at the top of main (defined in init.c)
    HAL_Init();
    make_maze();
    configure_ports();
	while(1){
		make_move();
	}
}

void configure_ports(){
    // Enables clocks for Port A and J
    __HAL_RCC_GPIOJ_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    //Initialize Push button (user)
    GPIO_InitTypeDef PB1;
    PB1.Pin = GPIO_PIN_0; //Push button on pin 0
    PB1.Mode = GPIO_MODE_INPUT; //Set to input
    PB1.Pull = GPIO_PULLUP; //Enable pull-up resistors
    HAL_GPIO_Init(GPIOA, &PB1); //Configure to Port A

    //Initialize LED02
    GPIO_InitTypeDef LED02;
    LED02.Pin = GPIO_PIN_5; //Set pin number
    LED02.Mode = GPIO_MODE_OUTPUT_PP; //Output type, with push-pull
    LED02.Pull = GPIO_NOPULL; //No pull
    HAL_GPIO_Init(GPIOJ, &LED02);//Configure to Port J
}
void make_maze(){
	printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
	printf("\033[34;47m"); fflush(stdout); //Blue text on white background
	//Print the maze
	printf("XXXXX   XXXXXXXXX\r\n");
	printf("X   X   X       X\r\n");
	printf("X   X   X   X   X\r\n");
	printf("X   X   XXXXX   X\r\n");
	printf("X               X\r\n");
	printf("X   XXXXXXXXXXXXX\r\n");
	printf("X   X   X       X\r\n");
	printf("X   X       X   X\r\n");
	printf("X   XXXXX   X    \r\n");
	printf("X           X   X\r\n");
	printf("XXXXXXXXXXXXXXXXX\r\n");

	printf("\033[0;7HO"); fflush(stdout); //Place the user at the starting position
	printf("\033[0;7H\033s"); fflush(stdout);//Highlight the current position, and save location
}

void make_move(){
	if ((current_row == 9) & (current_col == 17)){ //Check to see if the user has escaped maze
		HAL_GPIO_WritePin(GPIOJ,GPIO_PIN_5,GPIO_PIN_SET); //If so, turn LED on
		while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET); //Wait until restart
	}
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET){ //If the push button is pressed
		printf("\033[%d;%dH ", current_row, current_col); fflush(stdout); //erase last location
		HAL_GPIO_WritePin(GPIOJ,GPIO_PIN_5,GPIO_PIN_RESET);//Turn LED off
		current_row = 1; //Reset current row
		current_col = 7; //Reset current column
		test_col = 0; //Reset test column variable
		test_row = 0; //Reset test row variable
		printf("\033[0;7HO"); fflush(stdout); //Place the user at the starting position
		printf("\033[0;7H\033s"); fflush(stdout);//Highlight the current position, and save location
	}

	input = getchar(); //User move
	if ((input == 'A') | (input == 'a')){
		test_col = current_col - 1; //Test to see if the move is valid
		//Compare with matrix to see if move is valid, also not on non-negative
		if ((matrix[current_row - 1][test_col - 1] == 0) & (test_col >= 0)){
			printf("\033[%d;%dH ", current_row, current_col); fflush(stdout); //Erase last location
			current_col = test_col; //Valid move made, update current value
			printf("\033[%d;%dHO", current_row, current_col); fflush(stdout); //Move to new location
			printf("\033[%d;%dH", current_row, current_col); fflush(stdout); //Highlight location
		}else{
			printf("\a"); fflush(stdout); //Beep if the move was not valid
		}
	}else if ((input == 'D') | (input == 'd')){
		test_col = current_col + 1; //Test to see if the move is valid
		//Compare with matrix to see if move is valid, also not on not out of maze
		if ((matrix[current_row - 1][test_col-1] == 0) & (test_col <= 17)){
			printf("\033[%d,%dH ", current_row, current_col); fflush(stdout); //Erase last location
			current_col = test_col; //Valid move made, update current value
			printf("\033[%d;%dHO", current_row, current_col); fflush(stdout); //Move to new location
			printf("\033[%d;%dH", current_row, current_col); fflush(stdout); //Highlight location
			}else{
				printf("\a"); fflush(stdout); //Beep if the move was not valid
			}
	}else if ((input == 'S') | (input == 's')){
		test_row = current_row + 1;
		//Compare with matrix to see if move is valid, also not on not out of maze
		if ((matrix[test_row - 1][current_col - 1] == 0) & (test_row <= 11)){
			printf("\033[%d,%dH ", current_row, current_col); fflush(stdout);
			current_row = test_row;
			printf("\033[%d;%dHO", current_row, current_col); fflush(stdout);
			printf("\033[%d;%dH", current_row, current_col); fflush(stdout);
			}else{
				printf("\a"); fflush(stdout);
			}
	}else if ((input == 'W') | (input == 'w')){
		test_row = current_row - 1;
		//Compare with matrix to see if move is valid, also not on not out of maze
		if ((matrix[test_row - 1][current_col - 1] == 0) & (test_row >= 0)){
			printf("\033[%d,%dH ", current_row, current_col); fflush(stdout);
			current_row = test_row;
			printf("\033[%d;%dHO", current_row, current_col); fflush(stdout);
			printf("\033[%d;%dH", current_row, current_col); fflush(stdout);
		}else{
			printf("\a"); fflush(stdout);
		}
	}
}
