// for hex grid
//http://www.gamedev.net/reference/articles/article1800.asp
#include "HexMap.h"
#include "City.h"
#include "HexNode.h"

#include <grrlib.h>

//for debug created in SCWii.cpp
#include "NetworkConsole.h"
extern NetworkConsole netCon;

//widthx4,height x2
HexMap::HexMap(int width, int height):Map( width, height)
{
	// mercator projection
	prime = new HexNode(PRIME);
	east_pole = new HexNode(EAST_POLE);
	anti_prime=new HexNode(ANTI_PRIME);
	west_pole = new HexNode(WEST_POLE);
	
	north_pole = new HexNode(NORTH_POLE);
	south_pole = north_pole;
	highlighted=0;
	
	
	{
		HexNode * current_node[4];
		current_node[0]= prime;
		current_node[1] = east_pole;
		current_node[2] = anti_prime;
		current_node[3] = west_pole;
		//build equator
		for(int i=1; i<width; i++)
		{
			//    *     *     *
			// *     *     *
			// -> 1 <-> 2 <-> 3 <-
			for(int j=0; j<4; j++)
			{
				//current_node[j]->neighbor[EAST] = new HexNode;
				//current_node[j]->neighbor[EAST]->neighbor[WEST]=current_node[j];
				//current_node[j]=current_node[j]->neighbor[EAST];
				current_node[j]=current_node[j]->addNeighbor(EAST);
			}
		}
		//finialize edge
			//width=1 [3] == west pole
		prime->addNeighbor(WEST,current_node[3]);
			//width=1 [0] == prime
		east_pole->addNeighbor(WEST,current_node[0]);
			//width=1 [1] == EAST pole
		anti_prime->addNeighbor(WEST,current_node[1]);
			//width=1 [2] == anti_prime
		west_pole->addNeighbor(WEST,current_node[2]);
	}
	
	{
		// build prime meridian date line
		// HexMap wrap removes south pole possibility
		HexNode * meridian[2];
		meridian[0]= prime;//heading north
		meridian[1] = prime;//heading south

		for(int i=1; i<height; i++)
		{
			meridian[0] = meridian[0]->addNeighbor(NORTH_EAST);
			meridian[0]->isGround = true;
			meridian[1] = meridian[1]->addNeighbor(SOUTH_EAST);
			
			//fill in lattitude
			HexNode * current_node[2];
			current_node[0] = meridian[0]->addNeighbor(EAST);
			current_node[1] = meridian[1]->addNeighbor(EAST);
			int a=0;
			while(current_node[0]->neighbor[EAST]!=meridian[0])
			{
				if(a++>100)
				{
					netCon.sendMessage("meridian loop too much");
					break;
				}
				current_node[0] = current_node[0]->addNeighbor(EAST);
				current_node[1] = current_node[1]->addNeighbor(EAST);
			}
		}
		//attach to pole
		meridian[0]->addNeighbor(NORTH_EAST,north_pole);
		meridian[1]->addNeighbor(SOUTH_EAST,north_pole);
		
		int a=0;//debug
		HexNode * current_node=north_pole->addNeighbor(EAST);
		while(current_node->neighbor[EAST]!=north_pole)
		{
			if(a++>100)
			{
				netCon.sendMessage("north pole loop too much");
				break;
			}
			current_node=current_node->addNeighbor(EAST);
		}
/**/}

	prime->isGround = true;
	prime->neighbor[EAST]->isGround = true;
	prime->neighbor[NORTH_EAST]->isGround=true;
	prime->city = new City;
	prime->city->location=prime;
}




//Change so that nodes are unknown to outside world
void
HexMap::SetStart(HexNode * s)
{
	start = s;
}
void
HexMap::DrawMap()
{
	#define BLUE 0x0000FFAA
	#define GREEN 0x00FF00AA
	
	//int center_x = 300;
	//int center_y = 200;
	HexNode * current_lattitude = start;
	for( int i=0; i<30; i++)
	{
		// draw lattitude
		HexNode * current_longitude = current_lattitude;
		for( int j=0; j<40; j++)
		{
			int offset = 9;
			if(i%2)
				offset = 0;
			
			DrawNode(current_longitude, j*18 + offset, i*17);
			current_longitude = current_longitude->neighbor[EAST];
			
		}
		if(i%2)
			current_lattitude = current_lattitude->neighbor[SOUTH_EAST];
		else
			current_lattitude = current_lattitude->neighbor[SOUTH_WEST];
	}
}

void
HexMap::DrawNode(HexNode * n, int x, int y)
{
	if(n==highlighted)
		GRRLIB_Circle(x,y, 11,0xFFFFFFFF,1);
	if(n->isGround)
		GRRLIB_Circle(x, y, 9, GREEN, 1);
	else
		GRRLIB_Circle(x, y, 9, BLUE, 1);
	switch(n->location)
	{	
		case EAST_POLE:
			GRRLIB_Circle(x+5, y, 5, 0x000000FF, 1);
			break;
		case PRIME:
			GRRLIB_Circle(x, y, 5, 0x000000FF, 1);
			break;
		case WEST_POLE:
			GRRLIB_Circle(x-5, y, 5, 0x000000FF, 1);
			break;
		case ANTI_PRIME:
			GRRLIB_Line(x+3, y+4, x-5, y-4, 0x000000FF);
			GRRLIB_Line(x+4, y+4, x-4, y-4, 0x000000FF);
			GRRLIB_Line(x+5, y+4, x-3, y-4, 0x000000FF);
			GRRLIB_Line(x-5, y+4, x+3, y-4, 0x000000FF);
			GRRLIB_Line(x-4, y+4, x+4, y-4, 0x000000FF);
			GRRLIB_Line(x-3, y+4, x+5, y-4, 0x000000FF);
			break;
		case NORTH_POLE:
			GRRLIB_Circle(x, y-5, 5, 0x000000FF, 1);
			break;
		default:
		;
	}
	if(n->city)
		GRRLIB_Circle(x, y, 2, 0xFFFFFFFF, 1);
}



void
HexMap::highlightNode(int x, int y)
{
	
}