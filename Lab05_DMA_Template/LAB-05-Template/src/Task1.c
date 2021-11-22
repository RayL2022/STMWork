/*
 * Task1.c
 *
 *  Created on: Nov 1, 2021
 *      Author: Ray & Shayne
 */
// Task 1 - Compare efficiency of copying data in memory with and without using dma
// Copying should be done using multiple variable sizes and array sizes

//Includes for operation
#include "init.h"
#include<stdint.h>
#include<stdlib.h>

DMA_HandleTypeDef hdma1; //HandleType for DMA

uint32_t cycles; //Stores amount of cycles between two points in operation

//Size 10 data buffers of variable types 8, 16, and 32 bit uints
uint8_t data_10_8[10];
uint16_t data_10_16[10];
uint32_t data_10_32[10];

//Size 10 dma buffers of variable types 8, 16, and 32 bit uints
uint8_t dma_buffer_10_8[10];
uint16_t dma_buffer_10_16[10];
uint32_t dma_buffer_10_32[10];

//Size 10 copy buffers of variable types 8, 16, and 32 bit uints
uint8_t c_buffer_10_8[10];
uint16_t c_buffer_10_16[10];
uint32_t c_buffer_10_32[10];


//Size 100 data buffers of variable types 8, 16, and 32 bit uints
uint8_t data_100_8[100];
uint16_t data_100_16[100];
uint32_t data_100_32[100];

//Size 100 dma buffers of variable types 8, 16, and 32 bit uints
uint8_t dma_buffer_100_8[100];
uint16_t dma_buffer_100_16[100];
uint32_t dma_buffer_100_32[100];

//Size 100 copy buffers of variable types 8, 16, and 32 bit uints
uint8_t c_buffer_100_8[100];
uint16_t c_buffer_100_16[100];
uint32_t c_buffer_100_32[100];


//Size 1000 data buffers of variable types 8, 16, and 32 bit uints
uint8_t data_1000_8[1000];
uint16_t data_1000_16[1000];
uint32_t data_1000_32[1000];

//Size 1000 dma buffers of variable types 8, 16, and 32 bit uints
uint8_t dma_buffer_1000_8[1000];
uint16_t dma_buffer_1000_16[1000];
uint32_t dma_buffer_1000_32[1000];

//Size 1000 copy buffers of variable types 8, 16, and 32 bit uints
uint8_t c_buffer_1000_8[1000];
uint16_t c_buffer_1000_16[1000];
uint32_t c_buffer_1000_32[1000];

//Prototypes
void add_10_lists(); //Fill the data buffers of size 10
void add_100_lists(); //Fill the data buffers of size 100
void add_1000_lists(); //Fill the data buffers of size 1000

//Evaluate copying data time without using dma, various sizes
void c_evaluate_time_8(uint8_t* data, uint32_t size);
void c_evaluate_time_16(uint16_t* data, uint32_t size);
void c_evaluate_time_32(uint32_t* data, uint32_t size);

//Evaluate copying data time with using dma, various sizes
void dma_evaluate_time_8(uint8_t* data, uint32_t size);
void dma_evaluate_time_16(uint16_t* data, uint32_t size);
void dma_evaluate_time_32(uint32_t* data, uint32_t size);

//Configuration for DMA
void configureDMA();


