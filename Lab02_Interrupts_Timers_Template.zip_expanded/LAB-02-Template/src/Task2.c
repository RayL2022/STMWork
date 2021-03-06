//----------------------------------
// Task2.c (Lab02)
//----------------------------------
// Objective:
//  Configuring Timer6 Interrupt to count Elapsed
//	Time since program starts in tenths of a second
// 	using registers

//
//
// -- Imports ---------------
//
#include "init.h"

//
//
// -- Prototypes ------------
//
void Init_Timer(); //Configure Timer

double time_elapsed; //Variable to keep track of time elapsed (in tenths)

//
// -- Code Body -------------
//


int main() {
	Sys_Init();
    HAL_Init(); //Initialize HAL
	Init_Timer(); //Initialize Timer 6
	time_elapsed = 0; //Time cleared.
	while (1) {
	}
}

//
//
// -- Init Functions ----------
//
void Init_Timer() {

	//Enable interrupts on NVIC, position 22
	NVIC->ISER[1] |= 0x00400000;

	// Enable TIM6 clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	// Delay 2 clock cycles
	asm ( "nop" );
	asm ( "nop" );

	// Set pre-scaler to slow down ticks
	TIM6->PSC = 9999; // Chosen Fixed scaler value

	// Set the Auto-reload Value for 10Hz overflow (10Hz -> 100ms -> 0.1s)
	TIM6->ARR = 1079; // Value for 10Hz overflow given our fixed scaler

	// Generate update events to auto reload.
	TIM6->EGR |= 0x0001;

	// Enable Update Interrupts.
	TIM6->DIER |= 0x0001;

	// Start the timer, Tim6_ARR registered is buffered
	TIM6->CR1 = 0x0081;
}

void TIM6_DAC_IRQHandler() {
	// Clear Interrupt Bit
	TIM6->SR &= 0x0000;
	time_elapsed += 0.1; //Overflow indicates a tenth of second has passed
	printf("\033[2J\033[HTime Elapsed: %.1f seconds\r\n", time_elapsed); //Display this time
	//Formatted text is to clear screen and return to home position
}
