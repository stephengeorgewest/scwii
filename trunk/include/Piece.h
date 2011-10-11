#ifndef Piece_h
#define Piece_h
#include "HexDirection.h"
#include "HexNode.h"
#include "City.h"

enum pieceClass{
	UNDERWATER,
	WATER,
	GROUND,
	AIR
};
enum pieceType{TANK,FIGHTER,BOMBER,TRANSPORT,DESTROYER,SUBMARINE,CARRIER,BATTLESHIP};

class Piece
{
	public:
		Piece(pieceClass pc, pieceType pt, City * l, int sm, int mm, int days);
		void attack(Piece p);
		void move(HexDirection d);
	//private:
		const pieceClass piece_class;
		const pieceType piece_type;
		HexNode * location;
		const int strength_max;
		int strength_remaining;
		const int moves_max;
		int moves_remaining;
		const int days_to_produce;
		//pieceType * strongAgainst;
		//pieceType * weakAgainst;
		//team
};

class Tank:public Piece{
public:
	Tank(City* );
	void attack(City * );
};

class Fighter:public Piece{
public:
	Fighter(City* );
	Fighter(pieceType, City*, int, int, int, int);
	void move(HexDirection);
	const int fuel_max;
	int fuel_remaining;
};

class Bomber:public Fighter{
public:
	Bomber(City*);
	void bomb(HexNode * n, int r);
};

class Transport:public Piece{
public:
	Transport(City*);
	static const int max_tanks = 8;
};

class Destroyer:public Piece{
public:
	Destroyer(City* );
};

class Submarine:public Piece{
public:
	Submarine(City* );
};

class Carrier:public Piece{
public:
	Carrier(City* );

};

class Battleship:public Piece{
public:
	Battleship(City* );
};



#endif