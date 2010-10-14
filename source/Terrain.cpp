#include "Terrain.h"
#include <limits.h>
#include <time.h>
#include <stdlib.h>

#ifndef OSX
#include <grrlib.h>
#include "NetworkConsole.h"
extern NetworkConsole netCon;
#endif
//http://www.lighthouse3d.com/opengl/terrain/index.php?mpd2
#include <new>
using namespace std;
#define RAND_RANGE 10.0

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
	
	//for(int x=0; x<this->width; x++)
	//for(int y=0; y<this->height; y++)
	//	map[y][x] =  NAN;
	
	
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
	this->StretchHorizontal(1<<pow, this->width, decay_,decay*decay_);
	this->StretchVertical(1<<pow, this->height, decay_,decay*decay_*decay_);
	this->Normalize();
	this->RenderTerrain();
}
void
Terrain::Normalize()
{
	float max_,min_;
	max_=min_= (this->map[0][0]-this->min)/(this->max-this->min);
	for(int y=0; y<this->height; y++)
	{
		for(int x=0; x<this->width; x++)
		{
			float E = this->map[y][x] = (this->map[y][x]-this->min)/(this->max-this->min);
			//if(E==NAN)
			//	min_=max_=NAN;
			if(E<min_)
					min_ = E;
				if(E>max_)
					max_ = E;
		}
	}
	this->max = max_;
	this->min = min_;
}


