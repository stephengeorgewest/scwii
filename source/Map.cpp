// for hex grid
//http://www.gamedev.net/reference/articles/article1800.asp
#include "Map.h"
#include "City.h"
#include "Node.h"

//widthx4,height x2
Map::Map(int width, int height)
{
	// mercator projection
	prime = new Node;
	east_pole = new Node;
	anti_prime=new Node;
	west_pole = new Node;
	
	north_pole = new Node;
	south_pole = north_pole;
	 
	 
	 
	Node * current_node[4];
	current_node[0]= prime;
	current_node[1] = east_pole;
	current_node[2] = anti_prime;
	current_node[3] = west_pole;
	//build equator
	for(int i=1; i<width; i++)
	{
		//    *     *     *
		// *     *     *
		// -> 1 <-> 2 <-> 3 <-
		for(int j=0; j<4; j++)
		{
			//current_node[j]->neighbor[EAST] = new Node;
			//current_node[j]->neighbor[EAST]->neighbor[WEST]=current_node[j];
			//current_node[j]=current_node[j]->neighbor[EAST];
			current_node[j]=current_node[j]->addNeighbor(EAST);
		}
	}
	//finialize edge
		//width=1 [3] == west pole
	prime->addNeighbor(WEST,current_node[3]);
		//width=1 [0] == prime
	east_pole->addNeighbor(WEST,current_node[0]);
		//width=1 [1] == EAST pole
	anti_prime->addNeighbor(WEST,current_node[1]);
		//width=1 [2] == anti_prime
	west_pole->addNeighbor(WEST,current_node[2]);
	
	

	// build prime meridian date line
	// map wrap removes south pole possibility
	current_node[0]= prime;//heading north
	current_node[1] = prime;//heading south

	for(int i=1; i<height; i++)
	{
		current_node[0] = current_node[0]->addNeighbor(NORTH_EAST);
		current_node[1] = current_node[1]->addNeighbor(SOUTH_EAST);
	}
	//attach to pole
	current_node[0]->connectNodes(NORTH_EAST,north_pole);
	current_node[1]->connectNodes(SOUTH_EAST,north_pole);
/**/

	prime->isGround = true;
	prime->neighbor[EAST]->isGround = true;
	prime->city = new City;
	prime->city->location=prime;
}

void
Map::DrawMap()
{

}