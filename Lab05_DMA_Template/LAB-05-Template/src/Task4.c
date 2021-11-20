/*
 * Task4.c
 *
 *  Created on: Nov 15, 2021
 *      Author: Ray & Shayne
 */

//--------------------------------
// Lab 4 - Sample - Lab04_sample.c
//--------------------------------
// Task 4 - Take output from ADC, filter it, send output to DAC output
//

#include "init.h"

//TypeDef Structures for Needed Components
DAC_HandleTypeDef D1; //DAC HandleType
DAC_ChannelConfTypeDef D1_OUT; //DAC Channel HandleType
ADC_HandleTypeDef hadc1; //ADC HandleType
ADC_ChannelConfTypeDef sConfig; //ADC Channel HandleType
DMA_HandleTypeDef hdma1; //DMA for ADC
DMA_HandleTypeDef hdma2; //DMA for DAC
TIM_HandleTypeDef htim2; //Timer 2 Handletype
//Handletype to have timer trigger updates on overflow
TIM_MasterConfigTypeDef sMasterConfig;

//Variables to store readings/outputs
uint32_t current_reading[1000]; //Buffer to store ADC readings, 32 bit uint
uint32_t current_output[1000]; //Buffer to store DAC outputs, 32 bit uint
volatile double last_reading; //Last reading from ADC
volatile double second_last_reading; //2nd previous reading from ADC
volatile double last_output; //Store last output

//Pointers for adc and dac buffers to split them in half
uint32_t* adc_buff_ptr;
uint32_t* dac_buff_ptr;

//Coefficients for the Filter Equation
volatile double coeff1, coeff2, coeff3, coeff4;

//Prototypes
void Init_Timer(); //Timer Configurations Needed
void configureADC(); //ADC Configurations Needed
void configureDAC(); //DAC Configurations Needed
void configureDMA_ADC(); //DMA for ADC Configurations Needed
void configureDMA_DAC(); //DMA for DAC Configurations Needed
void Filter_Equation(); //Filter readings and place filtered output into DAC buffer

// Main Execution Loop
int main(void)
{
	//Initialize the system
	Sys_Init();
	Init_Timer();
	configureDAC();
	configureADC();

	//Initialize variables to 0
	last_reading = 0;
	second_last_reading = 0;
	last_output = 0;

	HAL_TIM_Base_Start(&htim2); //Start timer 2
	//Start ADC in DMA Transmission Mode, store received data in given buffer, size of 1000
	HAL_ADC_Start_DMA(&hadc1, (uint32_t* ) current_reading, 1000);
	//Start DAC in DMA Transmission Mode, store received data in given buffer, size of 1000
	//Output to given channel, 12bit and right aligned
	HAL_DAC_Start_DMA(&D1, DAC_CHANNEL_1, (uint32_t* ) current_output, 1000, DAC_ALIGN_12B_R);

	while(1){
	}
}

//TIM2 init function
void Init_Timer(){

	//Enable Clock for Timer 2
	__HAL_RCC_TIM2_CLK_ENABLE();

	//Associate the timer for the structure as Timer 2
	htim2.Instance = TIM2;

	//Configure for 500kHz Frequency
	htim2.Init.Prescaler = 23;
	htim2.Init.Period = 8;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim2);

	//
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE; //Trigger Updates on Timer
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE; //Not needed
	HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig); //Initialize configuration

}

void configureDAC()
{
	// Enable the DAC Clock.
	__HAL_RCC_DAC_CLK_ENABLE();

	D1.Instance = DAC1;
	HAL_DAC_Init(&D1); // Initialize the DAC

	// Configure the DAC channel
	D1_OUT.DAC_Trigger = DAC_TRIGGER_T2_TRGO; //Triggers based on T2 overflow
	D1_OUT.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE; //Buffer(delay) the data sent
	HAL_DAC_ConfigChannel(&D1, &D1_OUT, DAC_CHANNEL_1); //Initialize configuration

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

	configureDMA_DAC(); //Configuration for DMA for DAC
	__HAL_LINKDMA(&D1, DMA_Handle1, hdma2); //Link DAC with DMA
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
	 hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO; //ADC triggers on Timer 2 overflow
	 hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING; //On Timer 2 Rising Edge

	 HAL_ADC_Init(&hadc1); // Initialize the ADC

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

	configureDMA_ADC(); //Configure DMA for ADC
	__HAL_LINKDMA(&hadc1, DMA_Handle, hdma1); //Link DAC with DMA

}

