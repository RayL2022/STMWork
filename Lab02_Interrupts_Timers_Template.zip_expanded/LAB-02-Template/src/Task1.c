//----------------------------------
// Task1.c (Lab02)
//----------------------------------
// This code is to configure and connect 2 EXTI Interrupt Lines
// One EXTI should be with Registers and the other with HAL


//
// -- Imports ---------------
//
#include "init.h"

//
// -- Prototypes ------------
//
void Init_GPIO(); //Configure all GPIOs
void Init_Interrupts(); //Configure Interrupts

//
//
// -- Code Body -------------
//
volatile uint8_t D4_button = 0; //Button connected to D4
volatile uint8_t D5_button = 0; //Button connected to D5


int main() {
	Sys_Init();
    HAL_Init(); //Initialize HAL
	Init_GPIO(); //Initialize GPIO
	Init_Interrupts(); //Initialize Interrupts

	while (1) {
		if (D4_button == 1){
			printf("D4 button is ON\r\n");
			D4_button = 0; //Clear button
		}
		if (D5_button == 1){
			printf("D5 button is ON\r\n");
			D5_button = 0; //Clear button
		}
	}
}

void Init_GPIO() {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN; //Enable Clock for Port J (REG)
    __HAL_RCC_GPIOC_CLK_ENABLE(); //Enable Clock for Port C (HAL)

	// Enable clock to SYSCONFIG module to enable writing of EXTICRn registers
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	// Delay after an RCC peripheral clock enabling (2 clock cycles)
	asm ("nop");
	asm ("nop");

	//Configure D4(PJ0) as input, with pull-up resistors enabled (REG)
	GPIOJ->MODER &= 0xFFFFFFFCU; //Set the MODER pin to input
	GPIOJ->PUPDR |= 0x00000001U; //Set the PUPDR pin to pull-up

    //Configure D5(PC8) as input, with pull-up resistors enabled (HAL)
    GPIO_InitTypeDef D5;
    D5.Pin = GPIO_PIN_8; //On pin 8
	D5.Mode = GPIO_MODE_IT_RISING; //Set Mode to Interrupt on Rising Edge
	D5.Pull = GPIO_PULLUP; //Enable pull-up
	HAL_GPIO_Init(GPIOC, &D5); //Configure this to Port C

}
void Init_Interrupts(){

	//Re-route EXTI0 to GPIOJ pin 0
	SYSCFG->EXTICR[0] &= 0xFFF9; // (1001), set the 0s
	SYSCFG->EXTICR[0] |= 0xFFF9; // (1001), set the 1

	//Set interrupt enable for EXTI0.
	NVIC->ISER[0] |= 0x00000040;

	//Set interrupt enable for EXTI8 (included in 9_5)
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	// Unmask interrupt.
	EXTI->IMR |= 0x00000001;

	// Register for rising edge.
	EXTI->RTSR |= 0x00000001;
}

//
//
// -- ISRs (IRQs) -------------
//

// Non-HAL GPIO/EXTI Handler
void EXTI0_IRQHandler() {
	// Clear Interrupt Bit by setting it to 1.
	EXTI->PR |= 0x00000001;
	D4_button = 1; //Button is on
	for (int i = 0; i < 50; i++); //Give Enough Time to clear properly
}

//HAL - GPIO/EXTI Handler
void EXTI9_5_IRQHandler() {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8); //Pin 8 is the interrupt trigger
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if (GPIO_Pin == GPIO_PIN_8){
		D5_button = 1; //Button is on
	}
}
