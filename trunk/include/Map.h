#ifndef Map_h
#define Map_h


/*
http://en.wikipedia.org/wiki/Diamond-square_algorithm
http://wiibrew.org/wiki/Map_Maker

for hex grid
http://www.gamedev.net/reference/articles/article1800.asp
*/

class Node;

class Map
{
	public:
		Map(int width, int height);
		void DrawMap();
	//private:
		int width;
		int height;
		Node * prime;
		Node * anti_prime;
		Node * north_pole;
		Node * south_pole;
		Node * east_pole;
		Node * west_pole;
	
};

#endif