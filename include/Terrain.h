#ifndef HeightMap_h
#define HeightMap_h

class Terrain
{
	public:
		int width, height;
		float max, min;
		float ** map;
		Terrain(int width, int height, float decay, bool wrap);
		void TerrainPow2Square(int pow, float decay, bool wrap);//Terrain& Up, Terrain& Right, Terrain& Down, Terrain& Left);
		void StretchHorizontal(float**m, int current_width, int final_width);
		void StreachVertical();
		void Normalize();
		~Terrain();
		void Draw(bool display);
};

#endif