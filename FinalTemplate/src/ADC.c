#include "ADC.h"
#include "ConfUart.h"
#include "Timer.h"
#include "Draw.h"

void pollADC(){
	HAL_ADC_Start_DMA(&hadc1, adc_value, 1);
	HAL_Delay(10);
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
	 hadc1.Init.ContinuousConvMode = ENABLE;
	 hadc1.Init.DiscontinuousConvMode = DISABLE;
	 hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	 hadc1.Init.NbrOfConversion = 1;
	 hadc1.Init.DMAContinuousRequests = ENABLE; //Needed for DMA Configuration
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
	GPIO_InitStruct.Mode      = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_2;
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
	//Ensure data size is large enough, in bits, to transmit the reading from ADC
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

void DMA2_Stream0_IRQHandler(void){
	HAL_DMA_IRQHandler(&hdma1);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	if ((adc_value[0] > 3060)){ //Down
		if (my_current_state != DOWN){
			my_current_state = DOWN;
			HAL_UART_Transmit(&U6, (uint8_t*) &my_current_state, 1, 10); //Transmit input to other device
		}
	}

	else if (adc_value[0] < 2800){ //Up
		if (my_current_state != UP){
			my_current_state = UP;
			HAL_UART_Transmit(&U6, (uint8_t*) &my_current_state, 1, 10); //Transmit input to other device
		}
	}
	else if (adc_value[0] < 3050 && adc_value[0] > 2900){ //State is neutral, otherwise
		if (my_current_state != NEUTRAL){
			my_current_state = NEUTRAL;
			HAL_UART_Transmit(&U6, (uint8_t*) &my_current_state, 1, 10); //Transmit input to other device
		}
	}
}
