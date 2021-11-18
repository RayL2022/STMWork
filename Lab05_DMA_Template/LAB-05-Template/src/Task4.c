/*
 * Task4.c
 *
 *  Created on: Nov 15, 2021
 *      Author: Ray
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
DMA_HandleTypeDef hdma1; //ADC
DMA_HandleTypeDef hdma2; //DAC
TIM_HandleTypeDef htim2; //Timer 2
TIM_MasterConfigTypeDef sMasterConfig; //DMA Timer

//Variables to store readings/outputs
uint32_t current_reading[1000];
uint32_t current_output[1000];
double last_reading;
double second_last_reading;
double last_output;

static volatile uint32_t* adc_buff_ptr;
static volatile uint32_t* dac_buff_ptr;

//Coefficients for the Filter Equation
double coeff1, coeff2, coeff3, coeff4;

void Init_Timer();
void configureADC();
void configureDAC();
void configureDMA_ADC();
void configureDMA_DAC();
void Filter_Equation();

// Main Execution Loop
int main(void)
{
	//Initialize the system
	Sys_Init();
	Init_Timer();
	configureDAC();
	configureADC();

	last_reading = 0;
	second_last_reading = 0;
	last_output = 0;

	HAL_ADC_Start_DMA(&hadc1, (uint32_t* ) current_reading, 1000);
	HAL_DAC_Start_DMA(&D1, DAC_CHANNEL_1, (uint32_t* ) current_output, 1000, DAC_ALIGN_12B_R);
	HAL_TIM_Base_Start_IT(&htim2); //Start timer 2 in Interrupt Mode
	// Code goes here

	while(1){
		Filter_Equation();
	}
}

//TIM2 init function
void Init_Timer(){

	//Enable Clock for Timer 2
	__HAL_RCC_TIM2_CLK_ENABLE();

	//Associate the timer for the structure as Timer 2
	htim2.Instance = TIM2;

	//Enable interrupts on NVIC
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	//Configure for 0.5MHz Frequency
	htim2.Init.Prescaler = 1;
	htim2.Init.Period = 1200;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim2);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

}

void configureDAC()
{
	// Enable the DAC Clock.
	__HAL_RCC_DAC_CLK_ENABLE();

	D1.Instance = DAC1;
	HAL_DAC_Init(&D1); // Initialize the DAC

	// Configure the DAC channel
	D1_OUT.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
	D1_OUT.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
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

	configureDMA_DAC();
	__HAL_LINKDMA(&D1, DMA_Handle1, hdma2);
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
	 hadc1.Init.ContinuousConvMode = DISABLE;
	 hadc1.Init.DiscontinuousConvMode = DISABLE;
	 hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	 hadc1.Init.NbrOfConversion = 1;
	 hadc1.Init.DMAContinuousRequests = ENABLE;
	 hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
	 hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
	 hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;

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

	configureDMA_ADC();
	__HAL_LINKDMA(&hadc1, DMA_Handle, hdma1);

}

void configureDMA_ADC(){

	__HAL_RCC_DMA2_CLK_ENABLE();

	hdma1.Instance = DMA2_Stream0;
	hdma1.Init.Channel = DMA_CHANNEL_0;
	hdma1.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma1.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma1.Init.MemInc = DMA_MINC_ENABLE;
	hdma1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma1.Init.Mode = DMA_CIRCULAR;
	hdma1.Init.Priority = DMA_PRIORITY_HIGH;
	hdma1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma1.Init.MemBurst = DMA_MBURST_SINGLE;
	hdma1.Init.PeriphBurst = DMA_PBURST_SINGLE;
	HAL_DMA_Init(&hdma1);

	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

void configureDMA_DAC(){
	__HAL_RCC_DMA1_CLK_ENABLE();

	hdma2.Instance = DMA1_Stream5;
	hdma2.Init.Channel = DMA_CHANNEL_7;
	hdma2.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma2.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma2.Init.MemInc = DMA_MINC_ENABLE;
	hdma2.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma2.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma2.Init.Mode = DMA_CIRCULAR;
	hdma2.Init.Priority = DMA_PRIORITY_HIGH;
	hdma2.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma2.Init.MemBurst = DMA_MBURST_SINGLE;
	hdma2.Init.PeriphBurst = DMA_PBURST_SINGLE;
	HAL_DMA_Init(&hdma2);

	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}

void DMA2_Stream0_IRQHandler(void){
	HAL_DMA_IRQHandler(&hdma1);
}

void DMA1_Stream5_IRQHandler(void){
	HAL_DMA_IRQHandler(&hdma2);
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc){
	//First Half is filled, can be processed
	adc_buff_ptr = &current_reading[0];
	dac_buff_ptr = &current_output[500];
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	//Second Half is filled, can be processed
	adc_buff_ptr = &current_reading[500];
	dac_buff_ptr = &current_output[0];
}

void Filter_Equation(){
	for(int i = 0; i < 500; i++){
		dac_buff_ptr[i] = 0.3125*(double)adc_buff_ptr[i] + 0.240385*last_reading
				+ 0.3125*second_last_reading + 0.296875*last_output;

		second_last_reading = last_reading;
		last_reading = adc_buff_ptr[i];
		last_output = dac_buff_ptr[i];
	}
}

void HAL_DAC_ConvCpltCallbackCh1 (DAC_HandleTypeDef *hdac){

}

//HAL - GPIO/EXTI Handler
void TIM2_IRQHandler(TIM_HandleTypeDef *htim) {
	HAL_TIM_IRQHandler(&htim2); //Pass a reference to the structure for which Timer
}

//HAL - Callback Function
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == TIM2){ //Test if the Timer Interrupt was trigger by Timer 7
	}
}

//These functions need to be included in name but don't need to be implemented
void HAL_TIMEx_BreakCallback(TIM_HandleTypeDef *htim){};
void HAL_TIMEx_CommutationCallback(TIM_HandleTypeDef *htim){};


//Modify period elapsed callback
