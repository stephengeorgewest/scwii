#include "Piece.h"

Piece::Piece(pieceClass pc, pieceType pt, City * c, int sm, int mm, int days):piece_class(pc),piece_type(pt),strength_max(sm),moves_max(mm),days_to_produce(days)
{
	//this.pieceClass=pc;
	//this.pieceType=pt;
	this->location=c->location;
	//this.strength_max=sm;
	this->strength_remaining=sm;
	//this.moves_max=mm;
	this->moves_remaining=mm;
	//this.days_to_produce=days;
}

void Piece::attack(Piece p)
{
	//risk style?
	//at most both pieces can loose one strength/attack
	//int divisions;
	if(false);//I'm stronger
	if(false);//he's stronger
	if(false);//we're the same //this.type.strongAgainst.type.id==p.type.id)
}

void Piece::move(HexDirection d)
{
	if(moves_remaining>0)
	{
		this->location=this->location->neighbor[d];
		this->moves_remaining--;
	}
}




Tank::Tank(City* c):Piece(GROUND, TANK, c, 2, 2, 4)
{
	
}
void Tank::attack(City* c)
{
	
}


Fighter::Fighter(City* c):Piece(AIR, FIGHTER, c, 1, 20, 6),fuel_max(20)
{
	this->fuel_remaining=this->fuel_max;
}

Fighter::Fighter(pieceType pc, City * c, int sm, int mm, int fm, int d):Piece(AIR, pc, c, sm, mm, d),fuel_max(30)
{
	this->fuel_remaining=this->fuel_max;
}

void Fighter::move(HexDirection d)
{
	this->Piece::move(d);
	this->fuel_remaining--;
}


Bomber::Bomber(City* c):Fighter(BOMBER, c, 1, 10, 30, 20)
{
	
}

void Bomber::bomb(HexNode * n, int r)
{
	
}


Transport::Transport(City*c):Piece(WATER, TRANSPORT, c, 3, 3, 8)
{
	
}


Destroyer::Destroyer(City* c):Piece(WATER, DESTROYER, c, 3, 4, 8)
{
	
}


Submarine::Submarine(City* c):Piece(UNDERWATER, SUBMARINE, c, 3, 3, 8)
{
	
}


Carrier::Carrier(City* c):Piece(WATER, CARRIER, c, 12, 3, 14)
{
	
}


Battleship::Battleship(City* c):Piece(WATER, BATTLESHIP, c, 18, 3, 20)
{
	
}

