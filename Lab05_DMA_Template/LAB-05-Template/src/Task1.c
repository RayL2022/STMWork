#include "init.h" // Always need init.h, otherwise nothing will work.
#include<stdint.h>
#include<stdlib.h>

DMA_HandleTypeDef hdma1;
uint8_t buf1[20];
uint8_t buf2[20];

void configureDMA();

int main(void)
{
	Sys_Init();

	// Enable the DWT_CYCCNT register
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->LAR = 0xC5ACCE55;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;


	DWT->CYCCNT = 0; // Clear the cycle counter
	printf("\033[0m\033[44;33m\033[2J\033[;H"); // Erase screen & move cursor to home position
	fflush(stdout);
	uint32_t cycles = DWT->CYCCNT; // Store the cycle counter

	DWT->CYCCNT = 0; // Clear the cycle counter
	printf("Clearing and Moving the Cursor to Home took %lu CPU cycles\r\n",cycles);
	fflush(stdout);
	cycles = DWT->CYCCNT; // Store the cycle counter

	printf("And printing the above took %lu CPU cycles\r\n",cycles);
	fflush(stdout);

	while(1);
}


void configureDMA(){
	hdma1.Instance = DMA1_Stream0;	//This needs to change
	hdma1.Init.Direction = DMA_MEMORY_TO_MEMORY;
	hdma1.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma1.Init.MemInc = DMA_MINC_ENABLE;
	hdma1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma1.Init.Mode = DMA_NORMAL;
	hdma1.Init.Priority = DMA_PRIORITY_LOW;
	//hdma1.XferCpltCallback = &DMATransferComplete;
	HAL_DMA_Init(&hdma1);

	HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
}


