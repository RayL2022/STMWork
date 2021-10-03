//------------------------------------------------------------------------------------
// Test1+2.c
//------------------------------------------------------------------------------------
/*
This code executes the requirements for task 1 and task 2.
Task 1 is a generally simple task so it was combined with task 2 functionality.
Task 1 requires us to grab input from the user and display which key they
pressed on screen, and for any escape character pressed to halt the program.
Task 2 requires us to display yellow text on a blue background, with the
character pressed displayed in red, centered on line 6. If an non-printable character is
pressed it will be printed on line 12 in formatted blinking text. Subsequent invalid
characters will be printed on next available lines and force scroll when line 24 is reached.
*/

//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------
#include "stm32f769xx.h"
#include "hello.h"

#include<stdint.h>


char choice; //user key input
int first_invalid; //variable to track if it is the first invalid character pressed
int is_line_24; //variable for tracking line number of scroll region

void Initialize(); //Some initialization steps needed outside while loop
void check_valid(char choice); //Checks if a valid key is pressed and also performs appropriate steps based on line number

//------------------------------------------------------------------------------------
// MAIN Routine
//------------------------------------------------------------------------------------
int main(void)
 {
   Sys_Init(); // This always goes at the top of main (defined in init.c)
   Initialize(); //Calls initialization function
   while(1){
    	printf("\033[33;44m"); fflush(stdout); //Correct for text change to red, reset to default colors
		choice = getchar(); //Get user choice
		if (choice == '\033') return 1; //Halt if escape character is used
		else{
			check_valid(choice);
		}
    }
}

void Initialize(){
   printf("\033[2J"); fflush(stdout); // Erase screen & move cursor to home position
   printf("\033[33;44m"); fflush(stdout); //Yellow text on a blue background

	printf("\033[2;22HPRESS <ESC> KEY OR <CTRL> + [ TO QUIT.\r\n"); //Exit procedure, centered

	printf("\033[12;25r"); fflush(stdout); //Scroll region between lines 12 and 25

	first_invalid = 0; //Initially false
	is_line_24 = 12; //Scroll region starts on line 12
}

void check_valid(char choice){
	if (((choice >= 0) & (choice <= 31)) | (choice == 127)){ //ASCII value for non-printables
		if (first_invalid == 0){ //If the first invalid character is pressed
			//Beep, move to left edge of line 12, blink the text, display value in hex, and underline section
			//reset the formatted text after
			printf("\a\033[12;0H\033[5mThe keyboard character $%02x is \033[4m'not printable'\033[0;33;44m.\r\n",choice);
			first_invalid = 1; //No longer false
			printf("\033[s"); fflush(stdout); //Save current cursor position (line 13)
			is_line_24++; //Increment line count
		}
		else{//additional invalid characters
			printf("\033[u"); fflush(stdout); //Load saved position
			if (is_line_24 < 24){//Before line 24
				printf("\a\033[5mThe keyboard character $%02x is \033[4m'not printable'\033[0;33;44m.\r\n",choice);
				printf("\033[s"); fflush(stdout); //Save current cursor position
				is_line_24++;
			}
			else if (is_line_24 == 24){ //Special case when we are on line 24, we just need to print on line 24 (no \r\n)
				printf("\a\033[5mThe keyboard character $%02x is \033[4m'not printable'\033[0;33;44m.",choice); fflush(stdout);
				is_line_24++;
			}
			else if (is_line_24 > 24){ //When we need to scroll the section, scroll first with new line then print
				printf("\a\r\n\033[5mThe keyboard character $%02x is \033[4m'not printable'\033[0;33;44m.",choice); fflush(stdout);
			}
		}
	}
	else{//A valid character is pressed
		//Go to line 6 and print the formatted text
		printf("\033[6;0HThe keyboard character is \033[31m%c", choice); fflush(stdout);
	}
}
