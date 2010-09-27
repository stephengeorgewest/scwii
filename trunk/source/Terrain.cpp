#include "Terrain.h"
#include <limits.h>
#include <time.h>
#include <stdlib.h>
//#define DEBUG2
#ifdef DEBUG3
	#define DEBUG2
#endif
#ifdef DEBUG2
	#define DEBUG1
#endif
#ifdef DEBUG1
	#define DEBUG
#endif

#include <debug.h>

#include <grrlib.h>
//http://www.lighthouse3d.com/opengl/terrain/index.php?mpd2
#define RAND_RANGE 10.0

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
	 this->map=new float*[height];
	}
	catch (std::bad_alloc& ba)
	{
	netCon.sendMessage(ba.what());
	}
	for(int i=0; i<height; i++)
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
	decay = this->TerrainPow2Square(pow, decay_, true);//Terrain& Up, Terrain& Right, Terrain& Down, Terrain& Left);
		
	//finish off non square pow2section
	for(int x=width_2; x<width;x++)
	for(int y=0; y<height_2; y++)
	{
		{
			float val = (float)x-this->max*floor(float(x)/this->max);
			map[y][x] = val;
			map[y][x] = (this->max+this->min)/4;
		}
	}
	for(int y=0; y<height_2; y++)
	{
		map[y][width-1] = (this->max+this->min)/8;
	}
	this->StretchHorizontal(1<<pow, this->width, decay_,decay);
	for(int y=height_2; y<height;y++)
	for(int x = 0;x<width; x++)
	{
		{
			float val = (float)x-this->max*floor(float(x)/this->max);
			map[y][x] = val;
			map[y][x] = (this->max+this->min)/2+(this->max+this->min)/4;
		}
	}
	this->StretchVertical(1<<pow, this->height, decay_,decay);
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



float
Terrain::TerrainPow2Square(int pow, float decay_, bool wrap)//Terrain& Up, Terrain& Right, Terrain& Down, Terrain& Left)
{
#ifdef DEBUG3
netCon.sendMessage("In TerrainPow2Square");
#endif
	int width_ = 1<<pow;
#ifdef DEBUG3
char str[128];	
#endif
	srand(time(0));
	float max_,min_;
	float r = (float)rand()/(float)RAND_MAX*(RAND_RANGE)-RAND_RANGE/2;
#ifdef DEBUG3
sprintf(str,"Rand Done, =%f\nmap=%i\nmap[0]=%i",r,(int)&map[0],(int)&map[0][0]);
netCon.sendMessage(str);
#endif

	max_ = min_ = this->map[0][0] = 0 + r;
		
	float decay = 1;
#ifdef DEBUG3
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
#ifdef DEBUG3
sprintf(str, "\n\nsquare loop i=%i, j=%i, k=%i",i,j,k);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] A = %f",j-i/2, k-i/2,A);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] B = %f",j-i/2, (k+i/2)%(width_),B);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] C = %f",(j+i/2)%(width_), k-i/2,C);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] D = %f",(j+i/2)%(width_), (k+i/2)%(width_),D);
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
#ifdef DEBUG3			
sprintf(str, "\ndiamond loop even rows i=%i, j=%i, k=%i",i,j,k);
	netCon.sendMessage(str);
sprintf(str, "[%i][%i] Left = %f",(j), (k-i/2),Left);
	netCon.sendMessage(str);
sprintf(str, "[%i][%i] Up = %f", j_i_2 , (k),Up);
	netCon.sendMessage(str);
sprintf(str, "[%i][%i] Right = %f",(j), (k+i/2)%(width_),Right);
	netCon.sendMessage(str);
sprintf(str, "[%i][%i] Down = %f",(j+i/2)%(width_), (k),Down);
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
					
#ifdef DEBUG3			
sprintf(str, "\ndiamond loop odd rows i=%i, j=%i, k=%i",i,j,k);
	netCon.sendMessage(str);
sprintf(str, "[%i][%i] Left = %f",(j), k_i_2, Left);
	netCon.sendMessage(str);
sprintf(str, "[%i][%i] Up = %f",(j-i/2), (k),Up);
	netCon.sendMessage(str);
sprintf(str, "[%i][%i] Right = %f",(j), (k+i/2),Right);
	netCon.sendMessage(str);
sprintf(str, "[%i][%i] Down = %f",(j+i/2)%(width_), (k),Down);
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
	return decay;
}


