/*
 * Task5.c
 *
*/

#include "init.h"

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
ADC_ChannelConfTypeDef sConfig;

int get_value;

int main(void)
{
    while (1)
    {
        // Start ADC Conversion
        HAL_ADC_Start_IT(&hadc1);

    }

}


void configureADC()
{
	// Enable the ADC Clock.
	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_ADC3_CLK_ENABLE();


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

	 hadc2.Instance = ADC2;
	 hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
	 hadc2.Init.Resolution = ADC_RESOLUTION_12B;
	 hadc2.Init.ScanConvMode = DISABLE;
	 hadc2.Init.ContinuousConvMode = ENABLE;
	 hadc2.Init.DiscontinuousConvMode = DISABLE;
	 hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	 hadc2.Init.NbrOfConversion = 1;
	 hadc2.Init.DMAContinuousRequests = DISABLE;
	 hadc2.Init.EOCSelection = ADC_EOC_SEQ_CONV;

	 HAL_ADC_Init(&hadc1); // Initialize the ADC
	 HAL_ADC_Init(&hadc2);

	 /* Configure for the selected ADC regular channel its corresponding rank in the sequence\r
	 Analog-To-Digital Conversion 406
	 and its sample time. */
	 sConfig.Channel = ADC_CHANNEL_12;
	 sConfig.Rank = ADC_REGULAR_RANK_1;
	 sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	 HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	 sConfig.Channel = ADC_CHANNEL_6;
	 HAL_ADC_ConfigChannel(&hadc2, &sConfig);

}


void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{

// GPIO init
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Mode      = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); //Pin C2, Arduino A2, ADC1, In 12

	GPIO_InitStruct.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); //Pin A6, Arduino A0, ADC2, In 6



}

void ADC_IRQHandler(void) {
	HAL_ADC_IRQHandler(&hadc1);  //call the callback function
	HAL_ADC_IRQHandler(&hadc2);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    // Read & Update The ADC Result
    get_value = HAL_ADC_GetValue(&hadc1);
    get_value = HAL_ADC_GetValue(&hadc2);
}