void configureDMA_ADC(){
	//Configuration for ADC DMA
	__HAL_RCC_DMA2_CLK_ENABLE(); //Enable Clock for DMA2

	//ADC1 is stream 0 channel 0
	hdma1.Instance = DMA2_Stream0;
	hdma1.Init.Channel = DMA_CHANNEL_0;
	hdma1.Init.Direction = DMA_PERIPH_TO_MEMORY; //ADC to Memory
	hdma1.Init.PeriphInc = DMA_PINC_DISABLE; //Don't want to increment ADC
	hdma1.Init.MemInc = DMA_MINC_ENABLE; //Increment only in memory
	//Ensure readings are read correctly, can also be halfword
	hdma1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma1.Init.Mode = DMA_CIRCULAR; //Go back to first index when reaching end of buffer
	hdma1.Init.Priority = DMA_PRIORITY_HIGH;
	hdma1.Init.FIFOMode = DMA_FIFOMODE_DISABLE; //Not needed
	hdma1.Init.MemBurst = DMA_MBURST_SINGLE; //One instance of data at a time
	hdma1.Init.PeriphBurst = DMA_PBURST_SINGLE; //One instance of data at a time
	HAL_DMA_Init(&hdma1); //Initialize the DMA configuration

	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn); //Enable the given stream
}

void configureDMA_DAC(){
	//Configuration for DAC DMA
	__HAL_RCC_DMA1_CLK_ENABLE(); //Enable Clock for DMA1

	//DAC1 is stream 5 channel 7
	hdma2.Instance = DMA1_Stream5;
	hdma2.Init.Channel = DMA_CHANNEL_7;
	hdma2.Init.Direction = DMA_MEMORY_TO_PERIPH; //From memory to DAC
	hdma2.Init.PeriphInc = DMA_PINC_DISABLE; //Don't want to increment ADC
	hdma2.Init.MemInc = DMA_MINC_ENABLE; //Increment only in memory
	//Ensure outputs are read correctly, can also be halfword
	hdma2.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma2.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma2.Init.Mode = DMA_CIRCULAR; //Go back to first index when reaching end of buffer
	hdma2.Init.Priority = DMA_PRIORITY_HIGH;
	hdma2.Init.FIFOMode = DMA_FIFOMODE_DISABLE; //Not needed
	hdma2.Init.MemBurst = DMA_MBURST_SINGLE; //One instance of data at a time
	hdma2.Init.PeriphBurst = DMA_PBURST_SINGLE; //One instance of data at a time
	HAL_DMA_Init(&hdma2); //Initialize the DMA configuration

	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn); //Enable the given stream
}

void DMA2_Stream0_IRQHandler(void){
	HAL_DMA_IRQHandler(&hdma1);
}

void DMA1_Stream5_IRQHandler(void){
	HAL_DMA_IRQHandler(&hdma2);
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc){
	/*
	First Half is filled, can be processed
	Put ADC & DAC buffer pointer to the start of buffer
	Allows indices 0-499 of readings to be placed into indices 0-499 of DAC buffer
	*/
	adc_buff_ptr = &current_reading[0]; //Start of ADC buffer
	dac_buff_ptr = &current_output[0]; //Halfway into DAC buffer
	Filter_Equation(); //Filter 500 readings
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	/*
	Second Half is filled, can be processed
	Put ADC & DAC buffer pointer to the halfway point of the buffer
	Allows indices 500-999 of readings to be placed into indices 500-999 of DAC buffer
	*/
	adc_buff_ptr = &current_reading[500]; //Halfway into ADC buffer
	dac_buff_ptr = &current_output[500]; //Start of DAC buffer
	Filter_Equation(); //Filter 500 readings
}

void Filter_Equation(){
/*	Filter the readings of 500 instances, place these outputs into DAC buffer at
	the same index as the reading*/

	for(int i = 0; i < 500; i++){ //500 instances
		//Filter the reading based on where it was set, either start of halfway in
		dac_buff_ptr[i] = 0.3125*(double)adc_buff_ptr[i] + 0.240385*last_reading
				+ 0.3125*second_last_reading + 0.296875*last_output;

		//Update appropriate variables
		second_last_reading = last_reading;
		last_reading = adc_buff_ptr[i];
		last_output = dac_buff_ptr[i];
	}
}

