#ifndef Node_h
#define Node_h
#include "Direction.h"

class City;

class Node
{
	public:
		Node();
		Node(int );
		Node * addNeighbor(Direction dir);
		Node * addNeighbor(Direction dir, Node * n);
		void connectNodes(Direction dir, Node* n2);
		City * city;
		//PieceList pieces;
		Node * neighbor[6];
		bool isGround;
		int ID;
};

#endif