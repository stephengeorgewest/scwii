#ifndef HeightMap_h
#define HeightMap_h

class Terrain
{
	public:
		int width, height;
		float max,min;
		float ** map;
		Terrain(int width, int height, float decay);
		~Terrain();
		void Draw(bool display);
};

#endif