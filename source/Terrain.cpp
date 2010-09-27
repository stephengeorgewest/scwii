#include "Terrain.h"
#include <limits.h>
#include <time.h>
#include <stdlib.h>

#include <grrlib.h>
//http://www.lighthouse3d.com/opengl/terrain/index.php?mpd2
#define RAND_RANGE 10.0

#include <new>
using namespace std;
Terrain::Terrain(int w, int h, float decay_, bool map_wrap)
{
	this->width = w;
	this->height = h;
	try
	{
		this->map=new float*[this->height];
	}
	catch (std::bad_alloc& ba)
	{
		//ba.what();
	}
	for(int i=0; i<height; i++)
	{
		try
		{
			this->map[i] = new float[this->width];
		}
		catch (std::bad_alloc& ba)
		{
			//ba.what();
		}
	}
	//create power of two rectangles first
	int width_2=1<<((int)(log2(this->width)));
	int height_2=1<<((int)(log2(this->height)));
	float decay = 1;
	
	int pow;
	if(width_2<height_2)
		pow=log2(width_2);
	else
		pow=log2(height_2);
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
	this->StretchVertical(1<<pow, this->height, decay_,decay);
}
void
Terrain::Normalize()
{
	float max_,min_;
	max_=min_= (map[0][0]-min)/(max-min);
	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			float E = map[i][j] = (map[i][j]-min)/(max-min);
				if(E<min_)
					min_ = E;
				if(E>max_)
					max_ = E;
		}
	}
	this->max = max_;
	this->min = min_;
}



float
Terrain::TerrainPow2Square(int pow, float decay_, bool wrap)//Terrain& Up, Terrain& Right, Terrain& Down, Terrain& Left)
{
	int width_ = 1<<pow;
	srand(time(0));
	float r = (float)rand()/(float)RAND_MAX*(RAND_RANGE)-RAND_RANGE/2;

	this->max = this->min = this->map[0][0] = 0 + r;
		
	float decay = 1;
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
				if(E<this->min)
					this->min= E;
				if(E>this->max)
					this->max = E;
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
				if(E<this->min)
					this->min = E;
				if(E>this->max)
					this->max = E;
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
				if(E<this->min)
					this->min = E;
				if(E>this->max)
					this->max = E;
			}
		}
	}
	return decay;
}


float
Terrain::StretchHorizontal(int current_width_, int final_width, float decay_, float start_decay)
{//run before stretchVertical
	//fill out gaps in map
	// using midpoint and 
	// diamond modification
	float decay = start_decay;
	int current_width = current_width_;
	if(final_width>2*current_width)
	{
		decay = this->StretchHorizontal(current_width, final_width/2, decay_, start_decay);
		current_width=final_width/2;
	}
		decay*=decay_;
	float range = RAND_RANGE*decay;
	//should always be greater than two.
	// unless width==width_2
	int horizontal_gap = final_width/(final_width-current_width);
	for(int x=final_width-1, fill_count=final_width-current_width; fill_count>0;x--)
	{
		if((x)%horizontal_gap==0)
		{
			//do Square mid points
			int height_2=1<<((int)(log2(this->height)));
			for(int y=0; y<height_2; y+=2)
			{
				int up;
				if(y==0)
					up = height_2-1;
				else
					up = y-1;
				int left;
				if(x-fill_count<0)//should never happen
					left = x-fill_count+this->width;
				else
					left = x-fill_count;
				int right = (x+1)%this->width;
				int down = (y+1)%(height_2);
				float A = this->map[ up ][left ];
				float B = this->map[  y ][left ];
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
					this->max = G;
			}
			//do Diamond mid points
			for(int y=1; y<height_2; y+=2)
			{
				int up;
				if(y==0)
					up = height_2-1;
				else
					up = y-1;
				int left;
				if(x-fill_count<0)//should never happen
					left = x-fill_count+this->width;
				else
					left = x-fill_count;
				int right = (x+1)%this->width;
				int down = (y+1)%(height_2);
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
					this->max = G;
			}
			fill_count--;
		
		}
		else//not a fill column copy and continue
		{
			for(int y=0; y<height; y++)
			{
				map[y%height][x] = map[y][(x-fill_count)%width];
			}
		}
	}
return decay;
}


float
Terrain::StretchVertical(int current_height_, int final_height, float decay_, float start_decay)
{
	//use after stretchHorizontal
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
	for(int y=final_height-1, fill_count=final_height-current_height; fill_count>0;y--)
	{
		if((y)%vertical_gap==0)
		{
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
			}
			fill_count--;
		
		}
		else//not a fill column copy and continue
		{
			for(int x=0; x<this->width; x++)
			{
				this->map[y%this->height][x] = this->map[(y-fill_count)%this->height][x];
			}
		}
	}
	return decay;
}
void
Terrain::Draw(float seaLevel_)
{

#define GRRLIB_BLUE    0x0000FF00
#define GRRLIB_GREEN   0x00FF0000
	float seaLevel=seaLevel_;
	//float avg = (max+min)/2;
	if(seaLevel_<0.0)
		seaLevel=0.0;
	if(seaLevel_>1.0)
		seaLevel=1.0;
	float mid = (max+min)*seaLevel;
	for(int y=0; y<rmode->efbHeight; y++)
	{
		for(int x = 0; x<rmode->fbWidth; x++)
		{
			if(map[y%height][x%width]< mid)
			{
				int normalized = (int)((map[y%height][x%width]-min)/(mid-min)*127.0+128.0);
				int val = ((((int)(normalized))<<8)&GRRLIB_BLUE)|0xFF;
				GRRLIB_Plot(x, y, val);
			}
			else if(true)
			{
				int normalized = (int)((map[y%height][x%width]-mid)/(max-mid)*255.0);
				int val = ((((int)(255-normalized))<<16)+GRRLIB_GREEN)|0xFF;
				int yel = ((((int)(255-normalized)/16)<<24))|0xFF;
				//int red = 0;
				GRRLIB_Plot(x, y, val+yel);
			}
		}
	}
}


Terrain::~Terrain()
{
	for(int i=0; i<this->height; i++)
	{
		delete[] map[i];
	}
	delete[] map;
}