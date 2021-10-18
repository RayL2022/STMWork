/*
 * Task4.c
 *
 *  Created on: Oct 16, 2021
 *      Author: Shayne & Ray
 */


#include "init.h"
//
// If needed:
//#include <stdio.h>
//#include <stdlib.h>

SPI_HandleTypeDef S2;
uint8_t input;
uint8_t reg;
uint8_t spi_data;

//Function Prototypes
void write_to_reg(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t send_data);
void read_from_reg (SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t read_data);
void read_terminal_char (SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t read_data);
void read_version (SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t read_data);
void get_temp(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t read_data);
void reset_device (SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t read_data);
void change_id (SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t read_data);

/*
 * For convenience, configure the SPI handler here
 */
// See 769 Description of HAL drivers.pdf, Ch. 58.1 or stm32f7xx_hal_spi.c
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
	S2.Init.CLKPhase = SPI_PHASE_2EDGE;
	S2.Init.CLKPolarity = SPI_POLARITY_LOW;
	S2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	HAL_SPI_Init(&S2);

	//
	// Note: HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi)
	//
	// HAL_SPI_Init() will call HAL_SPI_MspInit() after reading the properties of
	// the passed SPI_HandleTypeDef. After finishing the MspInit call, it will set
	// the SPI property bits. This is how all HAL_[peripheral]_Init() functions work.
}

/*
 * This is called upon SPI initialization. It handles the configuration
 * of the GPIO pins for SPI.
 */
 // Do NOT change the name of an MspInit function; it needs to override a
 // __weak function of the same name. It does not need a prototype in the header.
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	// SPI GPIO initialization structure here
	GPIO_InitTypeDef  GPIO_InitStruct;

	if (hspi->Instance == SPI2)
	{
		// Enable GPIO Clocks
		__GPIOA_CLK_ENABLE();
		__GPIOJ_CLK_ENABLE();
		__GPIOB_CLK_ENABLE();
		__GPIOC_CLK_ENABLE();

		GPIO_InitStruct.Pin       = GPIO_PIN_12;  //D13 SCLK
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_NOPULL;
		GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI2; //Has to be 5
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		// Initialize RX Pin
		GPIO_InitStruct.Pin = GPIO_PIN_14;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); //D12 SDO

		GPIO_InitStruct.Pin = GPIO_PIN_15;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); //D11 SDI

		GPIO_InitStruct.Pin = GPIO_PIN_3;
		HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct); //J3 DEBUG

		GPIO_InitStruct.Pin = GPIO_PIN_11;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); //A11 CS

		// Enable SPI Clocking
		__SPI2_CLK_ENABLE();

	}
}

int main(void)
{
	Sys_Init();
	HAL_Init();
	configureSPI();

	input = 0; //Initialize to 0

	while(1){
		//Check for input from keyboard
		HAL_UART_Receive (&USB_UART, &input, 1, 10);
		if (input = "\033"){
			printf("\033[2J"); fflush(stdout);
			printf("Menu Selection:\r\n");
			printf("Press the Number of the Function you would like to perform with the SPI\r\n");
			printf("2) Receive any Terminal Characters in SPI\t\n");
			printf("3) Display the Firmware\r\n");
			printf("4) Get a Temperature Measurement\r\n");
			printf("5) Reset/Clear Peripheral Terminal\r\n");
			printf("6) Change device ID\r\n");

			HAL_UART_Receive (&USB_UART, &input, 1, 10);
			if (input == 2){
				read_terminal_char(&S2, reg, spi_data);
			}
			else if (input  == 3){
				read_version(&S2, reg, spi_data);
			}
			else if (input == 4){
				get_temp(&S2, reg, spi_data);
			}
			else if (input == 5){
				reset_device(&S2, reg, spi_data);
			}
			else if (input == 6){
				change_id(&S2, reg, spi_data);
			}
			else{
				printf("Invalid number/character pressed, please press <ESC> again\r\n");
			}
		}

		else if (input){ //Key was pressed that wasn't escape
			printf("\033[0;0H"); fflush(stdout); //Top of terminal
			//Display the data that was input to terminal
			HAL_UART_Transmit (&USB_UART, &input, 1, 10);

			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
			HAL_Delay(1);

			//Trying to Write to Register 5, the input character
			reg = 0x05;
			write_to_reg(&S2, reg, input);

			//Trying to Read Register 5, which should have the input character
			read_from_reg(&S2, reg, spi_data);

			input = 0; //Clear
			reg = 0;

			if (spi_data){ //We have valid spi_data
				printf("\033[12;0H"); fflush(stdout); //bottom half of terminal
				//Display data from SPI to the terminal
				HAL_UART_Transmit (&USB_UART, &spi_data, 1, 10);
				spi_data = 0; //Clear
			}
		}

	}

}

//Looks to be working
//Write to SPI on specified register, write the send_data in that register
void write_to_reg(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t send_data){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_SPI_Transmit(&hspi, &reg, 1,  1000);
	HAL_Delay(1);
	HAL_SPI_Transmit(&hspi, &send_data, 1,  1000);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_Delay(1);
}

//Needs help
//Read from SPI on specified register, store received data in read_data variable
void read_from_reg (SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t read_data){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_SPI_Transmit(&S2, &reg, 1,  100);
	HAL_Delay(1);
	HAL_SPI_Receive(&S2, &read_data, 1,  100);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_Delay(1);
}

//Option 2, Receive terminal characters from the peripheral device using SPI.
//The received characters should be printed on the bottom half of the controller’s terminal.
void read_terminal_char (SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t read_data){

}

//Option 3, Read the peripheral’s firmware version upon startup.
void read_version (SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t read_data){

}

//Option 4, Trigger a temperature measurement and retrieve the result when it is ready.
//The temperature should be printed on the right side of the terminal to avoid the transmitted
//and received terminal characters.
void get_temp(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t read_data){

}

//Option 5, Clear or reset the peripheral terminal.
void reset_device (SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t read_data){

}

//Option 6, Change the device ID of the peripheral.
void change_id (SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t read_data){

}

