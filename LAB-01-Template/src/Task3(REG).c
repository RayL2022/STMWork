//------------------------------------------------------------------------------------
// Task3(REG).c
//------------------------------------------------------------------------------------
/* This code is for the register implementation of task 3.
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

int Input_for_D0;
int Input_for_D1;
int Input_for_D2;
int Input_for_D3;

//------------------------------------------------------------------------------------
// MAIN Routine
//------------------------------------------------------------------------------------
int main(void)
 {
    Sys_Init(); // This always goes at the top of main (defined in init.c)
    configure_ports();
	while(1){
		check_state();
	}
}

void configure_ports(){
    // Need to enable clock for peripheral buses using registers (Ports J, A, D, C, F)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;

    //Bit mask to set LEDs as Output (LD1-LD4)
    GPIOJ->MODER |= 0x04000400U;
    GPIOA->MODER |= 0x01000000U;
    GPIOD->MODER |= 0x00000100U;

    //Bit mask to set input for D0-D3
    GPIOC->MODER &= 0xFFFF0FFFU;
    GPIOJ->MODER &= 0xFFFFFFF3U;
    GPIOF->MODER &= 0xFFFFCFFFU;

    //Bit mask to set pull-up resistors for D0-D3
    GPIOC->PUPDR |= 0x00005000U;
    GPIOJ->PUPDR |= 0x00000004U;
    GPIOF->PUPDR |= 0x00001000U;
}
void check_state(){
	Input_for_D0 = GPIOC->IDR & 0x0080; //Value will be 0x0080 if D0 is high (Port pin)
	Input_for_D1 = GPIOC->IDR & 0x0040; //Value will be 0x0040 if D1 is high (Port pin)
	Input_for_D2 = GPIOJ->IDR & 0x0002; //Value will be 0x0002 if D2 is high (Port pin)
	Input_for_D3 = GPIOF->IDR & 0x0040; //Value will be 0x0040 if D3 is high (Port pin)

	if (Input_for_D0 == 0x0080){//Check if D0 is high
		GPIOJ->BSRR = (uint16_t)GPIO_PIN_13; // Turn on Red LED (LED1)
	}
	else if (Input_for_D0 == 0x0000){//D0 is off (low)
		GPIOJ->BSRR = (uint32_t)GPIO_PIN_13 << 16; // Turn off Red LED (LED1)
	}
	if (Input_for_D1 == 0x0040){//Check if D1 is high
	    GPIOJ->BSRR = (uint16_t)GPIO_PIN_5; // Turn on Green LED (LED2)
	}
	else if (Input_for_D1 == 0x0000){//D1 is low
	    GPIOJ->BSRR = (uint32_t)GPIO_PIN_5 << 16; // Turn off Green LED (LED2)
	}
	if (Input_for_D2 == 0x0002){//Check if D2 is high
	    GPIOA->BSRR = (uint16_t)GPIO_PIN_12; // Turn on Green LED (LED3)
	}
	else if (Input_for_D2 == 0x0000){//D2 is low
	    GPIOA->BSRR = (uint32_t)GPIO_PIN_12 << 16; // Turn off Green LED (LED3)
	}
	if (Input_for_D3 == 0x0040){//Check if D3 is high
	    GPIOD->BSRR = (uint16_t)GPIO_PIN_4 << 16; // Turn on Red LED (LED4) (Note:inverted)
	}
	else if (Input_for_D3 == 0x0000){//Check if D3 is low
	    GPIOD->BSRR = (uint32_t)GPIO_PIN_4; // Turn off RED LED (LED4)
	}
}
