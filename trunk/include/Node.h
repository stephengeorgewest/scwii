#ifndef Node_h
#define Node_h


class City;


class Node
{
	public:
		City * city;
		//PieceList pieces;
		Node * up_left;
		Node * up_right;
		Node * right;
		Node * down_right;
		Node * down_left;
		Node * left;
		bool isGround;
};

#endif