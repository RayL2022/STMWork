/*
 * Task4.c
 *
 *  Created on: Oct 16, 2021
 *      Author: Shayne & Ray
 */
//The purpose of this task is to communicate with peripheral device
//and be able to effectively do requirements outlined in document with said device

#include "init.h"
//
// If needed:
//#include <stdio.h>
//#include <stdlib.h>

SPI_HandleTypeDef S2; //Handle type structure for SPI2
uint8_t input; //Input data
uint8_t reg; //Storage for starting register
uint8_t reg2; //Storage for starting register
uint8_t spi_data; //Storage for data to interact with SPI
uint8_t spi_data2; //Storage for data to interact with SPI
uint8_t unlock; //Unlock key for DEVID writing
uint8_t temphi; //High byte of Temp
uint8_t templo; //Low byte of Temp
uint16_t temp_complete; //Combined Temp
double final_temp; //Converted Temp

//Function Prototypes
void write_to_reg(uint8_t reg, uint8_t send_data);
void read_from_reg (uint8_t reg, uint8_t* read_data);
void read_terminal_char (uint8_t reg, uint8_t size, uint8_t read_data);
void read_version (uint8_t reg1, uint8_t reg2, uint8_t* read_data1, uint8_t* read_data2);
void get_temp(uint8_t reg1, uint8_t reg2, uint8_t* read_data1, uint8_t* read_data2);
void reset_device (uint8_t reg, uint8_t send_data);
void change_id (uint8_t reg1, uint8_t reg2, uint8_t read_data, uint8_t key, uint8_t* new_name);

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
	S2.Init.CLKPhase = SPI_PHASE_2EDGE; //Start from 2 EDGE
	S2.Init.CLKPolarity = SPI_POLARITY_LOW; //Trigger on Low
	S2.Init.FirstBit = SPI_FIRSTBIT_MSB; //Read MSB first
	HAL_SPI_Init(&S2);

}



//This is called upon SPI initialization. It handles the configuration
//of the GPIO pins for SPI.
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

	input = 0; //Initialize to 0

	printf("\033[2J"); fflush(stdout); //Clear Screen
	reg = 0x07; //For Major Version
	reg2 = 0x08; //For Minor Version
	read_version(reg, reg2, &spi_data, &spi_data2); //Read operation twice
	//Print Version Number
	printf("\033[0;0HVersion: [%d].[%d]\r\n", spi_data, spi_data2);
	//Clear registers
	reg = 0x00;
	reg2 = 0x00;

	while(1){
		//Check for input from keyboard
		HAL_UART_Receive (&USB_UART, &input, 1, 10);
		if ((char)input == '\033'){ //ESC Pressed
			printf("\033[2J"); fflush(stdout); //Clear Screen
			//Print Menu Options
			printf("\033[0;0HMenu Selection:\r\n");
			printf("Press the Number of the Function you would like to perform with the SPI\r\n");
			printf("2) Receive any Terminal Characters in SPI\r\n");
			printf("3) Display the Firmware\r\n");
			printf("4) Get a Temperature Measurement\r\n");
			printf("5) Reset/Clear Peripheral Terminal\r\n");
			printf("6) Change device ID\r\n");

			input = 0; //clear input
			while (input == 0){//Loop until they pressed a character
				HAL_UART_Receive (&USB_UART, &input, 1, 10);
			}
			printf("\033[2J"); fflush(stdout); //Clear Screen

			//Responds to ASCII value of numbers in DEC
			if (input == 50){ //Menu Operation 2
				//Check how many stored numbers
				reg = 0x01;
				read_from_reg(reg, &spi_data);

				//Bit math to check bits 6 and 5
				spi_data = spi_data | 0b10011111;
				if (spi_data == 0b11111111){//both were 1
					spi_data2 = 3;
				}else if (spi_data == 0b11011111){//only bit 6 was 1
					spi_data2 = 2;
				}else if (spi_data == 0b10111111){//only bit 5 was 1
					spi_data2 = 1;
				}else{//neither was 1
					spi_data2 = 0;
				}

				//Read from register 5 the stored characters
				reg = 0x05;
				spi_data = 0x00;
				read_terminal_char(reg, spi_data2, spi_data);
				printf("\033[0;0H"); fflush(stdout); //reset cursor
			}
			else if (input  == 51){ //Menu Operation 3
				//Needed registers
				reg = 0x07;
				reg2 = 0x08;
				//Read both major and minor versions and print
				read_version(reg, reg2, &spi_data, &spi_data2);
				printf("Version: [%d].[%d]", spi_data, spi_data2); fflush(stdout);
			}
			else if (input == 52){ //Menu Operation 4
				//request temperature measurement
				reg = 0x00;
				spi_data = 0x0E;
				write_to_reg(reg, spi_data);

				//Wait until temperature reading is ready
				reg = 0x01;
				spi_data = 0x00;
				spi_data2 = 0x01;

				//Breaks when TRDY is set to 1
				while (spi_data2){
					read_from_reg(reg, &spi_data);
					spi_data2 = spi_data | 0xF7;
					if (spi_data2 == 0xFF){ //Only when TRDY is 1
						spi_data2 = 0x00;
					}
				}

				//receive non-converted temp values
				reg = 0x03;
				reg2 = 0x04;
				get_temp(reg, reg2, &templo, &temphi);

				temp_complete = temphi;
				temp_complete = temp_complete<<8; //High byte, shift by 8
				temp_complete = temp_complete + templo; //Add with low byte

				final_temp = 357.6 - 0.187*(temp_complete); //Convert to Celcius

				//Printing
				printf("\033[12;0H\033[2K"); fflush(stdout); //Clear line 12
				printf("\033[12;40HTemp: %f Degrees Celcius\r\n", final_temp);
				printf("\033[0;0H"); fflush(stdout); //Reset cursor

				//Clear peripheral
				spi_data = 0x01;
				reg = 0x00;
				reset_device(reg, spi_data);
			}
			else if (input == 53){ //Menu Operation 5
				//Need to set bit 0, necessary register
				spi_data = 0x01;
				reg = 0x00;
				reset_device(reg, spi_data);
				printf("Peripheral Cleared\r\n");
			}
			else if (input == 54){ //Menu Operation 6
				//Set the 7th bit
				unlock = 0x80;
				//Needed registers
				reg = 0x00;
				reg2 = 0x09;
				spi_data = 11; //New name
				uint8_t old_id; //Storage for old_name
				read_from_reg(reg2, &spi_data2);
				old_id = spi_data2; //Store old name
				change_id(reg, reg2, spi_data, unlock, &spi_data2); //Change the ID
				printf("OLD ID: %d\r\n", old_id);
				printf("NEW ID: %d", spi_data2); fflush(stdout);
			}
			else{//A key was pressed but not 2-6
				printf("Invalid number/character pressed, please press <ESC> again\r\n");
			}
			//Reset variables
			input = 0;
			spi_data = 0;
			spi_data2 = 0;
			reg = 0x00;
			reg2 =0x00;
		}

		else if (input){ //Key was pressed that wasn't escape
			//Display the data that was input to terminal
			HAL_UART_Transmit (&USB_UART, &input, 1, 10);

			//Make sure the CS Pin is default high
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
			HAL_Delay(1);

			reg = 5; //register 5
			write_to_reg(reg, input); //write into to peripheral

			input = 0; //Clear
			reg = 0;

		}

	}

}

