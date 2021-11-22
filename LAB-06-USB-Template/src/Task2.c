/*
 * Task2.c
 *
 *  Created on: Nov 22, 2021
 *      Author: Shayne
 */
#include "init.h"
#include "usbh_conf.h"
#include "usbh_hid.h"
#include "usbh_core.h"
#include "ff_gen_drv.h"
#include "usbh_diskio.h"

USBH_HandleTypeDef husbh;

HID_MOUSE_Info_TypeDef *mouse;
int connected = 0;
FATFS *fs;     /* Ponter to the filesystem object */
DIR dp;
char* path;
UINT i;
static FILINFO fno;
FRESULT res;
void USBH_UserProcess(USBH_HandleTypeDef *, uint8_t);

int main(void){
	 // System Initializations
	Sys_Init();
	USBH_Init(&husbh, USBH_UserProcess, 0);
	USBH_RegisterClass(&husbh, USBH_HID_CLASS);	// USB Driver Class Registrations: Add device types to handle.
	USBH_RegisterClass(&husbh, USBH_MSC_CLASS);	// #1
	FATFS_LinkDriver(&USBH_Driver, "0:/"); // #2
	USBH_Start(&husbh); // Start USBH Driver
	fflush(stdout);
	printf("start\n\r");

    fs = malloc(sizeof (FATFS));           /* Get work area for the volume */
    while(1){
		//printf("while\n\r");
		USBH_Process(&husbh);
	}
}

void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id) {
	if (phost->pActiveClass == USBH_MSC_CLASS){
	    f_mount(fs, "", 0);                    /* Mount the default drive */
	    path[0] = '/';
	    res = f_opendir(&dp, path);
	    if (res == FR_OK) {
			for (;;) {
				res = f_readdir(&dp, &fno);
				if (res != FR_OK || fno.fname[0] == 0) break;
	            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
	                i = strlen(path);
	                sprintf(&path[i], "/%s", fno.fname);
	                //res = scan_files(path);                    /* Enter the directory */
	                if (res != FR_OK) break;
	                path[i] = 0;
	            } else {                                       /* It is a file. */
	                printf("%s/%s\n", path, fno.fname);
	            }
			}
			f_closedir(&dp);
	    }
	}
}

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost){
	if (phost->pActiveClass == USBH_HID_CLASS){
		if (USBH_HID_GetDeviceType(phost) == HID_MOUSE){ //#3
			mouse = USBH_HID_GetMouseInfo(&husbh);
			int xval = mouse->x;
			int yval = mouse->y;
			printf("X value moved: %d, Y value moved: %d, Button 1: %d, Button 2: %d, Button 3: %d\n\r", xval, yval, \
					mouse->buttons[0], mouse->buttons[1], mouse->buttons[2]);
		}
	}
}


