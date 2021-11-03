//--------------------------------
// Lab 4 - Sample - Lab04_sample.c
//--------------------------------
//
//

#include "init.h"
ADC_HandleTypeDef hadc1;
ADC_ChannelConfTypeDef sConfig;  //Configre handletypes as globals
double avg = 0;
double total;
int count = 0;
double max = -100;
double min = 10000;
int i = 1;
int slow = 0;
double calc;	//variables used for additional calculations

void configureADC();
void PB_config();

// Main Execution Loop
int main(void)
{
	//Initialize the system
	Sys_Init();
	configureADC();
	PB_config();

	HAL_ADC_Start(&hadc1);	//Start ADC
	while(1){
		count=0;
		uint16_t rawValue;
		HAL_ADC_PollForConversion(&hadc1, 2000);	//Continuously Poll for conversion
		while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET){ //When the button is pushed
			if (count == 0){ //Only once per button press
				rawValue = (float) HAL_ADC_GetValue(&hadc1); //store value from ADC into variable
				calc = (rawValue*3.3)/4095;	//convert value into voltage measurement
				if (calc < 0){
					calc = 0;
				}
				if (calc > 3.3){
					calc = 3.3;		//keep value within range of 0V to 3.3V
				}
				total = (calc + total);	//running total of all measurements
				avg = total/i;	//total value divided by number of values added
				i = i + 1;
				rawValue = (float)rawValue;
				if (calc < min){
					min = calc;	//check for new min
				}
				if (calc > max){
					max = calc;	//check for new max
				}

				printf("\033[2J"); fflush(stdout);
				printf("\033[0;0HHex: %x\n\r", rawValue);
				printf("Decimal: %.5f\n\r", calc);
				printf("Max: %.5f\n\r", max);
				printf("Min: %.5f\n\r", min);
				printf("Average: %.5f\n\r", avg);	//Print all values required
				count = 1;
			}


		}

	}
}

void configureADC(){
	// Enable the ADC Clock.
	__HAL_RCC_ADC1_CLK_ENABLE();

	 /* Configure the global features of the ADC (Clock, Resolution, Data Alignment and number
	 of conversion) */
	 hadc1.Instance = ADC1;
	 hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
	 hadc1.Init.Resolution = ADC_RESOLUTION_12B;	//Full 12-bit ADC Resolution
	 hadc1.Init.ScanConvMode = DISABLE;
	 hadc1.Init.ContinuousConvMode = ENABLE;
	 hadc1.Init.DiscontinuousConvMode = DISABLE;
	 hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	 hadc1.Init.NbrOfConversion = 1;		//Single Conversion
	 hadc1.Init.DMAContinuousRequests = DISABLE;
	 hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;

	 HAL_ADC_Init(&hadc1); // Initialize the ADC

	 sConfig.Channel = ADC_CHANNEL_12; //Configure for Channel 12
	 sConfig.Rank = 1;	//Rank 1
	 sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;	//Set sampling time for 15 cycles
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

void PB_config(){
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); //A0 Blue PB
}
