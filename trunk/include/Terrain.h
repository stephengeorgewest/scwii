#ifndef HeightMap_h
#define HeightMap_h

class Terrain
{
	public:
		int width, height;
		float max, min;
		float ** map;
		Terrain(int width, int height, float decay, bool wrap);
		float TerrainPow2Square(int pow, float decay, bool wrap);//Terrain& Up, Terrain& Right, Terrain& Down, Terrain& Left);
		float StretchHorizontal(int current_width, int final_width, float decay, float start_decay);
		float StretchVertical(int current_width, int final_width, float decay_, float start_decay);
		void Normalize();
		~Terrain();
		void Draw(bool display, float seaLevel);
};

#endif