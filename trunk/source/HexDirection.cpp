#include "HexDirection.h"
#include <iostream>

HexDirection& operator++(HexDirection& d, int) // postfix ++
{
	if ((int)d ==5)// WEST)
		return d = static_cast<HexDirection> (0);//NORTH_WEST; //rollover
	int temp = d; //convert to an int
	return d = static_cast<HexDirection> (++temp);
}

HexDirection operator+(HexDirection d, int number)
{
	int n_;
	if(number >6)
		n_ = number%6;
	else
		n_=number;
	int temp = (int)d+n_;
	HexDirection d1;
	if(temp> 5)
		return d1 = static_cast<HexDirection>(temp-6);
	else
		return d1 = static_cast<HexDirection>(temp);
}
HexDirection operator-(HexDirection d, int number)
{
	int n_;
	if(number >6)
		n_ = number%6;
	else 
		n_=number;
	int temp = (int)d-n_;
	HexDirection d1;
	if(temp< 0)
		return d1 = static_cast<HexDirection>(temp+6);
	else
		return d1 = static_cast<HexDirection>(temp);
}
HexDirection operator~(HexDirection dir)
{
	HexDirection d;
	return d = dir+3;
}
/*
ostream& operator<<(ostream& os, HexDirection d)
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
