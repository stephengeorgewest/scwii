#include "Terrain.h"
#include <limits.h>
#include <time.h>
#include <stdlib.h>

#include <debug.h>

#include <grrlib.h>
//http://www.lighthouse3d.com/opengl/terrain/index.php?mpd2
#define RAND_RANGE 10
#define HEIGHT_MAX 256


//for debug created in SCWii.cpp
#include "NetworkConsole.h"
extern NetworkConsole netCon;

Terrain::Terrain(int w, int h, float decay_)
{
	width=256;//w;
	height=256;//h;
	max = min = 0;
	float decay = 1;
	
	map=new float*[height];
	for(int i=0; i<height; i++)
	{
		map[i] = new float[width];
	}
//netCon.sendMessage("Height Map Allocated");
//int a=0;

	//initialize coorners
	//int A = map[0][0]=256/2;//INT_MAX/2;
	//int B = map[0][height-1]=256/2;
	//int C = map[Width-1][0]=256/2;
	//int D = map[Width-1][height-1]=256/2;
	srand(time(0));
	
	map[0][0] = 0 + (rand()%RAND_RANGE-RAND_RANGE/2);
	
	//unnecessary.. map wrap?
	//map[0][height-1] = map[0][0];
	//map[width-1][0] = map[0][0];
	//map[width-1][height-1] = map[0][0];
	
	
	/*int e = map[width/2][height/2]=(a + b + c + d)/4+rand()%(RAND_RANGE/2)-RAND_RANGE;
	
	int f = h = map[width/2][height/2]=(a + c + e + e)/4+rand()%(RAND_RANGE/2)-RAND_RANGE;
	int g = i = map[width/2][height/2]=(a + b + e + e)/4+rand()%(RAND_RANGE/2)-RAND_RANGE;
	//int h = map[width/2][height/2]=(b + d + c + d)/4+rand()%(RAND_RANGE/2)-RAND_RANGE;
	//int i = map[width/2][height/2]=(c + d + c + d)/4+rand()%(RAND_RANGE/2)-RAND_RANGE;
_break();*/

	for(int i=width; i>1; i/=2)
	{
		decay*=decay_;
		float range = RAND_RANGE*decay;
//		char str[128];
		//square center points
		//start off center
		// width/2 ... width/4
		//and increment by twice
		// that amount to skip over
		// previously set values
		for(int j=i/2; j<height; j+=i)
		{
			for(int k=i/2; k<width; k+=i)
			{
//sprintf(str, "square loop i=%i, j=%i, k=%i\n",i,j,k);
//netCon.sendMessage(str);
//			if((j-i/2)>=height)
//			{
//				sprintf(str, " wrap around %i %i",j-i/2,(j-i/2)%(height));
//				netCon.sendMessage(str);
//			}
				float A = map[j-i/2][k-i/2];
//	sprintf(str, "[%i][%i] A = %i",j-i/2, k-i/2,A);
//	netCon.sendMessage(str);
				float B = map[j-i/2][(k+i/2)%(width)];
//	sprintf(str, "[%i][%i] B = %i",j-i/2, (k+i/2)%(width),B);
//	netCon.sendMessage(str);
		
				float C = map[(j+i/2)%(height)][k-i/2];
//	sprintf(str, "[%i][%i] C = %i",(j+i/2)%(height), k-i/2,C);
//	netCon.sendMessage(str);
				float D = map[(j+i/2)%(height)][(k+i/2)%(width)];
//	sprintf(str, "[%i][%i] D = %i",(j+i/2)%(height), (k+i/2)%(width),D);
//	netCon.sendMessage(str);
				float r = (float)rand()/(float)RAND_MAX*(range)-range/2;
				float E = 
				map[j][k] = ((A + B + C + D)/4) + r;
				if(E<min)
					min = E;
				if(E>max)
					max = E;
//sprintf(str, "[%i][%i] E = %i",(j), (k), E);
//netCon.sendMessage(str);
			//return;
		//Draw();
		//GRRLIB_Render();
		//VIDEO_WaitVSync();
//if(a++>1000)
//{
//	netCon.sendMessage("error in square");
//	break;
//}
//			netCon.sendMessage("squareloop end");
							
			}
		}
		//diamond corners
		for(int j=0; j<height; j+=i/2)
		{
			//even rows
			for(int k = i/2; k<width; k+=i)
			{
//sprintf(str, "diamond loop even rows i=%i, j=%i, k=%i\n",i,j,k);
//netCon.sendMessage(str);
		
				
				float Left  = map[(j    )       ][(k-i/2)];
//sprintf(str, "[%i][%i] Left = %i",(j), (k-i/2),Left);
//netCon.sendMessage(str);
			//abs((j-i/2)%width) doesn't work
			//i=8, j=0, k=4
			//gives 4 instead of 12
			//overflow wrap still works with %
				int possible_wrap = (j-i/2);
				int j_i_2;
				if(possible_wrap<0)
					j_i_2 = possible_wrap+height;
				else
					j_i_2 = possible_wrap;
				float Up    = map[ j_i_2 ][(k    )];
//sprintf(str, "[%i][%i] Up = %i", j_i_2 , (k),Up);
//netCon.sendMessage(str);
			
				float Right = map[(j    )       ][(k+i/2)%(width)];
//sprintf(str, "[%i][%i] Right = %i",(j), (k+i/2)%(width),Right);
//netCon.sendMessage(str);
			
				float Down  = map[(j+i/2)%(height)][(k    )];//%height taken care of in if above
//sprintf(str, "[%i][%i] Down = %i",(j+i/2)%(height), (k),Down);
//netCon.sendMessage(str);
				
				float r = (float)rand()/(float)RAND_MAX*(range)-range/2;
				float E = 
				map[j][k] = (Left + Up + Right + Down)/4 +r;
				if(E<min)
					min = E;
				if(E>max)
					max = E;
//sprintf(str, "[%i][%i] E = %i",(j), (k), E);
//netCon.sendMessage(str);
			}
			//netCon.sendMessage("Diamond Even End Odd Start");
//a=0;
			//odd rows
			j+=i/2;
			for(int k= 0; k<width; k+=i)
			{
//sprintf(str, "diamond loop odd rows i=%i, j=%i, k=%i\n",i,j,k);
//netCon.sendMessage(str);
//if(a++>1000)
//{
//	netCon.sendMessage("error in diamond");
//	break;
//}
				int possible_wrap = (k-i/2);
				int k_i_2;
				if(possible_wrap<0)
					k_i_2 = possible_wrap+width;
				else
					k_i_2 = possible_wrap;
				float Left  = map[(j    )       ][k_i_2];
//sprintf(str, "[%i][%i] Left = %i",(j), k_i_2, Left);
//netCon.sendMessage(str);
			
				float Up    = map[(j-i/2)       ][(k    )];
//sprintf(str, "[%i][%i] Up = %i",(j-i/2), (k),Up);
//netCon.sendMessage(str);
			
				float Right = map[(j    )       ][(k+i/2)%width];
//sprintf(str, "[%i][%i] Right = %i",(j), (k+i/2),Right);
//netCon.sendMessage(str);
			
				float Down  = map[(j+i/2)%height][(k    )];
//sprintf(str, "[%i][%i] Down = %i",(j+i/2)%(height), (k),Down);
//netCon.sendMessage(str);
				float r = (float)rand()/(float)RAND_MAX*(range)-range/2;
				float E = 
				map[j][k] = (Left + Up + Right + Down)/4 + r;
				if(E<min)
					min = E;
				if(E>max)
					max = E;
//sprintf(str, "[%i][%i] val = %i",(j), (k),E);
//netCon.sendMessage(str);
//if(i==8&&j>=12)
//	return ;
			}
		}
	}
	char str[128];
	sprintf(str, "max = %f, min = %f", max, min);
	netCon.sendMessage(str);
}
	
Terrain::~Terrain()
{
	for(int i=0; i<height; i++)
		delete[] map[i];
	
	delete[] map;
}
void
Terrain::Draw(bool display)
{

#define GRRLIB_BLUE    0x0000FF00
#define GRRLIB_GREEN   0x00FF0000
	//float avg = (max+min)/2;
	for(int i=0; i<rmode->efbHeight; i++)
	{
		for(int j = 0; j<rmode->fbWidth; j++)
		{
		//scale to 0...255
			float normalized = (map[j%height][i%width]-min)*511.0/(max-min);
			if(display)
			{
				char str[128];
				sprintf(str, "value at %i,%i = %f", j%height, i%width, normalized);
				netCon.sendMessage(str);
			}
			if(normalized < 255)
			{
				int val = ((((int)(normalized))<<8)&GRRLIB_BLUE)|0xFF;
				GRRLIB_Plot(j, i, val);
			}
			else
			{
				int val = ((((int)(511-normalized))<<16)+GRRLIB_GREEN)|0xFF;
				int yel = ((((int)(511-normalized)/16)<<24))|0xFF;
				int red = 0;
				GRRLIB_Plot(j, i, val+yel);
			}
		}
	}
}