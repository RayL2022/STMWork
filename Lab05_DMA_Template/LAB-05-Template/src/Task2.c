/*
 * Task2.c
 *
 *  Created on: Nov 1, 2021
 *      Author: Shayne & Ray
 */
// Task 2 - Reconfigure Lab 03 Task 2 to send and receive data using DMA
// Allows storage of characters until enter is pressed and then all characters
// are transmitted at once

#include "init.h"
#include<stdint.h>
#include<stdlib.h>

SPI_HandleTypeDef S2; //Handle type structure for SPI2
DMA_HandleTypeDef hdma2;
DMA_HandleTypeDef hdma3;

char input; //Input data from keyboard
int i = 0;
int check;
char display = 0;
uint8_t spi_data[100]; //Storage for SPI data
uint8_t data[100];	//Storage for user inputs

void configureDMA();

//Necessary Configurations for SPI
void configureSPI()
{
	S2.Instance = SPI2; // Please use SPI2!
	S2.Init.Mode = SPI_MODE_MASTER; // Set master mode
	S2.Init.TIMode = SPI_TIMODE_DISABLE; // Use Motorola mode, not TI mode
	S2.Init.Direction = SPI_DIRECTION_2LINES; //2 separate lines, transmit and receive at same time
	S2.Init.DataSize = SPI_DATASIZE_8BIT; //transfer/receive 8 bits at a time
	S2.Init.NSS = SPI_NSS_SOFT; //Using software to select peripheral
	S2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128; //~0.84 MHz ~ 1 Mhz given noise
	S2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE; //Disable CRC
	HAL_SPI_Init(&S2);
}

//This is called upon SPI initialization. It handles the configuration
//of the GPIO pins for SPI.
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	// SPI GPIO initialization structure here
	GPIO_InitTypeDef  GPIO_InitStruct;
	if (hspi->Instance == SPI2) //Associated with SPI2
	{
		// Enable GPIO Clocks
		__GPIOA_CLK_ENABLE();
		__GPIOJ_CLK_ENABLE();
		__GPIOB_CLK_ENABLE();
		__GPIOC_CLK_ENABLE();
		GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull      = GPIO_NOPULL;
		GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Pin = GPIO_PIN_11;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); //A11 CS

		GPIO_InitStruct.Pin       = GPIO_PIN_12;  //D13 SCLK
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI2; //Has to be 5
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_14;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); //D12 SDO

		GPIO_InitStruct.Pin = GPIO_PIN_15;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); //D11 SDI

		GPIO_InitStruct.Pin = GPIO_PIN_3;
		HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct); //J3 DEBUG

		// Enable SPI Clocking
		__SPI2_CLK_ENABLE();

	}
}

int main(void)
{
	Sys_Init();
	HAL_Init();

	configureSPI();
	configureDMA();

	input = 0; //Initialize to 0
	check = 1;

	while(1){
		HAL_UART_Receive (&USB_UART, (uint8_t*) &input, 1, HAL_MAX_DELAY);	//Try to receive input
		if (input){ //Key was pressed
			if (check == 1){
				printf("\033[0;0H"); fflush(stdout); //Top of terminal
				printf("\033[2K"); fflush(stdout);
				check = 0;
			}
			data[i] = input;	//append inputs to array
			i = i + 1;
			HAL_UART_Transmit (&USB_UART, (uint8_t*) &input, 1, 10); //Display the data that was input to terminal
			if (input == '\r'){
				//Transmit data from "data", transmit (store) in spi_data
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
				HAL_Delay(1);//Delay between pin state change
				HAL_SPI_TransmitReceive_DMA(&S2, (uint8_t*) &data, (uint8_t*) &spi_data, 100);
				HAL_Delay(1);//Delay between pin state change
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
				display = 1;
			}
			input = 0; //Clear input

		}

		if (display){ //We have valid spi_data
			printf("\033[12;0H"); fflush(stdout); //bottom half of terminal
			printf("\033[2K"); fflush(stdout);
			//Display data from SPI to the terminal
			HAL_UART_Transmit (&USB_UART, (uint8_t*) &spi_data, 100, 10);
			display = 0;
			memset(data, '\0', sizeof(spi_data));
			memset(spi_data, '\0', sizeof(spi_data));	//clear data arrays
			check = 1;	//set to return printing to top of screen
			i = 0; 		//clear variable
		}
	}

}

void configureDMA(){

	__HAL_RCC_DMA1_CLK_ENABLE();	//Enable DMA1 clock

	hdma2.Instance = DMA1_Stream3;
	hdma2.Init.Channel = DMA_CHANNEL_0;	//Use Channel 0, Stream 3
	hdma2.Init.Direction = DMA_PERIPH_TO_MEMORY;	//Transfer data from peripheral to memory
	hdma2.Init.PeriphInc = DMA_PINC_DISABLE;	//Don't increment peripheral address
	hdma2.Init.MemInc = DMA_MINC_ENABLE;	//Increment memory address
	hdma2.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;	//Peripheral data width one byte
	hdma2.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;	//Memory data width one byte
	hdma2.Init.Mode = DMA_NORMAL;	//Use normal transfer type
	hdma2.Init.Priority = DMA_PRIORITY_HIGH;	//Sets the stream's priority to high for the software
	hdma2.Init.FIFOMode = DMA_FIFOMODE_DISABLE;	//Disable FIFO buffers
	hdma2.Init.MemBurst = DMA_MBURST_SINGLE;	//Single piece of data transfered in a transaction
	hdma2.Init.PeriphBurst = DMA_PBURST_SINGLE;	//Single piece of data transfered in a transaction
	HAL_DMA_Init(&hdma2);	//Initialize DMA configurations
	__HAL_LINKDMA(&S2,hdmarx,hdma2);	//Set this DMA stream in receive mode
	HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);	//Enable interrupt for stream 3

	hdma3.Instance = DMA1_Stream4;
	hdma3.Init.Channel = DMA_CHANNEL_0;	//Use Channel 0, Stream 4
	hdma3.Init.Direction = DMA_MEMORY_TO_PERIPH;	//Transfer data from memory to peripheral
	hdma3.Init.PeriphInc = DMA_PINC_DISABLE;	//Don't increment peripheral address
	hdma3.Init.MemInc = DMA_MINC_ENABLE;	//Increment memory address
	hdma3.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;	//Peripheral data width one byte
	hdma3.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;	//Memory data width one byte
	hdma3.Init.Mode = DMA_NORMAL;	//Use normal transfer type
	hdma3.Init.Priority = DMA_PRIORITY_HIGH;	//Sets the stream's priority to high for the software
	hdma3.Init.FIFOMode = DMA_FIFOMODE_DISABLE;	//Disable FIFO buffers
	hdma3.Init.MemBurst = DMA_MBURST_SINGLE;	//Single piece of data transfered in a transaction
	hdma3.Init.PeriphBurst = DMA_PBURST_SINGLE;	//Single piece of data transfered in a transaction
	HAL_DMA_Init(&hdma3);	//Initialize DMA configurations
	__HAL_LINKDMA(&S2,hdmatx,hdma3);	//Set this DMA stream in transmit mode
	HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn); //Enable interrupt for stream 4
}

void DMA1_Stream3_IRQHandler(void){
	HAL_DMA_IRQHandler(&hdma2);
}

void DMA1_Stream4_IRQHandler(void){
	HAL_DMA_IRQHandler(&hdma3);
}

void HAL_SPI_TxRxCpltCallback (SPI_HandleTypeDef * hspi){

}

