#include "init.h"


ADC_HandleTypeDef hadc1;
ADC_ChannelConfTypeDef sConfig;

UART_HandleTypeDef U6; //Handle type structure for USART6

uint16_t adc_value; //ADC value
uint8_t current_state;
uint8_t input;
void configureADC();

int main(void){
	Sys_Init();
	HAL_Init();

	configureADC();

	HAL_ADC_Start(&hadc1);

	current_state = 0; //1 for Down, 2 for Up, 3 for Neutral
	input = 0;
	while(1)
	{
		HAL_ADC_PollForConversion(&hadc1, 1000); //Start conversion
		adc_value = HAL_ADC_GetValue(&hadc1); //Get the value
		HAL_Delay(10);
		//printf("Value: %d\r\n", adc_value);

		if ((adc_value > 3060) && (adc_value < 3090)){ //Down
			if (current_state != 1){
				current_state = 1;
				//printf("Value: %d\r\n", adc_value);
				printf("State: Down\r\n");
			}
		}

		else if (adc_value < 3060){ //Up
			if (current_state != 2){
				current_state = 2;
				//printf("Value: %d\r\n", adc_value);
				printf("State: UP\r\n");
			}
		}
		else{ //State is neutral, otherwise
			if (current_state != 3){
				current_state = 3;
				//printf("Value: %d\r\n", adc_value);
				printf("State: Neutral\r\n");
			}
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

void Configure_Uart(void){
	initUart(&U6, 38400, USART6); //Initialize UART with 38400 Baud and Instance USART6

	HAL_NVIC_EnableIRQ(USART1_IRQn); //Enable interrupt
	HAL_NVIC_EnableIRQ(USART6_IRQn); //Enable interrupt

	HAL_UART_Receive_IT (&USB_UART, (uint8_t*) &current_state, 1); //Trigger receiving input for USB
	HAL_UART_Receive_IT (&U6, (uint8_t*) &current_state, 1); //Trigger receiving input for U6
}


//Handler for USART1
void USART1_IRQHandler(void) {
	HAL_UART_IRQHandler(&USB_UART);
}

//Handler for USART6
void USART6_IRQHandler(void) {
	HAL_UART_IRQHandler(&U6);
}

//Callback function
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == USART1){ //Associated with USART1
		if (input != current_state){ //We received input
			input = current_state;
			if (input == '\033'){ //Input was escape
				printf("Escape\r\n");
			}
			HAL_UART_Transmit(&U6, (uint8_t*) &input, 1, 10); //Transmit input to other device
			HAL_UART_Transmit(&USB_UART, (uint8_t*) &input, 1, 10); //Transmit input to this device
		}
	}

	if (huart->Instance == USART6){ //Associated with USART6
		if (input != current_state){ //Received input
			input = current_state;
			if (input == '\033'){ //Input was escape
				printf("Escape\r\n");
			}
			HAL_UART_Transmit(&USB_UART, (uint8_t*) &input, 1, 10);  //Transmit input to device
			}
	}
}



