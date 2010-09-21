#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include "NetworkConsole.h"
#include "Map.h"

int main(void)
{

	VIDEO_Init();		
	WPAD_Init();
	
	GXRModeObj *screenMode = VIDEO_GetPreferredMode(NULL);
	
	void *consoleframeBuffer = MEM_K0_TO_K1(SYS_AllocateFramebuffer(screenMode));
	//void *mapFrameBuffer =  MEM_K0_TO_K1(SYS_AllocateFramebuffer(screenMode));
	console_init(consoleframeBuffer,20,20,screenMode->fbWidth,screenMode->xfbHeight,screenMode->fbWidth*VI_DISPLAY_PIX_SZ);
	VIDEO_Configure(screenMode);
	VIDEO_SetNextFramebuffer(consoleframeBuffer);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	
	printf("\x1b[2;0H");
	printf("Hello World!\n");
	printf("initializing network console\n");
	NetworkConsole netCon;
	netCon.send_message("Hello from Wii");
	printf("down + home to exit");
	Map gameboard;
	while(true)
	{
		WPAD_ScanPads();
		if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME)
			exit(0);
		VIDEO_WaitVSync();
	}
	return(0);
}