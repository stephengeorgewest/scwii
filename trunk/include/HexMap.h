#ifndef HexMap_h
#define HexMap_h
#include "Map.h"
#include "Terrain.h"

/*
http://en.wikipedia.org/wiki/Diamond-square_algorithm
http://wiibrew.org/wiki/Map_Maker

for hex grid
http://www.gamedev.net/reference/articles/article1800.asp
*/

class HexNode;

class HexMap:public Map
{
	public:
		HexMap(int width, int height);
		~HexMap();
		//change to use moveMap(int x, int y) or such
		//to keep nodes internal
		/*void SetStart(HexNode * start);
		void DrawMap();
		void highlightNode(int x, int y);
		void moveMap(int x, int y);*/
	//private:
		int width;
		int height;
		HexNode *** Nodes;
		HexNode * prime;
		HexNode * anti_prime;
		HexNode * north_pole;
		HexNode * south_pole;
		HexNode * east_pole;
		HexNode * west_pole;
	private:
		void DrawNode(HexNode *, int , int);
		HexNode * start;
		HexNode * highlighted;
		//http://www.gamedev.net/reference/articles/article1800.asp
		float a,b,s,h;
		Terrain * land;
		void CheckMap();
};

#endif