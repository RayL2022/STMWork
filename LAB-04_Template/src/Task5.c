/*
 * Task5.c
 *
*/

#include "init.h"

DAC_HandleTypeDef D1;
DAC_ChannelConfTypeDef D1_OUT;
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
ADC_ChannelConfTypeDef sConfig;
ADC_ChannelConfTypeDef sConfig2;

TIM_HandleTypeDef HTIM1; //Structure to store information for the Timer

volatile double get_value[2];

volatile double value1_in_volt;
volatile double value2_in_volt;

volatile double current_reading;
volatile double last_reading;
volatile double second_last_reading;
volatile double third_last_reading;
volatile double fourth_last_reading;

volatile double current_output;
volatile double last_output;
volatile double second_last_output;
volatile double third_last_output;
volatile double fourth_last_output;

volatile int flag_1;
volatile int flag_2;

void configureADC();
void configureDAC();
void Init_Timer();

int main(void)
{
	Sys_Init();
	configureDAC();
	configureADC();
    HAL_Init(); //Initialize HAL

	current_reading = 0;
	last_reading = 0;
	second_last_reading = 0;
	third_last_reading = 0;
	fourth_last_reading = 0;
	current_output = 0;

	HAL_DAC_Start(&D1, DAC_CHANNEL_1);
    while (1)
    {
		if (flag_1 == 0){
			HAL_ADC_Start_IT(&hadc1);
		}
		if (flag_2 == 0){
			HAL_ADC_Start_IT(&hadc2);
		}
    	if (flag_1 & flag_2){
        	fourth_last_reading = third_last_reading;
        	third_last_reading = second_last_reading;
        	second_last_reading = last_reading;
        	last_reading = current_reading;

        	fourth_last_output = third_last_output;
        	third_last_output = second_last_output;
        	second_last_output = last_output;
        	last_output = current_output;

			HAL_DAC_SetValue(&D1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, current_output);
			flag_1 = 0;
			flag_2 = 0;
    	}

    }

}


void configureADC()
{
	// Enable the ADC Clock.
	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_ADC3_CLK_ENABLE();
	HAL_NVIC_EnableIRQ(ADC_IRQn);

	 //Configure the global features of the ADC (Clock, Resolution, Data Alignment and number
	 //of conversion)
	 hadc1.Instance = ADC1;
	 hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
	 hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	 hadc1.Init.ScanConvMode = DISABLE;
	 hadc1.Init.ContinuousConvMode = DISABLE;
	 hadc1.Init.DiscontinuousConvMode = DISABLE;
	 hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	 hadc1.Init.NbrOfConversion = 1;
	 hadc1.Init.DMAContinuousRequests = DISABLE;
	 hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;

	 HAL_ADC_Init(&hadc1); // Initialize the ADC

	 sConfig.Channel = ADC_CHANNEL_12;
	 sConfig.Rank = ADC_REGULAR_RANK_1;
	 sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	 HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	 hadc2.Instance = ADC3;
	 hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
	 hadc2.Init.Resolution = ADC_RESOLUTION_12B;
	 hadc2.Init.ScanConvMode = DISABLE;
	 hadc2.Init.ContinuousConvMode = DISABLE;
	 hadc2.Init.DiscontinuousConvMode = DISABLE;
	 hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	 hadc2.Init.NbrOfConversion = 1;
	 hadc2.Init.DMAContinuousRequests = DISABLE;
	 hadc2.Init.EOCSelection = ADC_EOC_SEQ_CONV;

	 HAL_ADC_Init(&hadc2);

	 sConfig2.Channel = ADC_CHANNEL_8;
	 sConfig2.Rank = ADC_REGULAR_RANK_1;
	 sConfig2.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	 HAL_ADC_ConfigChannel(&hadc2, &sConfig2);

}


void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{

// GPIO init
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();

	GPIO_InitStruct.Mode      = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); //Pin C2, Arduino A2, ADC1, In 12


	GPIO_InitStruct.Pin = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct); //Pin A6, Arduino A3, ADC2, In 8



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

void ADC_IRQHandler(void) {
	HAL_ADC_IRQHandler(&hadc1);  //call the callback function
	HAL_ADC_IRQHandler(&hadc2);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if (hadc->Instance == ADC1){
		get_value[0] = HAL_ADC_GetValue(&hadc1);
		flag_1 = 1;
	}

	if (hadc->Instance == ADC3){
		get_value[1] = HAL_ADC_GetValue(&hadc2);
		flag_2 = 1;
	}
	if (flag_1 & flag_2){
		value1_in_volt = ((get_value[0] * 3.3)/4095);
		value2_in_volt = ((get_value[1] * 3.3)/4095);
		current_reading = value1_in_volt * value2_in_volt;
		current_reading = ((current_reading * 4095) / 3.3);
		if (current_reading > 4095){
			current_reading = 4095;
		}
		//current_reading = get_value[0] * get_value[1];
	    current_output = 0.001*current_reading - 0.002*second_last_reading
	    		+ 0.001*fourth_last_reading + 3.166*last_output - 4.418*second_last_output
				+ 3.028*third_last_output - 0.915*fourth_last_output;

	    //current_output = current_reading;
	}
}


