#include "Direction.h"
#include <iostream>

Direction& operator++(Direction& d, int) // postfix ++
{
	if (d == WEST)
		return d = NORTH_WEST; //rollover
	int temp = d; //convert to an int
	return d = static_cast<Direction> (++temp);
}

Direction operator+(Direction d, int number)
{
	int n_;
	if(number >6)
		n_ = number%6;
	else
		n_=number;
	int temp = (int)d+n_;
	Direction d1;
	if(temp> 5)
		return d1 = static_cast<Direction>(temp-6);
	else
		return d1 = static_cast<Direction>(temp);
}
Direction operator-(Direction d, int number)
{
	int n_;
	if(number >6)
		n_ = number%6;
	else 
		n_=number;
	int temp = (int)d-n_;
	Direction d1;
	if(temp< 0)
		return d1 = static_cast<Direction>(temp+6);
	else
		return d1 = static_cast<Direction>(temp);
}
Direction operator~(Direction dir)
{
	Direction d;
	return d = dir+3;
}
/*
ostream& operator<<(ostream& os, Direction d)
{
		char* s;
		switch (d) {
				case EAST:
						s = "EAST";
						break;
				case SOUTH_EAST:
						s = "SOUTH_EAST";
						break;
				case SOUTH_WEST:
						s = "SOUTH_WEST";
						break;
				case WEST:
						s = "WEST";
						break;
				case NORTH_WEST:
						s = "NORTH_WEST";
						break;
				case NORTH_EAST:
						s = "NORTH_EAST";
						break;
				default:
						s = "badvalue";
						break;
		}
		return os << s;
}*/
