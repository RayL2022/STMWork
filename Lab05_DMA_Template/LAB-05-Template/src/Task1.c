#include "init.h" // Always need init.h, otherwise nothing will work.
#include<stdint.h>
#include<stdlib.h>

DMA_HandleTypeDef hdma1;
uint32_t cycles;

// For 10 indices buffers
uint8_t data_10_8[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
uint16_t data_10_16[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
uint32_t data_10_32[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

uint8_t dma_buffer_10_8[10];
uint16_t dma_buffer_10_16[10];
uint32_t dma_buffer_10_32[10];

uint8_t c_buffer_10_8[10];
uint16_t c_buffer_10_16[10];
uint32_t c_buffer_10_32[10];


// For 100 indices buffers
uint8_t data_100_8[100];
uint16_t data_100_16[100];
uint32_t data_100_32[100];

uint8_t dma_buffer_100_8[100];
uint16_t dma_buffer_100_16[100];
uint32_t dma_buffer_100_32[100];

uint8_t c_buffer_100_8[100];
uint16_t c_buffer_100_16[100];
uint32_t c_buffer_100_32[100];


// For 1000 indices buffers
uint8_t data_1000_8[1000];
uint16_t data_1000_16[1000];
uint32_t data_1000_32[1000];

uint8_t dma_buffer_1000_8[1000];
uint16_t dma_buffer_1000_16[1000];
uint32_t dma_buffer_1000_32[1000];

uint8_t c_buffer_1000_8[1000];
uint16_t c_buffer_1000_16[1000];
uint32_t c_buffer_1000_32[1000];

void add_100_lists();
void add_1000_lists();
void c_evaluate_time_8(uint8_t* data, uint32_t size);
void c_evaluate_time_16(uint16_t* data, uint32_t size);
void c_evaluate_time_32(uint32_t* data, uint32_t size);
void dma_evaluate_time_8(uint8_t* data, uint32_t size);
void dma_evaluate_time_16(uint16_t* data, uint32_t size);
void dma_evaluate_time_32(uint32_t* data, uint32_t size);
void configureDMA();


int main(void)
{
	Sys_Init();
	configureDMA();

	add_100_lists();
	add_1000_lists();

	// Enable the DWT_CYCCNT register
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->LAR = 0xC5ACCE55;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

	c_evaluate_time_8(data_10_8, 10);
	dma_evaluate_time_8(data_10_8, 10);
	printf("\r\n");

	c_evaluate_time_8(data_100_8, 100);
	dma_evaluate_time_8(data_100_8, 100);
	printf("\r\n");

	c_evaluate_time_8(data_1000_8, 1000);
	dma_evaluate_time_8(data_1000_8, 1000);
	printf("\r\n");


	c_evaluate_time_16(data_10_16, 10);
	dma_evaluate_time_16(data_10_16, 10);
	printf("\r\n");

	c_evaluate_time_16(data_100_16, 100);
	dma_evaluate_time_16(data_100_16, 100);
	printf("\r\n");

	c_evaluate_time_16(data_1000_16, 1000);
	dma_evaluate_time_16(data_1000_16, 1000);
	printf("\r\n");


	c_evaluate_time_32(data_10_32, 10);
	dma_evaluate_time_32(data_10_32, 10);
	printf("\r\n");

	c_evaluate_time_32(data_100_32, 100);
	dma_evaluate_time_32(data_100_32, 100);
	printf("\r\n");

	c_evaluate_time_32(data_1000_32, 1000);
	dma_evaluate_time_32(data_1000_32, 1000);
	printf("\r\n");


}
void add_100_lists(){
	for (int i = 0; i < 100; i++){
		data_100_8[i] = i;
		data_100_16[i] = i;
		data_100_32[i] = i;

		dma_buffer_100_8[i] = i;
		dma_buffer_100_16[i] = i;
		dma_buffer_100_32[i] = i;

		c_buffer_100_8[i] = i;
		c_buffer_100_16[i] = i;
		c_buffer_100_32[i] = i;
	}
}

void add_1000_lists(){
	for (int i = 0; i < 1000 ; i++){
		int j = i;
		if (j > 255){
			j = 255;
		}
		data_1000_8[i] = j;
		data_1000_16[i] = j;
		data_1000_32[i] = j;

		dma_buffer_1000_8[i] = j;
		dma_buffer_1000_16[i] = j;
		dma_buffer_1000_32[i] = j;

		c_buffer_1000_8[i] = j;
		c_buffer_1000_16[i] = j;
		c_buffer_1000_32[i] = j;
	}
}

void c_evaluate_time_8(uint8_t* data, uint32_t size){
	if (size == 10){
		DWT->CYCCNT = 0; // Clear the cycle counter
		for (int i = 0; i < size; i++){
			c_buffer_10_8[i] = data[i];
		}
		cycles = DWT->CYCCNT; // Store the cycle counter
		printf("C-buffer, uint8_t, 10 elements, %lu CPU cycles\r\n",cycles);
		fflush(stdout);
	}
	else if (size == 100){
		DWT->CYCCNT = 0; // Clear the cycle counter
		for (int i = 0; i < size; i++){
			c_buffer_100_8[i] = data[i];
		}
		cycles = DWT->CYCCNT; // Store the cycle counter
		printf("C-buffer, uint8_t, 100 elements, %lu CPU cycles\r\n",cycles);
		fflush(stdout);
	}
	else if (size == 1000){
		DWT->CYCCNT = 0; // Clear the cycle counter
		for (int i = 0; i < size; i++){
			c_buffer_1000_8[i] = data[i];
		}
		cycles = DWT->CYCCNT; // Store the cycle counter
		printf("C-buffer, uint8_t, 1000 elements, %lu CPU cycles\r\n",cycles);
		fflush(stdout);
	}
}
void c_evaluate_time_16(uint16_t* data, uint32_t size){
	if (size == 10){
		DWT->CYCCNT = 0; // Clear the cycle counter
		for (int i = 0; i < size; i++){
			c_buffer_10_16[i] = data[i];
		}
		cycles = DWT->CYCCNT; // Store the cycle counter
		printf("C-buffer, uint16_t, 10 elements, %lu CPU cycles\r\n",cycles);
		fflush(stdout);
	}
	else if (size == 100){
		DWT->CYCCNT = 0; // Clear the cycle counter
		for (int i = 0; i < size; i++){
			c_buffer_100_16[i] = data[i];
		}
		cycles = DWT->CYCCNT; // Store the cycle counter
		printf("C-buffer, uint16_t, 100 elements, %lu CPU cycles\r\n",cycles);
		fflush(stdout);
	}
	else if (size == 1000){
		DWT->CYCCNT = 0; // Clear the cycle counter
		for (int i = 0; i < size; i++){
			c_buffer_1000_16[i] = data[i];
		}
		cycles = DWT->CYCCNT; // Store the cycle counter
		printf("C-buffer, uint16_t, 1000 elements, %lu CPU cycles\r\n",cycles);
		fflush(stdout);
	}
}

void c_evaluate_time_32(uint32_t* data, uint32_t size){
	if (size == 10){
		DWT->CYCCNT = 0; // Clear the cycle counter
		for (int i = 0; i < size; i++){
			c_buffer_10_32[i] = data[i];
		}
		cycles = DWT->CYCCNT; // Store the cycle counter
		printf("C-buffer, uint32_t, 10 elements, %lu CPU cycles\r\n",cycles);
		fflush(stdout);
	}
	else if (size == 100){
		DWT->CYCCNT = 0; // Clear the cycle counter
		for (int i = 0; i < size; i++){
			c_buffer_100_32[i] = data[i];
		}
		cycles = DWT->CYCCNT; // Store the cycle counter
		printf("C-buffer, uint32_t, 100 elements, %lu CPU cycles\r\n",cycles);
		fflush(stdout);
	}
	else if (size == 1000){
		DWT->CYCCNT = 0; // Clear the cycle counter
		for (int i = 0; i < size; i++){
			c_buffer_1000_32[i] = data[i];
		}
		cycles = DWT->CYCCNT; // Store the cycle counter
		printf("C-buffer, uint32_t, 1000 elements, %lu CPU cycles\r\n",cycles);
		fflush(stdout);
	}
}

void dma_evaluate_time_8(uint8_t* data, uint32_t size){
	if (size == 10){
		DWT->CYCCNT = 0; // Clear the cycle counter
		HAL_DMA_Start(&hdma1, (uint32_t)data, (uint32_t)&dma_buffer_10_8, 10);
		HAL_DMA_PollForTransfer(&hdma1, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
		cycles = DWT->CYCCNT; // Store the cycle counter
		printf("DMA, uint8_t, 10 elements, %lu CPU cycles\r\n",cycles);
		fflush(stdout);
	}
	else if (size == 100){
		DWT->CYCCNT = 0; // Clear the cycle counter
		HAL_DMA_Start(&hdma1, (uint32_t)data, (uint32_t)&dma_buffer_100_8, 100);
		HAL_DMA_PollForTransfer(&hdma1, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
		cycles = DWT->CYCCNT; // Store the cycle counter
		printf("DMA, uint8_t, 100 elements, %lu CPU cycles\r\n",cycles);
		fflush(stdout);
	}
	else if (size == 1000){
		DWT->CYCCNT = 0; // Clear the cycle counter
		HAL_DMA_Start(&hdma1, (uint32_t)data, (uint32_t)&dma_buffer_1000_8, 1000);
		HAL_DMA_PollForTransfer(&hdma1, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
		cycles = DWT->CYCCNT; // Store the cycle counter
		printf("DMA, uint8_t, 1000 elements, %lu CPU cycles\r\n",cycles);
		fflush(stdout);
	}
}

void dma_evaluate_time_16(uint16_t* data, uint32_t size){
	if (size == 10){
		DWT->CYCCNT = 0; // Clear the cycle counter
		HAL_DMA_Start(&hdma1, (uint32_t)data, (uint32_t)&dma_buffer_10_16, 10);
		HAL_DMA_PollForTransfer(&hdma1, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
		cycles = DWT->CYCCNT; // Store the cycle counter
		printf("DMA, uint16_t, 10 elements, %lu CPU cycles\r\n",cycles);
		fflush(stdout);
	}
	else if (size == 100){
		DWT->CYCCNT = 0; // Clear the cycle counter
		HAL_DMA_Start(&hdma1, (uint32_t)data, (uint32_t)&dma_buffer_100_16, 100);
		HAL_DMA_PollForTransfer(&hdma1, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
		cycles = DWT->CYCCNT; // Store the cycle counter
		printf("DMA, uint16_t, 100 elements, %lu CPU cycles\r\n",cycles);
		fflush(stdout);
	}
	else if (size == 1000){
		DWT->CYCCNT = 0; // Clear the cycle counter
		HAL_DMA_Start(&hdma1, (uint32_t)data, (uint32_t)&dma_buffer_1000_16, 1000);
		HAL_DMA_PollForTransfer(&hdma1, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
		cycles = DWT->CYCCNT; // Store the cycle counter
		printf("DMA, uint16_t, 1000 elements, %lu CPU cycles\r\n",cycles);
		fflush(stdout);
	}
}

void dma_evaluate_time_32(uint32_t* data, uint32_t size){
	if (size == 10){
		DWT->CYCCNT = 0; // Clear the cycle counter
		HAL_DMA_Start(&hdma1, (uint32_t)data, (uint32_t)&dma_buffer_10_32, 10);
		HAL_DMA_PollForTransfer(&hdma1, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
		cycles = DWT->CYCCNT; // Store the cycle counter
		printf("DMA, uint32_t, 10 elements, %lu CPU cycles\r\n",cycles);
		fflush(stdout);
	}
	else if (size == 100){
		DWT->CYCCNT = 0; // Clear the cycle counter
		HAL_DMA_Start(&hdma1, (uint32_t)data, (uint32_t)&dma_buffer_100_32, 100);
		HAL_DMA_PollForTransfer(&hdma1, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
		cycles = DWT->CYCCNT; // Store the cycle counter
		printf("DMA, uint32_t, 100 elements, %lu CPU cycles\r\n",cycles);
		fflush(stdout);
	}
	else if (size == 1000){
		DWT->CYCCNT = 0; // Clear the cycle counter
		HAL_DMA_Start(&hdma1, (uint32_t)data, (uint32_t)&dma_buffer_1000_32, 1000);
		HAL_DMA_PollForTransfer(&hdma1, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
		cycles = DWT->CYCCNT; // Store the cycle counter
		printf("DMA, uint32_t, 1000 elements, %lu CPU cycles\r\n",cycles);
		fflush(stdout);
	}
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

