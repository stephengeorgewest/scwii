// for hex grid
//http://www.gamedev.net/reference/articles/article1800.asp
#include "Map.h"
#include "City.h"
#include "Node.h"

Map::Map()//later  int width, int height)
{
	center = new Node;
	center->right = new Node;
	center->down_right = new Node;
	center->down_left = new Node;
	center->left = new Node;
	center->up_left = new Node;
	center->up_right = new Node;
	
	center->isGround = true;
	center->right->isGround = true;
	center->city = new City;
	center->city->location=center;
}

void
Map::DrawMap()
{

}