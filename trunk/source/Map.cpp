// for hex grid
//http://www.gamedev.net/reference/articles/article1800.asp
#include "Map.h"
#include "City.h"
#include "Node.h"

//for debug created in SCWii.cpp
#include "NetworkConsole.h"
extern NetworkConsole netCon;

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
	
	
	{
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
	}
	
	{
		// build prime meridian date line
		// map wrap removes south pole possibility
		Node * meridian[2];
		meridian[0]= prime;//heading north
		meridian[1] = prime;//heading south

		for(int i=1; i<height; i++)
		{
			meridian[0] = meridian[0]->addNeighbor(NORTH_EAST);
			meridian[1] = meridian[1]->addNeighbor(SOUTH_EAST);
			
			//fill in lattitude
			Node * current_node[2];
			current_node[0] = meridian[0]->addNeighbor(EAST);
			current_node[1] = meridian[1]->addNeighbor(EAST);
			int a=0;
			while(current_node[0]->neighbor[EAST]!=meridian[0])
			{
				if(a++>100)
				{
					netCon.sendMessage("meridian loop too much");
					break;
				}
				current_node[0] = current_node[0]->addNeighbor(EAST);
				current_node[1] = current_node[1]->addNeighbor(EAST);
			}
		}
		//attach to pole
		meridian[0]->addNeighbor(NORTH_EAST,north_pole);
		meridian[1]->addNeighbor(SOUTH_EAST,north_pole);
		
		int a=0;//debug
		Node * current_node=north_pole->addNeighbor(EAST);
		while(current_node->neighbor[EAST]!=north_pole)
		{
			if(a++>100)
			{
				netCon.sendMessage("north pole loop too much");
				break;
			}
			current_node=current_node->addNeighbor(EAST);
		}
/**/}

	prime->isGround = true;
	prime->neighbor[EAST]->isGround = true;
	prime->city = new City;
	prime->city->location=prime;
}

void
Map::DrawMap()
{

}