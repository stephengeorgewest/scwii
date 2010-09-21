#include "Terrain.h"
#include <limits.h>
#include <time.h>
#include <stdlib.h>
//#define DEBUG
#include <debug.h>

#include <grrlib.h>
//http://www.lighthouse3d.com/opengl/terrain/index.php?mpd2
#define RAND_RANGE 1.0

//for debug created in SCWii.cpp
#include "NetworkConsole.h"
extern NetworkConsole netCon;
#include <new>
using namespace std;
Terrain::Terrain(int w, int h, float decay_, bool map_wrap)
{
	width = w;
	height = h;
	try
	{
	 this->map=new float*[width];
	}
	catch (std::bad_alloc& ba)
	{
	netCon.sendMessage(ba.what());
	}
	for(int i=0; i<width; i++)
	{
	try{
		this->map[i] = new float[width];
	}
	catch (std::bad_alloc& ba)
	{
	netCon.sendMessage(ba.what());
	}
#ifdef DEBUG
if(this->map[i]==0)
	netCon.sendMessage("empty");
#endif
	}
	//create power of two rectangles first
#ifdef DEBUG
char str[128];
sprintf(str,"log2(%i) =%f, int(log2(width) = %i\nlog2(%i) =%f",width,log2(width),(int)log2(width),height,log2(height));
netCon.sendMessage(str);
#endif
	int width_2=1<<((int)(log2(width)));
	int height_2=1<<((int)(log2(height)));
#ifdef DEBUG
sprintf(str,"height_2=%i, width_2=%i\nheight =%i, width = %i",height_2,width_2,height,width );
netCon.sendMessage(str);
#endif
	float decay = 1;
	
	int pow;
	if(width_2<height_2)
		pow=log2(width_2);
	else
		pow=log2(height_2);
#ifdef DEBUG
netCon.sendMessage("Entering this->TerrainPow2Square");
#endif
	this->TerrainPow2Square(pow, decay, true);//Terrain& Up, Terrain& Right, Terrain& Down, Terrain& Left);
		
	//finish off non square pow2section
	for(int y=0; y<height_2; y++)
		for(int x=width_2; x<width;x++)
			map[y][x] = 0;
	for(int y=height_2; y<height;y++)
		for(int x = 0;x<width; x++)
			map[y][x] = 0;
	/*if(width_2==width&&height_2==height)
	{
//netCon.sendMessage("copying over pointer");
//return;
		map = map_2;
	}
	else
	{

		decay*=decay_;
		float range = RAND_RANGE*decay;
		if(width-width_2!=0)
		{

		
			int number_of_vertical_insertions = height-height_2;
			//should always be greater than two.
			int vertical_gap =  height/(height-height_2);

	return;
			
			for(int i=0; i<height_2; i++)
				delete[] map_2[i];
			delete[] map_2;
		}
	}*/
}
void
Terrain::Normalize()
{
	//normalize here
netCon.sendMessage("normalizing");

	float max_,min_;
	max_=min_= (map[0][0]-min)/(max-min);
	//allocate full size map
	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
char str[64];
sprintf(str,"map[%i][%i] =",i,j );
netCon.sendMessage(str);
			float E = map[i][j] = (map[i][j]-min)/(max-min);
				if(E<min_)
					min_ = E;
				if(E>max_)
					max_ = E;
sprintf(str,"\t%f",E);
netCon.sendMessage(str);
		}
	}
	max = max_;
	min = min_;
}

