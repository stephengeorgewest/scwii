#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <gccore.h>
#include <wiiuse/wpad.h>

#include <grrlib.h>
#include "FreeMonoBold_ttf.h"
#include <debug.h>
#include <network.h>
#include <unistd.h>

#include "NetworkConsole.h"
#include "HexMap.h"
#include "HexNode.h"
#include "Terrain.h"
/*
http://en.wikipedia.org/wiki/Strategic_Conquest
*/
NetworkConsole netCon;
using namespace std;
void dir_test();

void ShutdownCB( s32 chan)
{
        exit(-1);
}
int main(void)
{

	VIDEO_Init();		
	WPAD_Init();
	WPAD_SetPowerButtonCallback((WPADShutdownCallback)ShutdownCB);
	
	
	
	GRRLIB_Init();
	GRRLIB_Settings.antialias = true;
	// Black background
	GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);
	// Load the font from memory
	GRRLIB_ttfFont *myFont = GRRLIB_LoadTTF(FreeMonoBold_ttf, FreeMonoBold_ttf_size);
	
	/*
	char localip[16] = {0};
	char gateway[16] = {0};
	char netmask[16] = {0};
	
	// Configure the network interface
	s32 ret = if_config ( localip, netmask, gateway, TRUE);
	
	DEBUG_Init(100, 5656);
_break();*/
	netCon.Init();
	netCon.sendMessage("Hello from Wii\n");
	printf("Home to exit");
	
	//dir_test();
	
	HexMap gameboard(2,2);
	netCon.sendMessage("map initialized\n");
	
	HexNode * current_selected;
	current_selected=gameboard.prime;
	
	bool done = false;
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
			HexNode * next = current_selected->neighbor[EAST];
			if(next != 0 )
			{
				netCon.sendMessage("\n\nMoving East");
				current_selected = next;
			}
			else
				netCon.sendMessage("\n\n East NULL HexNode");
			gameboard.SetStart(current_selected);
			
		}
		if(
			(pressed & WPAD_BUTTON_LEFT) &&
			!(held & (WPAD_BUTTON_UP | WPAD_BUTTON_DOWN))
			)
		{
			
			HexNode * next = current_selected->neighbor[WEST];
			if(next != 0 )
			{
				current_selected = next;
				netCon.sendMessage("\n\nMoving West");
			}
			else
				netCon.sendMessage("\n\n West NULL HexNode");
			gameboard.SetStart(current_selected);
		}
		
		
		//North and south
		if(
			(pressed & WPAD_BUTTON_LEFT) &&
			(held & (WPAD_BUTTON_UP))
			)
		{
			
			HexNode * next = current_selected->neighbor[NORTH_WEST];
			if(next != 0 )
			{
				current_selected = next;
				netCon.sendMessage("\n\nMoving NOrth West");
			}
			else
				netCon.sendMessage("\n\n NOrth West NULL HexNode");
			gameboard.SetStart(current_selected);
		}
		if(
			(pressed & WPAD_BUTTON_RIGHT) &&
			(held & (WPAD_BUTTON_UP))
			)
		{
			
			HexNode * next = current_selected->neighbor[NORTH_EAST];
			if(next != 0 )
			{
				current_selected = next;
				netCon.sendMessage("\n\nMoving NOrth East");
			}
			else
				netCon.sendMessage("\n\n NOrth east NULL HexNode");
			gameboard.SetStart(current_selected);
		}
		
		
		if(
			(pressed & WPAD_BUTTON_LEFT) &&
			(held & (WPAD_BUTTON_DOWN))
			)
		{
			
			HexNode * next = current_selected->neighbor[SOUTH_WEST];
			if(next != 0 )
			{
				current_selected = next;
				netCon.sendMessage("\n\nMoving SOUTH West");
			}
			else
				netCon.sendMessage("\n\n SOUTH West NULL HexNode");
			gameboard.SetStart(current_selected);
		}
		if(
			(pressed & WPAD_BUTTON_RIGHT) &&
			(held & (WPAD_BUTTON_DOWN))
			)
		{
			
			HexNode * next = current_selected->neighbor[SOUTH_EAST];
			if(next != 0 )
			{
				current_selected = next;
				netCon.sendMessage("\n\nMoving SOUTH EAST");
			}
			else
				netCon.sendMessage("\n\n SOUTH East NULL HexNode");
			gameboard.SetStart(current_selected);
		}
		
		
		
		//print current HexNode
		if(pressed & WPAD_BUTTON_A)// | WPAD_BUTTON_DOWN))
		{
			//string str = new string(current_selected);
			char buf[64];
			sprintf(buf, "\nCurrent HexNode - %i", current_selected->ID);
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
		gameboard.DrawMap();
		GRRLIB_Render();
		//VIDEO_WaitVSync();
	}
	
	//Terrain test Section
	int w = 512, h =560;
	
	float decay = 0.7;
	//float seaLevel = 0.65;
	int start_x=10, start_y=10;
	Terrain * land = new Terrain(w, h, decay, true);
	netCon.sendMessage("Terrain initialized");
	
	done = true;
	while(!done)
	{
		//GRRLIB_FillScreen(0xFFaaaaFF);
		(*land).Draw(start_x,start_y);
		char str[128];
			sprintf(str, "Width = %i, Height = %i, Decay = %f", w, h, decay);
			GRRLIB_PrintfTTF(50,
						rmode->efbHeight - 50,
						myFont,
						str,
						20,
						0xFFFFFFFF);
		GRRLIB_Render();
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		if(pressed & WPAD_BUTTON_HOME)
			done=true;
		if((pressed & WPAD_BUTTON_PLUS) && decay<1.0)
			decay+=0.01;
		if((pressed & WPAD_BUTTON_MINUS) && decay>=0.0)
			decay-=0.01;
		/*if((pressed & WPAD_BUTTON_DOWN) && seaLevel<1.0)
			seaLevel+=0.01;
		if((pressed & WPAD_BUTTON_UP) && seaLevel>=0.0)
			seaLevel-=0.01;
		*/
		if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_B)
		{
			if((pressed & WPAD_BUTTON_DOWN) )
				h++;
			if((pressed & WPAD_BUTTON_UP) && h>=0)
				h--;
			if((pressed & WPAD_BUTTON_LEFT) && w >=0)
				w--;
			if((pressed & WPAD_BUTTON_RIGHT) )
				w++;
		}
		else
		{
			if((pressed & WPAD_BUTTON_DOWN) )
				start_y+=10;
			if((pressed & WPAD_BUTTON_UP) )
				start_y-=10;
			if((pressed & WPAD_BUTTON_LEFT) )
				start_x-=10;
			if((pressed & WPAD_BUTTON_RIGHT) )
				start_x+=10;
		}
		if(pressed & WPAD_BUTTON_A)
		{
			char str[128];
			delete land;
			netCon.sendMessage("land Deleted");
			land = new Terrain(w, h, decay, true);
			sprintf(str,"decay=%f, max=%f, min=%f",decay, land->max,land->min);
			netCon.sendMessage(str);
		}
		
	}
	delete land;
	netCon.sendMessage("closing");
	GRRLIB_Exit();
	//close java client
	//netCon.sendMessage("exit");
	//exit(0);
	return(0);
}
void dir_test()
{
	HexDirection e=EAST,se=SOUTH_EAST,sw=SOUTH_WEST,w=WEST,nw=NORTH_WEST,ne=NORTH_EAST;
	netCon.sendMessage("HexDirection Test");
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