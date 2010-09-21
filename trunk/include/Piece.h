#ifndef Piece_h
#define Piece_h

class Node;

enum pieceClass{
	ground,
	water,
	air
};
class PieceType
{
	//name
	int max_moves;
	int max_strength;
	pieceClass piece_class;
	bool canCaptureCity;
	//bomber??
	//bool isBomber/canNeutralizeCity;
	int days_to_produce;
	PieceType * strongAgainst;
	PieceType * weakAgainst;
};


class Piece
{
	public:
		Node * location;
		int strength;
		int fuel;
		PieceType type;
		int moves_remaining;
		int moves_total;
		//team
};


#endif