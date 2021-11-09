#include "init.h" // Always need init.h, otherwise nothing will work.
#include<stdint.h>
#include<stdlib.h>

DMA_HandleTypeDef hdma1;
uint8_t data[10] = {0xff, 1, 2, 3, 4, 5, 6, 7, 8, 9};
uint8_t dma_buffer[10];
uint8_t c_buffer[10];

void configureDMA();

int main(void)
{
	Sys_Init();
	configureDMA();

	// Enable the DWT_CYCCNT register
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->LAR = 0xC5ACCE55;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

	// C-Implementation without DMA
	DWT->CYCCNT = 0; // Clear the cycle counter
	for (int i = 0; i < sizeof(data); i++){
		c_buffer[i] = data[i];
	}
	uint32_t cycles = DWT->CYCCNT; // Store the cycle counter

	DWT->CYCCNT = 0; // Clear the cycle counter
	printf("Copying data with C-implementation took %lu CPU cycles\r\n",cycles);
	fflush(stdout);
	cycles = DWT->CYCCNT; // Store the cycle counter

	printf("And printing the above took %lu CPU cycles\r\n",cycles);
	fflush(stdout);



	DWT->CYCCNT = 0; // Clear the cycle counter
	HAL_DMA_Start(&hdma1, (uint32_t)&data, (uint32_t)&dma_buffer, 10);
	HAL_DMA_PollForTransfer(&hdma1, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
	cycles = DWT->CYCCNT;

	DWT->CYCCNT = 0; // Clear the cycle counter
	printf("Copying data with DMA took %lu CPU cycles\r\n",cycles);
	fflush(stdout);
	cycles = DWT->CYCCNT; // Store the cycle counter

	printf("And printing the above took %lu CPU cycles\r\n",cycles);
	fflush(stdout);

}


void configureDMA(){

	__HAL_RCC_DMA2_CLK_ENABLE();

	hdma1.Instance = DMA2_Stream0;
	hdma1.Init.Channel = DMA_CHANNEL_1;
	hdma1.Init.Direction = DMA_MEMORY_TO_MEMORY;
	hdma1.Init.PeriphInc = DMA_PINC_ENABLE;
	hdma1.Init.MemInc = DMA_MINC_ENABLE;
	hdma1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma1.Init.Mode = DMA_NORMAL;
	hdma1.Init.Priority = DMA_PRIORITY_HIGH;
	hdma1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma1.Init.MemBurst = DMA_MBURST_SINGLE;
	//hdma1.XferCpltCallback = &DMATransferComplete;
	HAL_DMA_Init(&hdma1);

	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

//Transferring only the first element multiple times, fix this to increment past the initial buffer

