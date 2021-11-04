//--------------------------------
// Lab 4 - Sample - Lab04_sample.c
//--------------------------------
// Task 4 - Take output from ADC, filter it, send output to DAC output
//

#include "init.h"

//TypeDef Structures for Needed Components
DAC_HandleTypeDef D1;
DAC_ChannelConfTypeDef D1_OUT;
ADC_HandleTypeDef hadc1;
ADC_ChannelConfTypeDef sConfig;

//Variables to store readings/outputs
volatile double current_reading;
volatile double last_reading;
volatile double second_last_reading;
volatile double current_output;
volatile double last_output;

//Coefficients for the Filter Equation
volatile double coeff1, coeff2, coeff3, coeff4;

void configureADC();
void TP_config();
void configureDAC();

// Main Execution Loop
int main(void)
{
	//Initialize the system
	Sys_Init();
	configureDAC();
	configureADC();
	TP_config();

	current_reading = 0;
	last_reading = 0;
	second_last_reading = 0;
	current_output = 0;
	last_output = 0;

	HAL_ADC_Start(&hadc1);
	HAL_DAC_Start(&D1, DAC_CHANNEL_1);
	// Code goes here
	while(1){

		//The current readings/output become the past ones
		second_last_reading = last_reading;
		last_reading = current_reading;
		last_output = current_output;

		HAL_ADC_PollForConversion(&hadc1, 1000);
		current_reading = HAL_ADC_GetValue(&hadc1);



/*
		//C - Style Floating Point Equation
		current_output = 0.3125*current_reading + 0.240385*last_reading
				+ 0.3125*second_last_reading + 0.296875*last_output;

*/


		//Extended Assembly -

		current_output = 0;
		coeff1 = 0.3125;
		coeff2 = 0.240385;
		coeff3 = 0.3125;
		coeff4 = 0.296875;

		//First Coefficient and current_reading, multiply together and add to running result
		asm volatile ("VMLA.F64 %P[result], %P[oper1], %P[oper2]"
				: [result] "+&w" (current_output)
				: [oper1] "w" (coeff1), [oper2] "w" (current_reading));

		//Second Coefficient and last_reading, multiply together and add to running result
		asm volatile ("VMLA.F64 %P[result], %P[oper1], %P[oper2]"
				: [result] "+&w" (current_output)
				: [oper1] "w" (coeff2), [oper2] "w" (last_reading));

		//Third Coefficient and second_last_reading, multiply together and add to running result
		asm volatile ("VMLA.F64 %P[result], %P[oper1], %P[oper2]"
				: [result] "+&w" (current_output)
				: [oper1] "w" (coeff3), [oper2] "w" (second_last_reading));

		//Fourth Coefficient and last_output, multiply together and add to running result
		asm volatile ("VMLA.F64 %P[result], %P[oper1], %P[oper2]"
				: [result] "+&w" (current_output)
				: [oper1] "w" (coeff4), [oper2] "w" (last_output));

		//Send Filtered Value to DAC Output
		HAL_DAC_SetValue(&D1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, current_output);

		//Toggle Pin to Measure Sampling Frequency
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
	}
}

void configureDAC()
{
	// Enable the DAC Clock.
	__HAL_RCC_DAC_CLK_ENABLE();

	D1.Instance = DAC1;
	HAL_DAC_Init(&D1); // Initialize the DAC

	// Configure the DAC channel
	D1_OUT.DAC_Trigger = DAC_TRIGGER_NONE;
	D1_OUT.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	HAL_DAC_ConfigChannel(&D1, &D1_OUT, DAC_CHANNEL_1);

}


void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
{
	// GPIO init
	GPIO_InitTypeDef  GPIO_InitStruct;
	// Enable GPIO Clocks
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Pin		  = GPIO_PIN_4;
	GPIO_InitStruct.Mode      = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull	  = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); //Port A, pin 4, A1 -DISCO

}


void configureADC()
{
	// Enable the ADC Clock.
	__HAL_RCC_ADC1_CLK_ENABLE();


	 /* Configure the global features of the ADC (Clock, Resolution, Data Alignment and number
	 of conversion) */
	 hadc1.Instance = ADC1;
	 hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
	 hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	 hadc1.Init.ScanConvMode = DISABLE;
	 hadc1.Init.ContinuousConvMode = ENABLE;
	 hadc1.Init.DiscontinuousConvMode = DISABLE;
	 hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	 hadc1.Init.NbrOfConversion = 1;
	 hadc1.Init.DMAContinuousRequests = DISABLE;
	 hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;

	 HAL_ADC_Init(&hadc1); // Initialize the ADC

	 /* Configure for the selected ADC regular channel its corresponding rank in the sequence\r
	 Analog-To-Digital Conversion 406
	 and its sample time. */
	 sConfig.Channel = ADC_CHANNEL_12;
	 sConfig.Rank = ADC_REGULAR_RANK_1;
	 sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	 HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}


void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{

// GPIO init
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitStruct.Mode      = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); //Pin C2, Arduino A2, ADC1, In 12

}

void TP_config(){ //Toggle Pin
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); //D0 - PC7
}

