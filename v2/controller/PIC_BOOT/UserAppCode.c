//#######################################################################
/**
* @file UserAppCode.c
* This is just a dummy file that only exists in the bootloader project.
* This file is a placeholder for the user app.
*
* @author Joe Dunne
* @date 12/9/13
* @brief User app placeholder
*/
//#######################################################################




/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include "typedefs.h"
#include "usb.h"
#include "io_cfg.h"
#include "BootPIC18NonJ.h"
#include "boot.h"



//Placeholder code at address 0x1000 (the start of the non-bootloader firmware space)
//This gets overwritten when a real hex file gets programmed by the bootloader.
//If however no hex file has been programmed (yet), might as well stay in the
//bootloader firmware, even if the pushbutton or software entry attempt has not
//been made yet, since the device is still blank.
#pragma code user_app_vector = REMAPPED_APPLICATION_RESET_VECTOR
void userApp(void)
{
	_asm goto BOOTLOADER_ABSOLUTE_ENTRY_ADDRESS _endasm 	//Goes into the BootMain() section which force bootloader mode operation
}

//Initialize with a valid application signature already loaded to allow the code to run at startup rather than jumping to bootload mode.
#pragma	romdata	app_signature=APP_SIGNATURE_ADDRESS
rom const unsigned short app_sig = {APP_SIGNATURE_VALUE};

#pragma	romdata	app_version=APP_VERSION_ADDRESS
rom const unsigned short app_ver = {APP_VERSION};
