/*
 * Task3.c
 *
 *  Created on: Nov 1, 2021
 *      Author: Ray & Shayne
 */
// Task 3 - Reconfigure Lab 04 task 04 with the ADC using DMA

//Includes needed for Operation
#include "init.h"

//TypeDef Structures for Needed Components
DAC_HandleTypeDef D1; //DAC HandleType
DAC_ChannelConfTypeDef D1_OUT; //DAC Channel HandleType
ADC_HandleTypeDef hadc1; //ADC HandleType
ADC_ChannelConfTypeDef sConfig; //ADC Channel HandleType
DMA_HandleTypeDef hdma2; //DMA HandleType

//Variables to store readings/outputs
uint32_t current_reading[4]; //Buffer to store ADC readings, 32 bit uint
volatile double last_reading; //Last reading from ADC
volatile double second_last_reading; //2nd previous reading from ADC
volatile double current_output; //Store the current output to give to DAC
volatile double last_output; //Store last output

//Coefficients for the Filter Equation
volatile double coeff1, coeff2, coeff3, coeff4;

//Prototypes
void configureADC(); //ADC Configurations Needed
void configureDAC(); //DAC Configurations Needed
void configureDMA(); //DMA Configurations Needed

// Main Execution Loop
int main(void)
{
	//Initialize the system
	Sys_Init();
	configureDAC();
	configureADC();

	//Initialize variables to 0
	current_reading[0] = 0;
	last_reading = 0;
	second_last_reading = 0;
	current_output = 0;
	last_output = 0;

	//Start ADC in DMA Transmission Mode, receive 1 instance of data
	HAL_ADC_Start_DMA(&hadc1, current_reading, 1);
	HAL_DAC_Start(&D1, DAC_CHANNEL_1); //Start DAC

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
	 hadc1.Init.DMAContinuousRequests = ENABLE; //Needed for DMA Configuration
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

	configureDMA(); //Configure DMA associated with ADC
	__HAL_LINKDMA(&hadc1, DMA_Handle, hdma2); //Link the ADC to the configured DMA

}

void configureDMA(){
	//Peripheral (ADC1) Using DMA is Located on DMA2

	__HAL_RCC_DMA2_CLK_ENABLE(); //DMA2 Clock Enable

	//ADC1 is located on Channel 0 Stream 0 on DMA2
	hdma2.Instance = DMA2_Stream0;
	hdma2.Init.Channel = DMA_CHANNEL_0;
	hdma2.Init.Direction = DMA_PERIPH_TO_MEMORY; //From ADC and storing in memory
	hdma2.Init.PeriphInc = DMA_PINC_DISABLE; //Don't want to increment from ADC
	hdma2.Init.MemInc = DMA_MINC_ENABLE; //Ensure Increment in memory
	//Ensure data size is large enough, in bits, to transmit the reading from ADC
	hdma2.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma2.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma2.Init.Mode = DMA_CIRCULAR; //Loop back to start when buffer is full
	hdma2.Init.Priority = DMA_PRIORITY_HIGH;
	hdma2.Init.FIFOMode = DMA_FIFOMODE_DISABLE; //Not needed
	hdma2.Init.MemBurst = DMA_MBURST_SINGLE; //One instance of data at a time
	hdma2.Init.PeriphBurst = DMA_PBURST_SINGLE; //One instance of data at a time
	HAL_DMA_Init(&hdma2); //Initialize the DMA configuration

	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn); //Enable the given stream
}

void DMA2_Stream0_IRQHandler(void){
	HAL_DMA_IRQHandler(&hdma2);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	//Current reading buffer when transmission of 1 instance of data is done

	//C - Style Floating Point Equation
	current_output = 0.3125*(double)current_reading[0] + 0.240385*last_reading
			+ 0.3125*second_last_reading + 0.296875*last_output;

/*
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
*/

	//Send Filtered Value to DAC Output
	HAL_DAC_SetValue(&D1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, current_output);

	//The current readings/output become the past ones
	second_last_reading = last_reading;
	last_reading = current_reading[0];
	last_output = current_output;
}

