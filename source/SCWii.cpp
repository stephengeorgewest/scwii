#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <string>
#include "NetworkConsole.h"
#include "Map.h"
#include "Node.h"
/*
http://en.wikipedia.org/wiki/Strategic_Conquest
*/
NetworkConsole netCon;
using namespace std;
void dir_test();
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
	netCon.Init();
	netCon.sendMessage("Hello from Wii\n");
	printf("Home to exit");
	
	//dir_test();
	
	Map gameboard(2,2);
	netCon.sendMessage("map initialized\n");
	bool done = false;
	Node * current_selected;
	current_selected=gameboard.prime;
	while(!done)
	{
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		u32 held = WPAD_ButtonsHeld(0);
		if(pressed & WPAD_BUTTON_HOME)
			done=true;
		
		if((pressed & WPAD_BUTTON_RIGHT) &&
			!(held & (WPAD_BUTTON_UP | WPAD_BUTTON_DOWN))
			)
		{
			Node * next = current_selected->neighbor[EAST];
			if(next != 0 )
			{
				netCon.sendMessage("\n\nMoving East");
				current_selected = next;
			}
			else
				netCon.sendMessage("\n\n East NULL Node");
			
		}
		if(
			(pressed & WPAD_BUTTON_LEFT) &&
			!(held & (WPAD_BUTTON_UP | WPAD_BUTTON_DOWN))
			)
		{
			
			Node * next = current_selected->neighbor[WEST];
			if(next != 0 )
			{
				current_selected = next;
				netCon.sendMessage("\n\nMoving West");
			}
			else
				netCon.sendMessage("\n\n West NULL Node");
		}
		
		
		//North and south
		if(
			(pressed & WPAD_BUTTON_LEFT) &&
			(held & (WPAD_BUTTON_UP))
			)
		{
			
			Node * next = current_selected->neighbor[NORTH_WEST];
			if(next != 0 )
			{
				current_selected = next;
				netCon.sendMessage("\n\nMoving NOrth West");
			}
			else
				netCon.sendMessage("\n\n NOrth West NULL Node");
		}
		if(
			(pressed & WPAD_BUTTON_RIGHT) &&
			(held & (WPAD_BUTTON_UP))
			)
		{
			
			Node * next = current_selected->neighbor[NORTH_EAST];
			if(next != 0 )
			{
				current_selected = next;
				netCon.sendMessage("\n\nMoving NOrth East");
			}
			else
				netCon.sendMessage("\n\n NOrth east NULL Node");
		}
		
		
		if(
			(pressed & WPAD_BUTTON_LEFT) &&
			(held & (WPAD_BUTTON_DOWN))
			)
		{
			
			Node * next = current_selected->neighbor[SOUTH_WEST];
			if(next != 0 )
			{
				current_selected = next;
				netCon.sendMessage("\n\nMoving SOUTH West");
			}
			else
				netCon.sendMessage("\n\n SOUTH West NULL Node");
		}
		if(
			(pressed & WPAD_BUTTON_RIGHT) &&
			(held & (WPAD_BUTTON_DOWN))
			)
		{
			
			Node * next = current_selected->neighbor[SOUTH_EAST];
			if(next != 0 )
			{
				current_selected = next;
				netCon.sendMessage("\n\nMoving SOUTH EAST");
			}
			else
				netCon.sendMessage("\n\n SOUTH East NULL Node");
		}
		
		
		
		//print current node
		if(pressed & WPAD_BUTTON_A)// | WPAD_BUTTON_DOWN))
		{
			//string str = new string(current_selected);
			char buf[64];
			sprintf(buf, "\nCurrent Node - %i", current_selected->ID);
			netCon.sendMessage(buf);
			if(current_selected->isGround)
				netCon.sendMessage(" -ground");
			else
				netCon.sendMessage(" -water");
			if(current_selected->city)
				netCon.sendMessage(" -city");
			if(current_selected ==gameboard.prime)
				netCon.sendMessage(" -prime");
			if(current_selected ==gameboard.east_pole)
				netCon.sendMessage(" -east_pole");
			if(current_selected ==gameboard.anti_prime)
				netCon.sendMessage(" -anti_prime");
			if(current_selected ==gameboard.west_pole)
				netCon.sendMessage(" -west_pole");
			if(current_selected ==gameboard.north_pole)
				netCon.sendMessage(" -north_pole");
			if(current_selected ==gameboard.south_pole)
				netCon.sendMessage(" -south_pole");
		}
		
		VIDEO_WaitVSync();
	}
	netCon.sendMessage("closing");
	
	if (consoleframeBuffer != 0)
	{
		free(MEM_K1_TO_K0(consoleframeBuffer)); 
		consoleframeBuffer  = 0;
	}
	//close java client
	//netCon.sendMessage("exit");
	//exit(0);
	return(0);
}
void dir_test()
{
	Direction e=EAST,se=SOUTH_EAST,sw=SOUTH_WEST,w=WEST,nw=NORTH_WEST,ne=NORTH_EAST;
	netCon.sendMessage("Direction Test");
	char buffer[256];
	sprintf(buffer,"\
		EAST %i,\
		SOUTH_EAST %i,\
		SOUTH_WEST %i,\
		WEST %i,\
		NORTH_WEST %i,\
		NORTH_EAST %i",
		
		e,
		se,
		sw,
		w,
		nw,
		ne);
	netCon.sendMessage(buffer);
	
	sprintf(buffer,"\
		EAST+1 %i,\
		SOUTH_EAST+1 %i,\
		SOUTH_WEST+1 %i,\
		WEST+1 %i,\
		NORTH_WEST+1 %i,\
		NORTH_EAST+1 %i",
		
		EAST+1,
		se+1,
		sw+1,
		w+1,
		nw+1,
		ne+1);
	netCon.sendMessage(buffer);
	
	sprintf(buffer,"\
		EAST %i,\
		SOUTH_EAST %i,\
		SOUTH_WEST %i,\
		WEST %i,\
		NORTH_WEST %i,\
		NORTH_EAST %i",
		
		EAST,
		se,
		sw,
		w,
		nw,
		ne);
	netCon.sendMessage(buffer);
	
	sprintf(buffer,"\
		EAST-1 %i,\
		SOUTH_EAST-1 %i,\
		SOUTH_WEST-1 %i,\
		WEST-1 %i,\
		NORTH_WEST-1 %i,\
		NORTH_EAST-1 %i",
		
		EAST-1,
		se-1,
		sw-1,
		w-1,
		nw-1,
		ne-1);
	netCon.sendMessage(buffer);
	
	sprintf(buffer,"\
		EAST-1 %i,\
		SOUTH_EAST-1 %i,\
		SOUTH_WEST-1 %i,\
		WEST-1 %i,\
		NORTH_WEST-1 %i,\
		NORTH_EAST-1 %i",
		
		EAST-1,
		se-1,
		sw-1,
		w-1,
		nw-1,
		ne-1);
	netCon.sendMessage(buffer);
	
	sprintf(buffer,"\
		~EAST %i,\
		~SOUTH_EAST %i,\
		~SOUTH_WEST %i,\
		~WEST %i,\
		~NORTH_WEST %i,\
		~NORTH_EAST %i",
		
		~EAST,
		~se,
		~sw,
		~w,
		~nw,
		~ne);
	netCon.sendMessage(buffer);
}