int main(void)
{
	Sys_Init();
	configureDMA();

	//Initialize data buffers of all sizes
	add_10_lists();
	add_100_lists();
	add_1000_lists();

	// Enable the DWT_CYCCNT register
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->LAR = 0xC5ACCE55;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

	//Compute amount of time for copying data of size 10, 8 bit
	c_evaluate_time_8(data_10_8, 10);
	dma_evaluate_time_8(data_10_8, 10);
	printf("\r\n");

	//Compute amount of time for copying data of size 100, 8 bit
	c_evaluate_time_8(data_100_8, 100);
	dma_evaluate_time_8(data_100_8, 100);
	printf("\r\n");

	//Compute amount of time for copying data of size 1000, 8 bit
	c_evaluate_time_8(data_1000_8, 1000);
	dma_evaluate_time_8(data_1000_8, 1000);
	printf("\r\n");


	//Compute amount of time for copying data of size 10, 16 bit
	c_evaluate_time_16(data_10_16, 10);
	dma_evaluate_time_16(data_10_16, 10);
	printf("\r\n");

	//Compute amount of time for copying data of size 100, 16 bit
	c_evaluate_time_16(data_100_16, 100);
	dma_evaluate_time_16(data_100_16, 100);
	printf("\r\n");

	//Compute amount of time for copying data of size 1000, 16 bit
	c_evaluate_time_16(data_1000_16, 1000);
	dma_evaluate_time_16(data_1000_16, 1000);
	printf("\r\n");


	//Compute amount of time for copying data of size 10, 32 bit
	c_evaluate_time_32(data_10_32, 10);
	dma_evaluate_time_32(data_10_32, 10);
	printf("\r\n");

	//Compute amount of time for copying data of size 100, 32 bit
	c_evaluate_time_32(data_100_32, 100);
	dma_evaluate_time_32(data_100_32, 100);
	printf("\r\n");

	//Compute amount of time for copying data of size 1000, 32 bit
	c_evaluate_time_32(data_1000_32, 1000);
	dma_evaluate_time_32(data_1000_32, 1000);
	printf("\r\n");


}

void add_10_lists(){
	//Adds values 0-9 into array of size 10
	for (int i = 0; i < 10; i++){
		data_10_8[i] = i;
		data_10_16[i] = i;
		data_10_32[i] = i;
	}
}

void add_100_lists(){
	//Adds values 0-99 into array of size 100
	for (int i = 0; i < 100; i++){
		data_100_8[i] = i;
		data_100_16[i] = i;
		data_100_32[i] = i;
	}
}

void add_1000_lists(){
	//Adds values 0-255 into array of size 1000
	//When 255 is reached, continue placing that value of 255
	for (int i = 0; i < 1000 ; i++){
		int j = i;
		if (j > 255){
			j = 255;
		}
		data_1000_8[i] = j;
		data_1000_16[i] = j;
		data_1000_32[i] = j;
	}
}

void c_evaluate_time_8(uint8_t* data, uint32_t size){
	if (size == 10){
		DWT->CYCCNT = 0; // Clear the cycle counter
		//Copy from data buffer and store in copy
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
	//Configure to 8 bit sending/receiving of data
	hdma1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	HAL_DMA_Init(&hdma1);

	if (size == 10){
		DWT->CYCCNT = 0; // Clear the cycle counter
		//Start Transmission of data from data buffer to dma buffer, length of 10
		HAL_DMA_Start(&hdma1, (uint32_t)data, (uint32_t)&dma_buffer_10_8, 10);
		//Wait for the complete transfer to complete for transmission
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
	//Configure to 16 bit sending/receiving of data
	hdma1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	HAL_DMA_Init(&hdma1);

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
	//Configure to 32 bit sending/receiving of data
	hdma1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	HAL_DMA_Init(&hdma1);

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
	//Memory to Memory requires the use of DMA2

	__HAL_RCC_DMA2_CLK_ENABLE(); //Clock for DMA2

	//Not using a specified peripheral, configure unused channel and stream on DMA2
	hdma1.Instance = DMA2_Stream0;
	hdma1.Init.Channel = DMA_CHANNEL_1;
	hdma1.Init.Direction = DMA_MEMORY_TO_MEMORY; //Moving data from memory location to another
	hdma1.Init.PeriphInc = DMA_PINC_ENABLE; //Ensure Increment moves while copying
	hdma1.Init.MemInc = DMA_MINC_ENABLE; //Ensure Increment moves while copying
	hdma1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; //Sending 8bit
	hdma1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE; //Sending 8bit
	hdma1.Init.Mode = DMA_NORMAL; //Normal Buffer Mode
	hdma1.Init.Priority = DMA_PRIORITY_HIGH;
	hdma1.Init.FIFOMode = DMA_FIFOMODE_DISABLE; //Not needed
	hdma1.Init.MemBurst = DMA_MBURST_SINGLE; //Transmission one at a time

	HAL_DMA_Init(&hdma1); //Associate configurations with handletype

	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn); //Enable the configured stream
}