float
Terrain::StretchHorizontal(int current_width_, int final_width, float decay_, float start_decay)
{
#ifdef DEBUG3
char str[128];
netCon.sendMessage("StretchHorizontal");
sprintf(str,"Current_width=%i, Final_width=%i",current_width_, final_width);
netCon.sendMessage(str);
#endif
	//fill out gaps in map
	// using midpoint 
	// not diamond
	float decay = start_decay;
	int current_width = current_width_;
	if(final_width>2*current_width)
	{
//netCon.sendMessage("start recurse");
		decay = this->StretchHorizontal(current_width, final_width/2, decay_, start_decay);
		current_width=final_width/2;
//netCon.sendMessage("end recurse");
	}
		decay*=decay_;
	float range = RAND_RANGE*decay;
	//should always be greater than two.
	// unless width==width_2
	int horizontal_gap = final_width/(final_width-current_width);
#ifdef DEBUG3
sprintf(str,"Current_width=%i, Final_width=%i, decay =%f, start_d = %f, h_gap = %i",current_width_, final_width, decay, start_decay, horizontal_gap);
netCon.sendMessage(str);
#endif
	for(int i=final_width-1, fill_count=final_width-current_width; fill_count>0;i--)
	{
#ifdef DEBUG3
sprintf(str, "\n\nFill loop i=%i, fill_c=%i, (i #)=%i",i,fill_count,(i%horizontal_gap));
netCon.sendMessage(str);
#endif
//return decay_;


		if((i)%horizontal_gap==0)
		{
#ifdef DEBUG3
sprintf(str, "Fill Column %i",i);
netCon.sendMessage(str);
#endif
			//do Square mid points
			int height_2=1<<((int)(log2(this->height)));
			for(int j=0; j<height_2; j+=2)
			{
				int up;
				if(j==0)
					up = height_2-1;
				else
					up = j-1;
				int left;
				if(i-fill_count<0)//should never happen
					left = i-fill_count+this->width;
				else
					left = i-fill_count;
				int right = (i+1)%this->width;
				int down = (j+1)%(height_2);
				float A = this->map[ up ][left ];
				float B = this->map[  j ][left ];
				float C = this->map[down][left ];
				float D = this->map[ up ][right];
				float E = this->map[  j ][right];
				float F = this->map[down][right];
				float r = (float)rand()/(float)RAND_MAX*(range)-range/2;
				float G = 
					this->map[j][i] = (A+B+C+D+E+F)/6+r;
				if(G<this->min)
					this->min = G;
				if(G>this->max)
					this->max = G;
#ifdef DEBUG3
if(/*(j>height_2-3 && i>this->width-horizontal_gap) ||*/ G>this->max || G<this->min)
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
sprintf(str, "[%i][%i] G = %f",(j), (i), G);
netCon.sendMessage(str);
}
#endif
			}
			//do Diamond mid points
			for(int j=1; j<height_2; j+=2)
			{
				int up;
				if(j==0)
					up = height_2-1;
				else
					up = j-1;
				int left;
				if(i-fill_count<0)//should never happen
					left = i-fill_count-1+this->width;
				else
					left = i-fill_count;
				int right = (i+1)%this->width;
				int down = (j+1)%(height_2);
				float A = this->map[ up ][left ];
				float B = this->map[ up ][  i  ];
				float C = this->map[ up ][right];
				float D = this->map[down][left ];
				float E = this->map[down][  i  ];
				float F = this->map[down][right];
				float r = (float)rand()/(float)RAND_MAX*(range)-range/2;
				float G = 
					this->map[j][i] = (A+B+C+D+E+F)/6+r;
				if(G<this->min)
					this->min = G;
				if(G>this->max)
					this->max = G;
#ifdef DEBUG3
if(/*(j>height_2-3 && i>this->width-horizontal_gap) ||*/ G>this->max || G<this->min)
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
#endif
			}
			fill_count--;
		
		}
		else//not a fill column copy and continue
		{
#ifdef DEBUG3
sprintf(str, "Copy Column %i", i);
netCon.sendMessage(str);
#endif
			for(int j=0; j<height; j++)
			{
				map[j%height][i] = map[j][(i-fill_count)%width];
			}
		}
	}
return decay;
}


