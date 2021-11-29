//--------------------------------
// Microprocessor Systems Lab 6 - Template - Lab06_p1_sample.c
//--------------------------------
//
//

#include "init.h"
#include "usbh_conf.h"
#include "usbh_hid.h"
#include "usbh_core.h"
#include "ff_gen_drv.h"
#include "usbh_diskio.h"

USBH_HandleTypeDef husbh;
HID_MOUSE_Info_TypeDef *mouse;

void USBH_UserProcess(USBH_HandleTypeDef *, uint8_t);

int main(void){
	 // System Initializations
	Sys_Init();
	USBH_Init(&husbh, USBH_UserProcess, 0);	//Initialize HID USB Handle
	USBH_RegisterClass(&husbh, USBH_HID_CLASS);	// USB Driver Class Registrations: Add HID to handle.
	USBH_Start(&husbh); // Start USBH Driver
	fflush(stdout);
	while(1){
		USBH_Process(&husbh);	//Poll for USB updates
	}
}

void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id) {
	//Connected to USBH_Process(&husbh) function call
}

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost){
	if (USBH_HID_GetDeviceType(phost) == HID_MOUSE){	//If a mouse is connected
		mouse = USBH_HID_GetMouseInfo(&husbh);	//Get mouse info
		int xval = mouse->x;
		int yval = mouse->y;	//Assign info from mouse to variables
		printf("X value moved: %d, Y value moved: %d, Button 1: %d, Button 2: %d, Button 3: %d\n\r", xval, yval, \
				mouse->buttons[0], mouse->buttons[1], mouse->buttons[2]);	//Print Mouse Info
	}
}