void Terrain::diamond_square(int x, int y, int half_step,
		int h_gap, int v_gap,
		int w, int h,
		float range, ds_type ds)
{
	if(x<0 || x>=w || y<0 || y>=h)
		return;
	
	
	int left = x-half_step-h_gap;
	if(left<0)
		left += w;
		
	int right = x+half_step;
	if(right >=w)
		right -= w;
		
	int up = y-half_step-v_gap;
	if(up<0)
		up += h;
	
	int down = y+half_step;
	if(down>=h)
		down -= h;
		
	float A,B,C,D;
	float r = (float)rand()/(float)RAND_MAX*(range)-range/2;
	if(ds==SQUARE)
	{
		A = this->map[up  ][left];
		B = this->map[up  ][right];
		C = this->map[down][right];
		D = this->map[down][left];
	}
	else
	{
		A = this->map[up  ][x    ];
		B = this->map[y   ][right];
		C = this->map[down][x    ];
		D = this->map[y   ][left ];
	}
	float E = 
		this->map[y][x] = ((A + B + C + D)/4) + r;
	if(E<this->min)
		this->min= E;
	if(E>this->max)
		this->max = E;
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
		for(int y=i/2; y<width_; y+=i)
		{
			for(int x=i/2; x<width_; x+=i)
			{
				diamond_square( x, y, i/2, 0, 0, width_,  width_, range, SQUARE);
			}
		}
		//??
		decay*=decay_;
		//diamond corners
		for(int y=0; y<width_; y+=i/2)
		{
			//even rows
			for(int x = i/2; x<width_; x+=i)
			{
				diamond_square( x, y, i/2, 0, 0, width_, width_, range, DIAMOND);
			}
			//odd rows
			y+=i/2;
			for(int x= 0; x<width_; x+=i)
			{
				diamond_square( x, y, i/2, 0, 0, width_, width_, range, DIAMOND);
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
		decay = this->StretchHorizontal(current_width, (final_width+1)/2, decay_, start_decay);
		current_width=(final_width+1)/2;
	}
		decay*=decay_;
	float range = RAND_RANGE*decay;
	//should always be greater than two.
	// unless width==width_2
	float horizontal_gap = final_width/(final_width-current_width);
	float count = horizontal_gap;
	char str[128];
	for(int x=final_width-1; final_width-current_width>0;x--)
	{
		if(count>=horizontal_gap)
		{
			count-=horizontal_gap;
			//do Square mid points
			int height_2=1<<((int)(log2(this->height)));
			for(int y=0; y<height_2; y+=2)
			{
				diamond_square( x, y, 1, final_width-current_width, 0, final_width,  height_2, range, SQUARE);
			}
			//do Diamond mid points
			for(int y=1; y<height_2; y+=2)
			{
				diamond_square( x, y, 1, final_width-current_width, 0, final_width,  height_2, range, DIAMOND);
			}
			current_width++;
		}
		else//not a fill column copy and continue
		{
			for(int y=0; y<height; y++)
			{
				map[y%height][x] = map[y][(x-(final_width-current_width))%width];
			}
		}
		count++;
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
		decay = this->StretchVertical(current_height, (final_height+1)/2, decay_, start_decay);
		current_height = (final_height+1)/2;
	}
		decay*=decay_;
	float range = RAND_RANGE*decay;
	//should always be greater than two.
	// unless width==width_2
	float vertical_gap = final_height/(final_height-current_height);
	float count = vertical_gap;
	for(int y=final_height-1; final_height-current_height>0;y--)
	{
		if(count>vertical_gap)
		{
			count-=vertical_gap;
			//do Square mid points
			//int height_2=1<<((int)(log2(this->height)));
			for(int x=0; x<this->width; x+=2)
			{
				diamond_square( x, y, 1, 0, final_height-current_height, this->width, final_height, range, SQUARE);
			}
			//do Diamond mid points
			for(int x=1; x<this->width; x+=2)
			{
				diamond_square( x, y, 1, 0, final_height-current_height, this->width, final_height, range, DIAMOND);
			}
			current_height++;
		}
		else//not a fill column copy and continue
		{
			for(int x=0; x<this->width; x++)
			{
				this->map[y%this->height][x] = this->map[(y-(final_height-current_height))%this->height][x];
			}
		}
		count++;
	}
	return decay;
}
void
Terrain::RenderTerrain()
{
	
	heightMap.SetSize(this->width, this->height);
	for(int x=0; x<this->width; x++)
	for(int y=0; y<this->height; y++)
	{
		heightMap.SetValue(x,y,this->map[y][x]);
	}
	renderer.SetSourceNoiseMap (heightMap);
	renderer.SetDestImage (image);
	renderer.ClearGradient ();
	//http://www.allscoop.com/tools/Web-Colors/web-colors.php?abc=%23bbbbff
	renderer.AddGradientPoint ( 0.0000, utils::Color (0x00, 0x00, 0x55, 255)); // deeps
	renderer.AddGradientPoint ( 0.4000, utils::Color (0x11, 0x00, 0x77, 255)); // deeps
	renderer.AddGradientPoint ( 0.5000, utils::Color (0x99, 0xcc, 0xff, 255)); // shallow
	renderer.AddGradientPoint ( 0.5500, utils::Color (0x00, 0x7f, 0xff, 255)); // shore
	renderer.AddGradientPoint ( 0.5700, utils::Color (0xff, 0xff, 0xdd, 255)); // sand
	renderer.AddGradientPoint ( 0.6000, utils::Color (0x33, 0x99, 0x00, 255)); // grass
	renderer.AddGradientPoint ( 0.7000, utils::Color (0x00, 0x77, 0x00, 255)); // forest
	renderer.AddGradientPoint ( 0.8900, utils::Color (0x44, 0x44, 0x00, 255)); // dirt
	renderer.AddGradientPoint ( 0.9500, utils::Color (0xaa, 0xaa, 0xaa, 255)); // rock
	renderer.AddGradientPoint ( 0.9800, utils::Color (0xff, 0xff, 0xff, 255)); // snow
	renderer.EnableLight ();
	renderer.SetLightContrast (3.0); // Triple the contrast
	renderer.EnableWrap (true);
	renderer.SetLightBrightness (2.0); // Double the brightness
	//renderer.SetLightAzimuth();
	//renderer.SetLightElevation();
	//renderer.SetLightColor();
	renderer.Render ();
}
void
Terrain::Draw(int start_x, int start_y)
{

#define GRRLIB_BLUE    0x0000FF00
#define GRRLIB_GREEN   0x00FF0000
#ifdef OSX
	int w = this->width;
	int h = this->height;
#else
	int w = rmode->fbWidth;
	int h = rmode->efbHeight;
#endif


	for(int y=0; y<h; y++)
	{
		for(int x = 0; x<w; x++)
		{
		
			int x_=x+start_x;
			while(x_ >= this->width)
				x_-=this->width;
			while(x_ < 0)
				x_+=this->width;
				
				
			int y_=y+start_y;
			while(y_ >= this->height)
				y_-=this->height;
			while(y_ < 0)
				y_+=this->height;
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
//

float Terrain::getAverageHeight(int x_ , int y_, int r_)
{
	// square aproximation for now
	float avg=0;
	for(int x = x_-r_; x<x_+r_; x++)
	for(int y = y_+-r_; y<y_+r_; y++)
	{
		int x_wrap=x;
		while(x_wrap<0)
			x+=this->width;
		while(x_wrap>=this->width)
			x-=this->width;
		
		int y_wrap=y;
		while(y_wrap<0)
			y+=this->height;
		while(y_wrap>=this->height)
			y-=this->height;
			
		avg+=map[y_wrap][x_wrap];
	}
	avg/=r_*r_;
	return avg;
}
//


Terrain::~Terrain()
{
	//image.~Image();
	//heightMap.~NoiseMap();
	//renderer.~RendererImage();
	for(int i=0; i<this->height; i++)
	{
		delete[] map[i];
	}
	delete[] map;
}