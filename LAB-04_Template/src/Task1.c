//--------------------------------
// Lab 4 - Sample - Lab04_sample.c
//--------------------------------
//
//

#include "init.h"
ADC_HandleTypeDef hadc1;
int average = 0;
int count = 0;
int max = 0;
int min = 0;

void configureADC();
void PB_config();

// Main Execution Loop
int main(void)
{
	//Initialize the system
	Sys_Init();
	configureADC();
	PB_config();

	// Code goes here
	HAL_ADC_Start(&hadc1);
	while(1){
		char msg[20];
		uint16_t rawValue;
		float temp;
		HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
		rawValue = HAL_ADC_GetValue(&hadc1);
		sprintf(msg, "rawValue: %hu\r\n", rawValue);
		HAL_UART_Transmit(&USB_UART, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);

	}
}

void configureADC()
{
	// Enable the ADC Clock.
	 __HAL_RCC_ADC1_CLK_ENABLE();
	 ADC_ChannelConfTypeDef sConfig;


	 /* Configure the global features of the ADC (Clock, Resolution, Data Alignment and number
	 of conversion) */
	 hadc1.Instance = ADC1;
	 hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	 hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	 hadc1.Init.ScanConvMode = DISABLE;
	 hadc1.Init.ContinuousConvMode = ENABLE;
	 hadc1.Init.DiscontinuousConvMode = DISABLE;
	 hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	 hadc1.Init.NbrOfConversion = 1;
	 hadc1.Init.DMAContinuousRequests = DISABLE;
	 hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;

	 HAL_ADC_Init(&hadc1); // Initialize the ADC

//		ADC_SAMPLETIME_3CYCLES
//		ADC_SAMPLETIME_15CYCLES
//		ADC_SAMPLETIME_28CYCLES
//		ADC_SAMPLETIME_56CYCLES
//		ADC_SAMPLETIME_84CYCLES
//		ADC_SAMPLETIME_112CYCLES
//		ADC_SAMPLETIME_144CYCLES
//		ADC_SAMPLETIME_480CYCLES

	 /* Configure for the selected ADC regular channel its corresponding rank in the sequence\r
	 Analog-To-Digital Conversion 406
	 and its sample time. */
	 sConfig.Channel = ADC_CHANNEL_14;
	 sConfig.Rank = 1;
	 sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
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
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); //Pin C2, Arduino A2, ADC1, In 12

}

void PB_config(){
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); //A0 Blue PB
}
