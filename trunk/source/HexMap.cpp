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
	delete this->land;
}
HexMap::HexMap(int _width, int _height):Map( _width, _height)
{//minimum size 4x4
	
	this->height= 2*(abs(_height)+2);
	this->width = 4*(abs(_width )+1);
	this->s=10.0;
	#define PI 3.14159265358979323846264338327950288419716939937510
	this->h = sin( 2*PI*30/360 ) * this->s;
	float r = cos( 2*PI*30/360 ) * this->s;
	this->b = this->s + 2 * this->h;
	this->a = 2 * r;
	this->land = new Terrain(this->width*((int)(this->a+0.5)), this->height*((int)(this->b+0.5)),0.7,true);
	// mercator projection
	//try http://axiscity.hexamon.net/users/isomage/traveller/world/
	this->Nodes = new HexNode**[this->height];
	for(int i=0; i< this->height; i++)
	{
		this->Nodes[i]=new HexNode*[this->width];
	}
	/*
	this->prime = new HexNode(PRIME);
	this->prime->x=0;
	this->prime->y=height;
	this->east_pole = new HexNode(EAST_POLE);
	this->east_pole->x=width;
	this->east_pole->y=height;
	this->anti_prime=new HexNode(ANTI_PRIME);
	this->anti_prime->x=2*width;
	this->anti_prime->y=height;
	this->west_pole = new HexNode(WEST_POLE);
	this->west_pole->x=3*width;
	this->west_pole->y=height;
	*/
	
	
	for(int i=0; i< this->height; i++)
	{
		for(int j=0;j<this->width; j++)
		{
			if(j==0)
			{
				if(i==0)
				{
					this->north_pole = new HexNode(NORTH_POLE);
					this->south_pole = north_pole;
					this->highlighted=0;
					this->north_pole->x=0;
					this->north_pole->y=0;
					this->Nodes[0][0]=this->north_pole;
				}
				else
				{
					if(i%2==1)
						Nodes[j][i]=Nodes[j][i-1]->addNeighbor(SOUTH_EAST);
					else
						Nodes[j][i]=Nodes[j][i-1]->addNeighbor(SOUTH_WEST);
					
				}
			}
			else
			{
				Nodes[j][i]=Nodes[j-1][i]->addNeighbor(EAST);
			}
		}
		if(Nodes[width-1][i]->neighbor[EAST]==0)
			Nodes[width-1][i]->addNeighbor(EAST, Nodes[0][i]);
	}
	
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

	HexNode * up = north_pole;//prime; //
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
			sprintf(str,"%i --%i-- x=%i, y=%i, %s",(int)r, r->ID,r->x,r->y,loc);
			netCon.sendMessage(str);
			bool exit=false;
			if( r->neighbor[      EAST]==0)
			{
				netCon.sendMessage("EAST is null");
				exit = true;
			}
			if(r->neighbor[SOUTH_EAST]==0)
			{
				netCon.sendMessage("SOUTH_EAST is null");
				exit = true;
			}
			if(r->neighbor[SOUTH_WEST]==0)
			{
				netCon.sendMessage("SOUTH_WEST is null");
				exit = true;
			}
			if(r->neighbor[      WEST]==0)
			{
				netCon.sendMessage("WEST is null");
				exit = true;
			}
			if(r->neighbor[NORTH_WEST]==0)
			{
				netCon.sendMessage("North_west is null");
				exit = true;
			}
			if(r->neighbor[NORTH_EAST]==0)
			{
				netCon.sendMessage("NORTH_EAST is null");
				exit = true;
			}
			//if(q++>100 || exit)
			//	return;
			sprintf(str,"\t[ East(%i,%i), SOUTH_EAST(%i,%i), SOUTH_WEST(%i,%i), WEST(%i,%i), NORTH_WEST(%i,%i), NORTH_EAST(%i,%i)]",
				r->neighbor[      EAST]->x,r->neighbor[      EAST]->y,
				r->neighbor[SOUTH_EAST]->x,r->neighbor[SOUTH_EAST]->y,
				r->neighbor[SOUTH_WEST]->x,r->neighbor[SOUTH_WEST]->y,
				r->neighbor[      WEST]->x,r->neighbor[      WEST]->y,
				r->neighbor[NORTH_WEST]->x,r->neighbor[NORTH_WEST]->y,
				r->neighbor[NORTH_EAST]->x,r->neighbor[NORTH_EAST]->y);
			netCon.sendMessage(str);
			
			r=r->neighbor[EAST];
		}while(r!=up);
		if(up->neighbor[SOUTH_EAST]->y%2==0)
			up=up->neighbor[SOUTH_EAST];
		else
			up=up->neighbor[SOUTH_WEST];
	}while(up!=north_pole || up->y!=0);
}

//Change so that nodes are unknown to outside world
void
HexMap::SetStart(HexNode * s)
{
	start = s;
	char str[128];
	sprintf(str,"set Start %i, %i",(int)s->x,s->y);
	netCon.sendMessage(str);
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