float
Terrain::StretchVertical(int current_height_, int final_height, float decay_, float start_decay)
{
#ifdef DEBUG2
char str[128];
netCon.sendMessage("StretchVertical");
#endif
	//fill out gaps in map
	// using midpoint 
	// not diamond
	float decay = start_decay;
	int current_height = current_height_;
	if(final_height>2*current_height)
	{
		decay = this->StretchVertical(current_height, final_height/2, decay_, start_decay);
		current_height = final_height/2;
	}
		decay*=decay_;
	float range = RAND_RANGE*decay;
	//should always be greater than two.
	// unless width==width_2
	int vertical_gap = final_height/(final_height-current_height);
#ifdef DEBUG2
sprintf(str,"Current_height=%i, Final_height=%i, decay =%f, start_d = %f, h_gap = %i",current_height, final_height, decay, start_decay, vertical_gap);
netCon.sendMessage(str);
#endif
	for(int y=final_height-1, fill_count=final_height-current_height; fill_count>0;y--)
	{
#ifdef DEBUG2
sprintf(str, "\n\nFill loop y=%i, fill_c=%i, (y #)=%i",y,fill_count,(y%vertical_gap));
netCon.sendMessage(str);
#endif
//return decay_;


		if((y)%vertical_gap==0)
		{
#ifdef DEBUG2
sprintf(str, "Fill Column %i",y);
netCon.sendMessage(str);
#endif
			//do Square mid points
			//int height_2=1<<((int)(log2(this->height)));
			for(int x=0; x<this->width; x+=2)
			{
				int left;
				if(y==0)
					left = this->width-1;
				else
					left = x-1;
				int right = (x+1)%(this->width);
				int up;
				if(y-fill_count<0)
					up = y-fill_count+final_height;
				else
					up = y-fill_count;
				int down = (y+1)%final_height;
				float A = this->map[ up ][left ];
				float B = this->map[ up ][  x  ];
				float C = this->map[ up ][right];
				float D = this->map[down][left ];
				float E = this->map[down][  x  ];
				float F = this->map[down][right];
				float r = (float)rand()/(float)RAND_MAX*(range)-range/2;
				float G = 
					this->map[y][x] = (A+B+C+D+E+F)/6+r;
				if(G<this->min)
					this->min = G;
				if(G>this->max)
					this->max= G;
#ifdef DEBUG2
if((y>current_height_-vertical_gap && x>this->width-3) || G>this->max || G<this->min)
{ 
sprintf(str, "\n\nFill loop y=%i, j=%i, fill_count=%i",y,x,fill_count);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] A = %f", up,    left, A);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] B = %f", up,       x, B);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] C = %f", up,   right, C);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] D = %f", down,  left, D);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] E = %f", down,     x, E);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] F = %f", down, right, F);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] G = %f", (y),    (x), G);
netCon.sendMessage(str);
}
#endif
			}
			//do Diamond mid points
			for(int x=1; x<width; x+=2)
			{
				int left;
				if(y==0)
					left = width-1;
				else
					left = x-1;
				int right = (x+1)%(this->width);
				int up;
				if(y-fill_count<0)
					up = y-fill_count+final_height;
				else
					up = y-fill_count;
				int down = (y+1)%final_height;
				float A = this->map[ up ][left ];
				float B = this->map[ y  ][left ];
				float C = this->map[down][left ];
				float D = this->map[ up ][right];
				float E = this->map[  y ][right];
				float F = this->map[down][right];
				float r = (float)rand()/(float)RAND_MAX*(range)-range/2;
				float G = 
					this->map[y][x] = (A+B+C+D+E+F)/6+r;
				if(G<this->min)
					this->min = G;
				if(G>this->max)
					this->max= G;
#ifdef DEBUG2
if((y>current_height_-vertical_gap && x>this->width) || G>this->max || G<this->min)
{ 
sprintf(str, "\n\nFill loop x=%i, f=%i, fill_count=%i",x,y,fill_count);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] A = %f", up,          left, A);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] B = %f", y,           left, B);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] C = %f", down,        left, C);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] D = %f", up,         right, D);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] E = %f", y,          right, E);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] F = %f", down,       right, F);
netCon.sendMessage(str);
sprintf(str, "[%i][%i] G = %f", (y), x+fill_count, G);
netCon.sendMessage(str);
}
#endif
			}
			fill_count--;
		
		}
		else//not a fill column copy and continue
		{
#ifdef DEBUG2
sprintf(str, "Copy row %i", y);
netCon.sendMessage(str);
#endif
			for(int x=0; x<this->width; x++)
			{
				this->map[y%this->height][x] = this->map[(y-fill_count)%this->height][x];
			}
		}
	}
	return decay;
}
void
Terrain::Draw(bool display, float seaLevel_)
{

#define GRRLIB_BLUE    0x0000FF00
#define GRRLIB_GREEN   0x00FF0000
				char str[128];
	float seaLevel=seaLevel_;
	float avg = (max+min)/2;
	if(seaLevel_<0.0)
		seaLevel=0.0;
	if(seaLevel_>1.0)
		seaLevel=1.0;
	float mid = (max+min)*seaLevel;
	for(int i=0; i<rmode->efbHeight; i++)
	{
		for(int j = 0; j<rmode->fbWidth; j++)
		{
			if(map[i%height][j%width]< mid)
			{
				int normalized = (int)((map[i%height][j%width]-min)/(mid-min)*127.0+128.0);
				int val = ((((int)(normalized))<<8)&GRRLIB_BLUE)|0xFF;
				GRRLIB_Plot(j, i, val);
			}
			else if(true)
			{
				int normalized = (int)((map[i%height][j%width]-mid)/(max-mid)*255.0);
				int val = ((((int)(255-normalized))<<16)+GRRLIB_GREEN)|0xFF;
				int yel = ((((int)(255-normalized)/16)<<24))|0xFF;
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