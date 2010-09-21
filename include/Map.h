#ifndef Map_h
#define Map_h


// for hex grid
//http://www.gamedev.net/reference/articles/article1800.asp

class Node;

class Map
{
	public:
		Map();//later int width, int height);
		void DrawMap();
	private:
		int width;
		int height;
		Node * center;
	
};

#endif