void
Terrain::TerrainPow2Square(int pow, float decay_, bool wrap)//Terrain& Up, Terrain& Right, Terrain& Down, Terrain& Left)
{
#ifdef DEBUG
netCon.sendMessage("In TerrainPow2Square");
#endif
	int width_ = 1<<pow;
	int height_ = width_;
char str[128];	
	srand(time(0));
	float max_,min_;
	float r = (float)rand()/(float)RAND_MAX*(RAND_RANGE)-RAND_RANGE/2;
#ifdef DEBUG
sprintf(str,"Rand Done, =%f\nmap=%i\nmap[0]=%i",r,(int)&map[0],(int)&map[0][0]);
netCon.sendMessage(str);
#endif

	max_ = min_ = this->map[0][0] = 0 + r;
		
	int decay = 1;
#ifdef DEBUG
sprintf(str, "\n\nsquare loop pow=%i, width_=%i, r=%f",pow,width_,r);
netCon.sendMessage(str);
#endif
	for(int i=width_; i>1; i/=2)
	{
		decay*=decay_;
		float range = RAND_RANGE*decay;
		
		//square center points
		//start off center
		// width/2 ... width/4
		//and increment by twice
		// that amount to skip over
		// previously set values
		for(int j=i/2; j<width_; j+=i)
		{
			for(int k=i/2; k<width_; k+=i)
			{
				float A = map[j-i/2][k-i/2];
				float B = map[j-i/2][(k+i/2)%(width_)];
				float C = map[(j+i/2)%(width_)][k-i/2];
				float D = map[(j+i/2)%(width_)][(k+i/2)%(width_)];
				float r = (float)rand()/(float)RAND_MAX*(range)-range/2;
				float E = 
				map[j][k] = ((A + B + C + D)/4) + r;
				if(E<min_)
					min_ = E;
				if(E>max_)
					max_ = E;
#ifdef DEBUG
sprintf(str, "\n\nsquare loop i=%i, j=%i, k=%i",i,j,k);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] A = %f",j-i/2, k-i/2,A);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] B = %f",j-i/2, (k+i/2)%(width_),B);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] C = %f",(j+i/2)%(height_), k-i/2,C);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] D = %f",(j+i/2)%(height_), (k+i/2)%(width_),D);
	netCon.sendMessage(str);
sprintf(str, "[%i][%i] E = %f",(j), (k), E);
	netCon.sendMessage(str);
//return;
#endif
			}
		}
		//diamond corners
		for(int j=0; j<width_; j+=i/2)
		{
			//even rows
			for(int k = i/2; k<width_; k+=i)
			{
				float Left  = map[(j    )       ][(k-i/2)];
			//abs((j-i/2)%width_2) doesn't work
			//  with undertflow
			//i=8, j=0, k=4
			//  gives 4 instead of 12
			//overflow wrap still works with %
				int possible_wrap = (j-i/2);
				int j_i_2;
				if(possible_wrap<0)
					j_i_2 = possible_wrap+width_;
				else
					j_i_2 = possible_wrap;
				float Up    = map[ j_i_2 ][(k    )];
				float Right = map[(j    )       ][(k+i/2)%(width_)];
				float Down  = map[(j+i/2)%(width_)][(k    )];
				float r = (float)rand()/(float)RAND_MAX*(range)-range/2;
				float E = 
				map[j][k] = (Left + Up + Right + Down)/4 +r;
				if(E<min_)
					min_ = E;
				if(E>max_)
					max_ = E;
#ifdef DEBUG				
sprintf(str, "\ndiamond loop even rows i=%i, j=%i, k=%i",i,j,k);
	netCon.sendMessage(str);
sprintf(str, "[%i][%i] Left = %f",(j), (k-i/2),Left);
	netCon.sendMessage(str);
sprintf(str, "[%i][%i] Up = %f", j_i_2 , (k),Up);
	netCon.sendMessage(str);
sprintf(str, "[%i][%i] Right = %f",(j), (k+i/2)%(width_),Right);
	netCon.sendMessage(str);
sprintf(str, "[%i][%i] Down = %f",(j+i/2)%(height_), (k),Down);
	netCon.sendMessage(str);
sprintf(str, "[%i][%i] E = %f",(j), (k), E);
	netCon.sendMessage(str);
//return;
#endif
			}
			//odd rows
			j+=i/2;
			for(int k= 0; k<width_; k+=i)
			{
				int possible_wrap = (k-i/2);
				int k_i_2;
				if(possible_wrap<0)
					k_i_2 = possible_wrap+width_;
				else
					k_i_2 = possible_wrap;
				float Left  = map[(j    )       ][k_i_2];
				float Up    = map[(j-i/2)       ][(k    )];
				float Right = map[(j    )       ][(k+i/2)%width_];
				float Down  = map[(j+i/2)%width_][(k    )];
				float r = (float)rand()/(float)RAND_MAX*(range)-range/2;
				float E = 
				map[j][k] = (Left + Up + Right + Down)/4 + r;
				if(E<min_)
					min_ = E;
				if(E>max_)
					max_ = E;
					
#ifdef DEBUG				
sprintf(str, "\ndiamond loop odd rows i=%i, j=%i, k=%i",i,j,k);
	netCon.sendMessage(str);
sprintf(str, "[%i][%i] Left = %f",(j), k_i_2, Left);
	netCon.sendMessage(str);
sprintf(str, "[%i][%i] Up = %f",(j-i/2), (k),Up);
	netCon.sendMessage(str);
sprintf(str, "[%i][%i] Right = %f",(j), (k+i/2),Right);
	netCon.sendMessage(str);
sprintf(str, "[%i][%i] Down = %f",(j+i/2)%(height_), (k),Down);
	netCon.sendMessage(str);
sprintf(str, "[%i][%i] val = %f",(j), (k),E);
	netCon.sendMessage(str);
//if(i==8&&j>=12)
//return ;
#endif
			}
		}
	}
