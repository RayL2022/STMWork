/*
 * Task2.c of Lab 06
 *
 *	Same mouse functionality as task 1
 *	Ability to support MSC flash drive connection
 *	Display contents of root directory of flash drive and
 *	differentiate between file and directory, only upon connection
 *
 *  Created on: Nov 09, 2021
 *      Author: Ray & Shayne
 */
#include "init.h"
#include "usbh_conf.h"
#include "usbh_hid.h"
#include "usbh_core.h"
#include "ff_gen_drv.h"
#include "usbh_diskio.h"

USBH_HandleTypeDef husbh; //Handle type for USBH communication

HID_MOUSE_Info_TypeDef *mouse; //Mouse information structure

FATFS *fs; //File System Structure/Workspace
FRESULT fr; //Check return status of function runs
DIR dir; //Directory Object Structure
FILINFO fno; //File Information Structure

void USBH_UserProcess(USBH_HandleTypeDef *, uint8_t);

int main(void){
	// System Initializations
	Sys_Init();

	USBH_Init(&husbh, USBH_UserProcess, 0); //Initialize HID USB Handle

	USBH_RegisterClass(&husbh, USBH_HID_CLASS);	//Ability to handle Human Interface Devices (HID)
	USBH_RegisterClass(&husbh, USBH_MSC_CLASS);	//Ability to handle Mass Storage Class (MSC) device types

	FATFS_LinkDriver(&USBH_Driver, "0:/"); //Link our connected first device ("0:/") to the FATFS driver
	USBH_Start(&husbh); //Start USBH Driver
	fflush(stdout);
	fs = malloc(sizeof (FATFS)); //Get work area for the volume

    while(1){
		USBH_Process(&husbh); //Poll for USB updates
	}
}

void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id) {
	//Check to see the flash drive or MSC device is connected
	if (phost->pActiveClass == USBH_MSC_CLASS){
		//Modified From Elm-Chan.org f_findfirst Code Example
	    fr = f_mount(fs, "0:/", 0); //Mount the default drive
	    if (fr == FR_OK) { //No errors with mounting
	        fr = f_findfirst(&dir, &fno, "0:/", "*"); //Start to search for any file
	        while (fr == FR_OK && fno.fname[0]){ //Break when error or no more items matched
	            if (fno.fattrib & AM_DIR) { //It is a directory
		            printf("Directory: %s\r\n", fno.fname); //Print the object name
	            }
	            else{ //It is a file
		            printf("File: %s\r\n", fno.fname); //Print the object name
	            }
	            fr = f_findnext(&dir, &fno); //Search for next item
	        }
	        f_closedir(&dir); //Close directory
	    }
	}
}

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost){
	if (phost->pActiveClass == USBH_HID_CLASS){ //HID connected
		if (USBH_HID_GetDeviceType(phost) == HID_MOUSE){ //If a mouse is Connected
			mouse = USBH_HID_GetMouseInfo(&husbh); //Get mouse info
			int xval = mouse->x;
			int yval = mouse->y; //Assign info from mouse to variables
			printf("X value moved: %d, Y value moved: %d, Button 1: %d, Button 2: %d, Button 3: %d\n\r", xval, yval, \
					mouse->buttons[0], mouse->buttons[1], mouse->buttons[2]); //Print Mouse Info
		}
	}
}

