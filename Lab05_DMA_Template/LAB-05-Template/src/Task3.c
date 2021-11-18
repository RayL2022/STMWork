/*
 * Task3.c
 *
 *  Created on: Nov 1, 2021
 *      Author: Shayne
 */

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
DMA_HandleTypeDef hdma2;

//Variables to store readings/outputs
uint32_t current_reading[4];
volatile double last_reading;
volatile double second_last_reading;
volatile double current_output;
volatile double last_output;

//Coefficients for the Filter Equation
volatile double coeff1, coeff2, coeff3, coeff4;

void configureADC();
void configureDAC();
void configureDMA();

// Main Execution Loop
int main(void)
{
	//Initialize the system
	Sys_Init();
	configureDAC();
	configureADC();

	current_reading[0] = 0;
	last_reading = 0;
	second_last_reading = 0;
	current_output = 0;
	last_output = 0;

	HAL_ADC_Start_DMA(&hadc1, current_reading, 1);
	HAL_DAC_Start(&D1, DAC_CHANNEL_1);
	// Code goes here
	while(1){
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
	 hadc1.Init.DMAContinuousRequests = ENABLE;
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

	configureDMA();
	__HAL_LINKDMA(&hadc1, DMA_Handle, hdma2);

}

void configureDMA(){

	__HAL_RCC_DMA2_CLK_ENABLE();

	hdma2.Instance = DMA2_Stream0;
	hdma2.Init.Channel = DMA_CHANNEL_0;
	hdma2.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma2.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma2.Init.MemInc = DMA_MINC_ENABLE;
	hdma2.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma2.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma2.Init.Mode = DMA_CIRCULAR;
	hdma2.Init.Priority = DMA_PRIORITY_HIGH;
	hdma2.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma2.Init.MemBurst = DMA_MBURST_SINGLE;
	hdma2.Init.PeriphBurst = DMA_PBURST_SINGLE;
	//hdma1.XferCpltCallback = &DMATransferComplete;
	HAL_DMA_Init(&hdma2);
	//__HAL_LINKDMA(&S2,hdmarx,hdma2);

	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

void DMA2_Stream0_IRQHandler(void){
	HAL_DMA_IRQHandler(&hdma2);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){

	//C - Style Floating Point Equation
	current_output = 0.3125*(double)current_reading[0] + 0.240385*last_reading
			+ 0.3125*second_last_reading + 0.296875*last_output;

	//Assembly Implementation
/*
	current_output = 0;
	coeff1 = 0.3125;
	coeff2 = 0.240385;
	coeff3 = 0.3125;
	coeff4 = 0.296875;

	//First Coefficient and current_reading, multiply together and add to running result
	asm volatile ("VMLA.F64 %P[result], %P[oper1], %P[oper2]"
			: [result] "+&w" (current_output)
			: [oper1] "w" (coeff1), [oper2] "w" ((double)current_reading[0]));

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
			: [oper1] "w" (coeff4), [oper2] "w" (last_output));*/

/*	printf("Read: %lu, Out: %f, Last: %f, 2_Last: %f, Last_Out: %f, i: %lu\r\n"
				, current_reading[0], current_output, last_reading,
				second_last_reading, last_output, i);*/

	//Send Filtered Value to DAC Output
	HAL_DAC_SetValue(&D1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, current_output);

	//The current readings/output become the past ones
	second_last_reading = last_reading;
	last_reading = current_reading[0];
	last_output = current_output;
}

