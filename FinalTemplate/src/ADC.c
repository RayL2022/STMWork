#include "ADC.h"
#include "ConfUart.h"

void pollADC(){
	HAL_ADC_PollForConversion(&hadc1, 1000); //Start conversion
	adc_value = HAL_ADC_GetValue(&hadc1); //Get the value
	HAL_Delay(10);
	//printf("Value: %d\r\n", adc_value);

	if ((adc_value > 3060) && (adc_value < 3085)){ //Down
		if (my_current_state != '1'){
			my_current_state = '1';
			HAL_UART_Transmit(&U6, (uint8_t*) &my_current_state, 1, 10); //Transmit input to other device
			//printf("Value: %d\r\n", adc_value);
			printf("State: Down\r\n");
		}
	}

	else if (adc_value < 3060){ //Up
		if (my_current_state != '2'){
			my_current_state = '2';
			HAL_UART_Transmit(&U6, (uint8_t*) &my_current_state, 1, 10); //Transmit input to other device
			//printf("Value: %d\r\n", adc_value);
			printf("State: UP\r\n");
		}
	}
	else if (adc_value > 3110){ //State is neutral, otherwise
		if (my_current_state != '3'){
			my_current_state = '3';
			HAL_UART_Transmit(&U6, (uint8_t*) &my_current_state, 1, 10); //Transmit input to other device
			//printf("Value: %d\r\n", adc_value);
			printf("State: Neutral\r\n");
		}
	}
}


void configureADC()
{
	// Enable the ADC Clock.
	__HAL_RCC_ADC1_CLK_ENABLE();


	 /* Configure the global features of the ADC (Clock, Resolution, Data Alignment and number
	 of conversion) */
	 hadc1.Instance = ADC1; //Using ADC1
	 hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
	 hadc1.Init.Resolution = ADC_RESOLUTION_12B; //12 Bit resolution
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
	GPIO_InitStruct.Mode      = GPIO_MODE_ANALOG; //Analog mode for pin
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); //Pin C2, Arduino A2, ADC1, In 12

}
