#ifndef Map_h
#define Map_h


/*
http://en.wikipedia.org/wiki/Diamond-square_algorithm
http://wiibrew.org/wiki/Map_Maker

for hex grid
http://www.gamedev.net/reference/articles/article1800.asp
*/

class Map
{
	public:
		Map(int w, int h){width = w; height = h;}
		
		//change to use moveMap(int x, int y) or such
		//to keep nodes internal
		//void SetStart();
		void DrawMap();
		void moveMap(int x, int y);
	//private:
		int width;
		int height;
	private:

	
};

#endif