#include "Draw.h"

#include <grrlib.h>
void
Draw::DrawTerrain(Terrain* t, int start_x, int start_y)
{

#define GRRLIB_BLUE    0x0000FF00
#define GRRLIB_GREEN   0x00FF0000
#ifdef OSX
	int w = t->width;
	int h = t->height;
#else
	int w = rmode->fbWidth;
	int h = rmode->efbHeight;
#endif


	for(int y=0; y<h; y++)
	{
		for(int x = 0; x<w; x++)
		{
		
			int x_=x+start_x;
			while(x_ >= t->width)
				x_-=t->width;
			while(x_ < 0)
				x_+=t->width;
				
				
			int y_=y+start_y;
			while(y_ >= t->height)
				y_-=t->height;
			while(y_ < 0)
				y_+=t->height;
#ifndef OSX
			utils::Color c = image.GetValue(x_,y_);
			GRRLIB_Plot(x, y, c.red<<24 | c.green<<16 | c.blue<<8 | c.alpha);
			/*float num = this->map[y_][x_];
			if(num == NAN)
			{
				GRRLIB_Plot(x,y,0xff0000FF);
			}
			else
			{
				int val = (num)*255.0;
				GRRLIB_Plot(x,y,val<<24|val<<16|val<<8|0xff);
			}*/
#endif
		}
	}
}
void
Draw::DrawMap(Terrain *t )
{
	
	Draw.DrawTerrain(t,start->x*a-a/2,start->y*(s+2*h)-(s+2*h)/2);
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
Draw::DrawNode(HexNode * n, int x, int y)
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
