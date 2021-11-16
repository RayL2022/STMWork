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

HCD_HandleTypeDef husb;
USBH_HandleTypeDef husbh;
HID_MOUSE_Info_TypeDef *mouse;

void USBH_UserProcess(USBH_HandleTypeDef *, uint8_t);



int main(void){
	 // System Initializations
	Sys_Init();

	// Application Initializations....What are these?
	#define HAL_HCD_MODULE_ENABLED	// USBH Driver Initialization...Don't think this is right?
	USBH_RegisterClass(&husbh, USBH_HID_CLASS);	// USB Driver Class Registrations: Add device types to handle.
	USBH_Start(&husbh); // Start USBH Driver

	while(1){
		USBH_Process(&husbh);	//What else would be needed if this triggers interrupt?
		// Other stuff
	}
}

void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id) {
	//Why is this needed?
}

void OTG_HS_IRQ(void){
	HAL_HCD_IRQHandler(&husb);	//Why this handle when the callbacks use the other handletype?
}

// Interrupts and Callbacks...
void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd){
	mouse = USBH_HID_GetMouseInfo(&husbh);
	int xval = mouse->x;
	int yval = mouse->y;
	printf("X value moved: %d, Y value moved: %d, Button 1: %d, Button 2: %d, Button 3: %d", xval, yval, \
			mouse->buttons[0], mouse->buttons[1], mouse->buttons[2]);
}
void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd);
