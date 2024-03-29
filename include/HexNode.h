#ifndef HexNode_h
#define HexNode_h
#include "HexDirection.h"
#include "HexGeographicLocation.h"

class City;

class HexNode
{
	friend ostream& operator<<(ostream& output, const HexNode* n);
	public:
		HexNode();
		HexNode(GeographicLocation );
		HexNode(int );
		~HexNode();
		HexNode * addNeighbor(HexDirection dir);
		HexNode * addNeighbor(HexDirection dir, HexNode * n);
		void connectNodes(HexDirection dir, HexNode* n2);
		City * city;
		//PieceList pieces;
		HexNode * neighbor[6];
		bool isGround;
		int ID;
		GeographicLocation location;
		int x,y;
};

#endif