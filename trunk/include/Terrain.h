#ifndef Terrain_h
#define Terrain_h
#include "noiseutils.h"
using namespace noise;
class Terrain
{
	public:
		enum ds_type{DIAMOND, SQUARE};
		int width, height;
		float max, min;
		float ** map;
		
		utils::NoiseMap heightMap;
		utils::RendererImage renderer;
		utils::Image image;
		
		Terrain(int width, int height, float decay, bool wrap);
		float TerrainPow2Square(int pow, float decay, bool wrap);//Terrain& Up, Terrain& Right, Terrain& Down, Terrain& Left);
		float StretchHorizontal(int current_width, int final_width, float decay, float start_decay);
		float StretchVertical(int current_width, int final_width, float decay_, float start_decay);
		void Normalize();
		void diamond_square(int x, int y, int half_step, int w_gap, int v_gap, int w, int h, float range, ds_type ds);
		void RenderTerrain();
		float getAverageHeight(int x , int y, int r);
		~Terrain();
		void Draw(int start_x, int start_y);
};

#endif