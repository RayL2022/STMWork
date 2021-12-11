/*
 * ADC.c
 *
 *  Created on: Nov 30, 2021
 *      Author: Ray & Shayne
 */

#include "ADC.h" //Include our header file

void configureADC(void)
{
	// Enable the ADC Clock.
	__HAL_RCC_ADC1_CLK_ENABLE();

	 hadc1.Instance = ADC1; //Using ADC1
	 hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
	 hadc1.Init.Resolution = ADC_RESOLUTION_12B; //Full 12-bit ADC Resolution
	 hadc1.Init.ScanConvMode = DISABLE;
	 hadc1.Init.ContinuousConvMode = ENABLE;
	 hadc1.Init.DiscontinuousConvMode = DISABLE;
	 hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT; //Read bits as right-aligned
	 hadc1.Init.NbrOfConversion = 1;
	 hadc1.Init.DMAContinuousRequests = ENABLE; //Needed for DMA Configuration
	 hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;

	 HAL_ADC_Init(&hadc1); // Initialize the ADC

	 //Configure ADC to Corresponding Channel
	 sConfig.Channel = ADC_CHANNEL_12;
	 sConfig.Rank = ADC_REGULAR_RANK_1;
	 sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	 HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{

// GPIO init
	GPIO_InitTypeDef GPIO_InitStruct; //GPIO Structure for PIN on Board
	__HAL_RCC_GPIOC_CLK_ENABLE(); //Clock for GPIO C Port
	GPIO_InitStruct.Mode      = GPIO_MODE_ANALOG; //Analog Input
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_2; //Given pin
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); //Pin C2, Arduino A2, ADC1, In 12

	configureDMA(); //Configure DMA associated with ADC
	__HAL_LINKDMA(&hadc1, DMA_Handle, hdma1); //Link the ADC to the configured DMA

}

void configureDMA(){
	//Peripheral (ADC1) Using DMA is Located on DMA2

	__HAL_RCC_DMA2_CLK_ENABLE(); //DMA2 Clock Enable

	//ADC1 is located on Channel 0 Stream 0 on DMA2
	hdma1.Instance = DMA2_Stream0;
	hdma1.Init.Channel = DMA_CHANNEL_0;
	hdma1.Init.Direction = DMA_PERIPH_TO_MEMORY; //From ADC and storing in memory
	hdma1.Init.PeriphInc = DMA_PINC_DISABLE; //Don't want to increment from ADC
	hdma1.Init.MemInc = DMA_MINC_ENABLE; //Ensure Increment in memory
	//Ensure data size is large enough, in bits, to transmit the reading from ADC, this can be half as well
	hdma1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma1.Init.Mode = DMA_CIRCULAR; //Loop back to start when buffer is full
	hdma1.Init.Priority = DMA_PRIORITY_HIGH;
	hdma1.Init.FIFOMode = DMA_FIFOMODE_DISABLE; //Not needed
	hdma1.Init.MemBurst = DMA_MBURST_SINGLE; //One instance of data at a time
	hdma1.Init.PeriphBurst = DMA_PBURST_SINGLE; //One instance of data at a time
	HAL_DMA_Init(&hdma1); //Initialize the DMA configuration

	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn); //Enable the given stream
}

//IRQ for given DMA stream, calls the appropriate callback
void DMA2_Stream0_IRQHandler(void){
	HAL_DMA_IRQHandler(&hdma1);
}

//ADC input buffer is filled
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	if ((adc_value[0] > 3060)){ //ADC values above this is considered Down State
		//Don't keep changing the state if it is being held, only if it was in a previous state was different
		if (my_current_state != DOWN){
			my_current_state = DOWN; //assign state
			HAL_UART_Transmit(&U6, (uint8_t*) &my_current_state, 1, 10); //Transmit state input to other device
		}
	}
	else if (adc_value[0] < 2800){ //ADC values below this is considered Down State
		if (my_current_state != UP){
			my_current_state = UP; //assign state
			HAL_UART_Transmit(&U6, (uint8_t*) &my_current_state, 1, 10); //Transmit input to other device
		}
	}
	else if (adc_value[0] < 3050 && adc_value[0] > 2900){ //State is neutral, otherwise
		if (my_current_state != NEUTRAL){
			my_current_state = NEUTRAL; //assign state
			HAL_UART_Transmit(&U6, (uint8_t*) &my_current_state, 1, 10); //Transmit input to other device
		}
	}
}
