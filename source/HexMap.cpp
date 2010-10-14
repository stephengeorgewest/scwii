// for hex grid
//http://www.gamedev.net/reference/articles/article1800.asp
#include "HexMap.h"
#include "City.h"
#include "HexNode.h"
#include "Terrain.h"

#include <grrlib.h>

//for debug created in SCWii.cpp
#include "NetworkConsole.h"
extern NetworkConsole netCon;

//widthx4,height x2
HexMap::~HexMap()
{
	delete land;
}
HexMap::HexMap(int width, int height):Map( width, height)
{
	s=10.0;
	#define PI 3.14159265358979323846264338327950288419716939937510
	h = sin( 2*PI*30/360 ) * s;
	float r = cos( 2*PI*30/360 ) * s;
	b = s + 2 * h;
	a = 2 * r;
	land = new Terrain((width*4)*(a+0.5), (height*4+2)*(b+0.5),0.7,true);
	// mercator projection
	prime = new HexNode(PRIME);
	prime->x=0;
	prime->y=2*height;
	east_pole = new HexNode(EAST_POLE);
	east_pole->x=width;
	east_pole->y=2*height;
	anti_prime=new HexNode(ANTI_PRIME);
	anti_prime->x=2*width;
	anti_prime->y=2*height;
	west_pole = new HexNode(WEST_POLE);
	west_pole->x=3*width;
	west_pole->y=2*height;
	
	north_pole = new HexNode(NORTH_POLE);
	south_pole = north_pole;
	highlighted=0;
	north_pole->x=0;
	north_pole->y=0;
	
	
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
				/*if(land->getAverageHeight(current_node[j]->x*a, current_node[j]->y*b, r)>=0.5)
				{
					current_node[j]->isGround=true;
				}*/
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
		
		for(int i=0; i<height*2+1; i++)
		{
			if(i%2==0)
			{
				//char str[128];
				//sprintf(str,"East %i",i);
				//netCon.sendMessage(str);
				meridian[0] = meridian[0]->addNeighbor(NORTH_EAST);
				meridian[1] = meridian[1]->addNeighbor(SOUTH_EAST);
				
			}
			else
			{
				//char str[128];
				//sprintf(str,"West %i",i);
				//netCon.sendMessage(str);
				meridian[0] = meridian[0]->addNeighbor(NORTH_WEST);
				meridian[0]->isGround = true;
				meridian[1] = meridian[1]->addNeighbor(SOUTH_WEST);
			}
			/*if(land->getAverageHeight(meridian[0]->x*a, meridian[0]->y*b, r)>=0.5)
			{
				meridian[0]->isGround=true;
			}
			if(land->getAverageHeight(meridian[1]->x*a, meridian[1]->y*b, r)>=0.5)
			{
				meridian[1]->isGround=true;
			}*/
			//fill in lattitude
			HexNode * current_node[2];
			current_node[0] = meridian[0]->addNeighbor(EAST);
			current_node[1] = meridian[1]->addNeighbor(EAST);
			int j=1;
			while(current_node[0]->neighbor[EAST]!=meridian[0])
			{
				if(j++>100)
				{
					netCon.sendMessage("meridian loop too much");
					break;
				}
				current_node[0] = current_node[0]->addNeighbor(EAST);
				current_node[1] = current_node[1]->addNeighbor(EAST);
				
			/*char str[128];
			sprintf(str,"x=%i, y=%i",current_node[0]->x, current_node[0]->y);
			netCon.sendMessage(str);
				if(land->getAverageHeight(current_node[0]->x*this->a, current_node[0]->y*this->b, this->r)>=0.5)
				{
					current_node[0]->isGround=true;
				}
				if(land->getAverageHeight(current_node[1]->x*this->a, current_node[1]->y*this->b, this->r)>=0.5)
				{
					current_node[1]->isGround=true;
				}
				
			netCon.sendMessage("End");*/
			}
		}
				netCon.sendMessage("north Pole");
		//attach to pole
		meridian[0]->addNeighbor(NORTH_WEST,north_pole);
		meridian[1]->addNeighbor(SOUTH_WEST,north_pole);
		
		HexNode * current_node=north_pole->addNeighbor(EAST);
		int j=1;//debug
		while(current_node->neighbor[EAST]!=north_pole)
		{
			if(j++>100)
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
	CheckMap();
}

void
HexMap::CheckMap()
{

	HexNode * up = prime;
	int q=0;
	do
	{
		HexNode * r = up;
		do
		{
			char str[256];
			char loc[8];
			if(r==this->prime)
				sprintf(loc,"prime");
			else if(r==this->anti_prime)
				sprintf(loc,"anti");
			else if(r==this->north_pole)
				sprintf(loc,"north");
			else if(r==this->east_pole)
				sprintf(loc,"east");
			else if(r==this->west_pole)
				sprintf(loc,"west");
			else
				sprintf(loc,"");
			sprintf(str,"%i -- x=%i, y=%i, %s",(int)r,r->x,r->y,loc);
			netCon.sendMessage(str);
			sprintf(str,"\t[ East(%i,%i), SOUTH_EAST(%i,%i), SOUTH_WEST(%i,%i), WEST(%i,%i), NORTH_WEST(%i,%i), NORTH_EAST(%i,%i)]",
				r->neighbor[      EAST]->x,r->neighbor[      EAST]->y,
				r->neighbor[SOUTH_EAST]->x,r->neighbor[SOUTH_EAST]->y,
				r->neighbor[SOUTH_WEST]->x,r->neighbor[SOUTH_WEST]->y,
				r->neighbor[      WEST]->x,r->neighbor[      WEST]->y,
				r->neighbor[NORTH_WEST]->x,r->neighbor[NORTH_WEST]->y,
				r->neighbor[NORTH_EAST]->x,r->neighbor[NORTH_EAST]->y);
			netCon.sendMessage(str);
			if( r->neighbor[      EAST]==0 ||
				r->neighbor[SOUTH_EAST]==0 ||
				r->neighbor[SOUTH_WEST]==0 ||
				r->neighbor[      WEST]==0 ||
				r->neighbor[NORTH_WEST]==0 ||
				r->neighbor[NORTH_EAST]==0 ||
				q++>100)
			return;
			r=r->neighbor[EAST];
		}while(r!=up);
		if(up->neighbor[NORTH_EAST]->x==0)
			up=up->neighbor[NORTH_EAST];
		else
			up=up->neighbor[NORTH_WEST];
	}while(up!=prime || up->y!=0);
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
	(*land).Draw(start->x*a-a/2,start->y*(s+2*h)-(s+2*h)/2);
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
			int offset = a/2;
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
		GRRLIB_Circle(x,y, a/2+1,0xFFFFFFFF,1);
	if(n->isGround)
		GRRLIB_Circle(x, y, a/2, GREEN, 1);
	else
		GRRLIB_Circle(x, y, a/2, BLUE, 1);
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