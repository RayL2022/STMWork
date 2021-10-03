//------------------------------------------------------------------------------------
// Task3(HAL).c
//------------------------------------------------------------------------------------
/* This code is for the HAL implementation of task 3.
Task 3 requires us to set pins D0-3 to digital inputs with
pull up resistors, LEDs LD1-4 to be digital outputs and for
the program to continuously read the input and output the corresponding
lit/non-lit LED based on it D0 controls LD1, high on input, light on
*/


//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------
#include "stm32f769xx.h"
#include "hello.h"

#include<stdint.h>

void configure_ports(); //Configure necessary ports for input and output pins
void check_state(); //Check state of input and relate to corresponding LED output

//------------------------------------------------------------------------------------
// MAIN Routine
//------------------------------------------------------------------------------------
int main(void)
 {
    Sys_Init(); // This always goes at the top of main (defined in init.c)
    HAL_Init(); //Initialize HAL
    configure_ports();
	while(1){
		check_state();
	}
}
void configure_ports(){
    // Need to enable clock for peripheral bus on GPIO Port J, A, D, C, F
    __HAL_RCC_GPIOJ_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();

    // Make a GPIO structure for LED01
    GPIO_InitTypeDef LED01;
    LED01.Pin = GPIO_PIN_13; //Set the pin number
    LED01.Mode = GPIO_MODE_OUTPUT_PP; //Output type, with push-pull
    LED01.Pull = GPIO_NOPULL; // No pull-up
    HAL_GPIO_Init(GPIOJ, &LED01); //Configure this to Port J

    // Make a GPIO structure for LED02
    GPIO_InitTypeDef LED02;
    LED02.Pin = GPIO_PIN_5;
    LED02.Mode = GPIO_MODE_OUTPUT_PP;
    LED02.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOJ, &LED02); //Configure this to Port J

    // Make a GPIO structure for LED03
    GPIO_InitTypeDef LED03;
    LED03.Pin = GPIO_PIN_12;
    LED03.Mode = GPIO_MODE_OUTPUT_PP;
    LED03.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &LED03); //Configure this to Port A

    // Make a GPIO structure for LED04
    GPIO_InitTypeDef LED04;
    LED04.Pin = GPIO_PIN_4;
    LED04.Mode = GPIO_MODE_OUTPUT_PP;
    LED04.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &LED04); //Configure this to Port D

    //Make GPIO structure for D0
    GPIO_InitTypeDef D0;
    D0.Pin = GPIO_PIN_7; //Set the pin number
	D0.Mode = GPIO_MODE_INPUT; //Input type
	D0.Pull = GPIO_PULLUP; //Enable pull-up resistors
    HAL_GPIO_Init(GPIOC, &D0); //Configure this to Port C

    //Make GPIO structure for D1
    GPIO_InitTypeDef D1;
    D1.Pin = GPIO_PIN_6;
	D1.Mode = GPIO_MODE_INPUT;
	D1.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &D1); //Configure this to Port C

    //Make GPIO structure for D2
    GPIO_InitTypeDef D2;
    D2.Pin = GPIO_PIN_1;
	D2.Mode = GPIO_MODE_INPUT;
	D2.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOJ, &D2); //Configure this to Port J

	//Make GPIO structure for D2
    GPIO_InitTypeDef D3;
    D3.Pin = GPIO_PIN_6;
	D3.Mode = GPIO_MODE_INPUT;
	D3.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOF, &D3); //Configure this to Port F
}
void check_state(){
	//Read the pin on Port C, pin 7 and see if it is set, if yes write to
	//port J, pin 13 and set this as well, checking if D0 is high, if yes
	//then set the LED01 to high as well
	if (HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7) == GPIO_PIN_SET){
		HAL_GPIO_WritePin(GPIOJ,GPIO_PIN_13,GPIO_PIN_SET);
	}//else check if D0 is off, then set LED01 to off if yes
	else if (HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7) == GPIO_PIN_RESET){
		HAL_GPIO_WritePin(GPIOJ,GPIO_PIN_13,GPIO_PIN_RESET);
	}//Check if D1 is ON, if yes, turn LED02 ON
	if (HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6) == GPIO_PIN_SET){
		HAL_GPIO_WritePin(GPIOJ,GPIO_PIN_5,GPIO_PIN_SET);
	}//else check if D1 is off, then set LED02 to off if yes
	else if (HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6) == GPIO_PIN_RESET){
		HAL_GPIO_WritePin(GPIOJ,GPIO_PIN_5,GPIO_PIN_RESET);
	}//Check if D2 is ON, if yes, turn LED03 ON
	if (HAL_GPIO_ReadPin(GPIOJ,GPIO_PIN_1) == GPIO_PIN_SET){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
	}//else check if D2 is off, then set LED03 to off if yes
	else if (HAL_GPIO_ReadPin(GPIOJ,GPIO_PIN_1) == GPIO_PIN_RESET){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
	}//Check if D3 is ON, if yes, then turn LED04 off (Note: inverted)
	if (HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_6) == GPIO_PIN_SET){
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_RESET); //(Note: inverted)
	}//else check if D3 is off, if yes, then turn LED04 ON
	else if (HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_6) == GPIO_PIN_RESET){
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET);
	}
}
