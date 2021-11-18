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
int connected = 0;

void USBH_UserProcess(USBH_HandleTypeDef *, uint8_t);



int main(void){
	 // System Initializations
	Sys_Init();
	//HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
	// Application Initializations
	//#define HAL_HCD_MODULE_ENABLED	// USBH Driver Initialization...Don't think this is right?
	USBH_Init(&husbh, USBH_UserProcess, 0);
	USBH_RegisterClass(&husbh, USBH_HID_CLASS);	// USB Driver Class Registrations: Add device types to handle.
	USBH_Start(&husbh); // Start USBH Driver
	fflush(stdout);
	printf("start\n\r");
	while(1){
		//printf("while\n\r");
		USBH_Process(&husbh);
	}
}

void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id) {

}

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost){
	if (USBH_HID_GetDeviceType(phost) == HID_MOUSE){
		mouse = USBH_HID_GetMouseInfo(&husbh);
		int xval = mouse->x;
		int yval = mouse->y;
		printf("X value moved: %d, Y value moved: %d, Button 1: %d, Button 2: %d, Button 3: %d\n\r", xval, yval, \
				mouse->buttons[0], mouse->buttons[1], mouse->buttons[2]);
	}
}
