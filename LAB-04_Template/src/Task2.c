//--------------------------------
// Lab 4 - Sample - Lab04_sample.c
//--------------------------------
//
//

#include "init.h"

DAC_HandleTypeDef D1;
DAC_ChannelConfTypeDef D1_OUT;
ADC_HandleTypeDef hadc1;
ADC_ChannelConfTypeDef sConfig;
double avg = 0;
double total;
int count = 0;
double max = -100;
double min = 10000;
int i = 1;
int slow = 0;
double calc;
double dac_value;
uint16_t rawValue;

void configureADC();
void PB_config();
void configureDAC();

// Main Execution Loop
int main(void)
{
	//Initialize the system
	Sys_Init();
	configureDAC();
	configureADC();
	PB_config();

	dac_value = 0;
	//HAL_ADC_Start(&hadc1);
	//HAL_DAC_Start(&D1, DAC_CHANNEL_1);
	// Code goes here
	while(1){
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 1000);
		rawValue = HAL_ADC_GetValue(&hadc1);
		HAL_DAC_Start(&D1, DAC_CHANNEL_1);
		HAL_DAC_SetValue(&D1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, rawValue);
	}
/*


		//Part 1 - SawTooth
		HAL_DAC_SetValue(&D1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_value);
		if (dac_value < 4095){
			dac_value++;
		}
		else{
			dac_value = 0;
		}


		count=0;
		//char msg[20];
		HAL_ADC_PollForConversion(&hadc1, 1000);
		//while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET){
			if (count == 0){
				rawValue = HAL_ADC_GetValue(&hadc1);

				dac_value = rawValue;
				//dac_value = (calc*4096)/3.3;
				HAL_DAC_SetValue(&D1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_value);

				calc = (rawValue*3.3)/4095;
				if (calc < 0){
					calc = 0;
				}
				if (calc > 3.3){
					calc = 3.3;
				}
				total = (calc + total);
				avg = total/i;
				i = i + 1;
				rawValue = (float)rawValue;
				if (calc < min){
					min = calc;
				}
				if (calc > max){
					max = calc;
				}

				printf("\033[2J"); fflush(stdout);
				printf("\033[0;0HHex: %x\n\r", rawValue);
				printf("Decimal: %.5f\n\r", calc);
				printf("Max: %.5f\n\r", max);
				printf("Min: %.5f\n\r", min);
				printf("Average: %.5f\n\r", avg);
				count = 1;
			}

			//Part 2 - Other Signals
			dac_value = rawValue;
			//dac_value = (calc*4096)/3.3;
			HAL_DAC_SetValue(&D1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_value);


			if (dac_value > 4095){
				dac_value = 4095;
			}
			if (dac_value < 0){
				dac_value = 0;
			}

			printf("dac: %f\r\n",dac_value);

			//HAL_Delay(1);
		//}
		HAL_DAC_SetValue(&D1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_value);
		//dac_value = rawValue;

		printf("dac: %f\r\n",dac_value);

		//Up part
		while(dac_value < 4095){
			HAL_DAC_SetValue(&D1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_value);
			dac_value++;
		}
		//Bottom part
		while(dac_value > 1){
			HAL_DAC_SetValue(&D1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_value);
			dac_value--;
		}

	}*/
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
	 sConfig.Channel = ADC_CHANNEL_12;
	 sConfig.Rank = ADC_REGULAR_RANK_1;
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