//Write to SPI on specified register, write the send_data in that register
void write_to_reg(uint8_t reg, uint8_t send_data){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET); //CS Low
	HAL_Delay(1); //Needed Delay
	HAL_SPI_Transmit(&S2, &reg, 1,  1000); //Communicate with specified register
	HAL_Delay(1);
	HAL_SPI_Transmit(&S2, &send_data, 1,  1000); //Write data to register
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET); //CS High
	HAL_Delay(1);
}

//Read from SPI on specified register, store received data in read_data variable
void read_from_reg (uint8_t reg, uint8_t* read_data){

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET); //CS Low
	HAL_Delay(1); //Needed Delay
	HAL_SPI_Transmit(&S2, &reg, 1, 100); //Communicate with specified register
	HAL_Delay(1);
	HAL_SPI_Receive(&S2, read_data, 1, 100); //Read and store data from register
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET); //CS High
	HAL_Delay(1);
}

//Option 2, Receive terminal characters from the peripheral device using SPI.
//The received characters should be printed on the bottom half of the controller’s terminal.
void read_terminal_char (uint8_t reg, uint8_t size, uint8_t read_data){
	printf("\033[18;0H"); fflush(stdout); //Bottom of screen
	if (size == 0){ //Nothing stored
		printf("No currently stored values\r\n");
	}
	while (size){//read as many times as there are characters
		read_from_reg(reg, &read_data); //read from register
		read_data = read_data - '0'; //get the character instead of ASCII value
		printf("Stored value: %d\r\n", read_data);
		size--; //buffer decreased
	}
}

//Option 3, Read the peripheral’s firmware version upon startup.
void read_version (uint8_t reg1, uint8_t reg2, uint8_t* read_data1, uint8_t* read_data2){
	//Read V_MAJ
	read_from_reg(reg1, read_data1);
	//Read V_MIN
	read_from_reg(reg2, read_data2);

}

//Option 4, Trigger a temperature measurement and retrieve the result when it is ready.
//The temperature should be printed on the right side of the terminal to avoid the transmitted
//and received terminal characters.
void get_temp(uint8_t reg1, uint8_t reg2, uint8_t* read_data1, uint8_t* read_data2){
	read_from_reg(reg1, read_data1); //Get low byte of temp
	read_from_reg(reg2, read_data2); //Get high byte of temp
}

//Option 5, Clear or reset the peripheral terminal.
void reset_device (uint8_t reg, uint8_t send_data){
	write_to_reg(reg, send_data);
}

//Option 6, Change the device ID of the peripheral.
void change_id (uint8_t reg1, uint8_t reg2, uint8_t read_data, uint8_t key, uint8_t* new_name){
	//Unlock
	write_to_reg(reg1, key);
	//Set New ID and read it
	write_to_reg(reg2, read_data);
	read_from_reg(reg2, new_name);
}