//	return map_2;
this->min=min_;
this->max=max_;
}
void
Terrain::StretchHorizontal(float**m, int current_width, int final_width)
{
netCon.sendMessage("Allocating real map");
	//fill out gaps in map
	// using midpoint 
	// not diamond
	/*float ** map_WIDE=new float*[height];
	for(int i=0; i<height_2; i++)
	{
		map_WIDE[i] = new float[width];
	}
sprintf(str,"height_2=%i, width_2=%i\nheight =%i, width = %i",height_2,width_2,height,width );
netCon.sendMessage(str);
	//should always be greater than two.
	// unless width==width_2
	int horizontal_gap = width/(width-width_2);
	for(int i=0, fill_count=0; i<width_2;i++)
	{
sprintf(str, "\n\nFill loop i=%i, gap=%i, (i #)=%i",i,horizontal_gap,(i%horizontal_gap));
netCon.sendMessage(str);
		if((i)%horizontal_gap)
		{
			
			for(int j=0; j<height_2; j++)
			{
				int up;
				if(j-1<0)
					up = j-1+height_2;
				else
					up = j-1;
				int left;
				if(i+fill_count-1<0)
					left = i+fill_count-1+width_2;
				else
					left = i+fill_count-1;
				int right = (i+fill_count+1)%width_2;
				int down = (j+1)%(height_2);
				float A = map_2[up][left];
				float B = map_2[j][left];
				float C = map_2[down][left];
				float D = map_2[up][right];
				float E = map_2[j][right];
				float F = map_2[down][right];
				float r = (float)rand()/(float)RAND_MAX*(range)-range/2;
				float G = 
					map[j][i] = (A+B+C+D+E+F)/6+r;
		//	if(G<min_)
		//		min_ = G;
		//	if(G>max_)
		//		max_ = G;
				if(j==height_2-1)
{ 
sprintf(str, "\n\nFill loop i=%i, j=%i, fill_count=%i",i,j,fill_count);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] A = %f",up, left,A);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] B = %f",j, left,B);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] C = %f",down, left,C);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] D = %f",up, right,D);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] E = %f",j, right,E);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] F = %f",down, right,F);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] G = %f",(j), (i+fill_count), G);
netCon.sendMessage(str);
}
			}
			fill_count++;
		
		}
		else//not a fill column copy and continue
		{
			netCon.sendMessage("copy column");
			for(int j=0; j<height_2; j++)
			{
				map[j][i+fill_count] = map_2[j][i];
			}
		}
	}*/
return;
}
void
Terrain::StreachVertical()
{

	for(int i=0; i<1;i++)
	{
		for(int j=0; j<1; j++)
		{
			
		}
	}
}
void
Terrain::Draw(bool display)
{

#define GRRLIB_BLUE    0x0000FF00
#define GRRLIB_GREEN   0x00FF0000
				char str[128];
	//float avg = (max+min)/2;
	for(int i=0; i<rmode->efbHeight; i++)
	{
		for(int j = 0; j<rmode->fbWidth; j++)
		{
		//scale to 0...255
			float normalized = (map[j%height][i%width]-min)*511.0/(max-min);
			if(display && j<10 && i<10)
			{
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
//				int red = 0;
				GRRLIB_Plot(j, i, val+yel);
			}
		}
	}
}


Terrain::~Terrain()
{
//char str[64];
//sprintf(str,"deleting %i",height);
//netCon.sendMessage(str);
	for(int i=0; i<this->height; i++)
	{
//sprintf(str,"deting [%i]",i);
//netCon.sendMessage(str);
		delete[] map[i];
	}
//netCon.sendMessage("delet map");
	delete[] map;
//	netCon.sendMessage("done");
}