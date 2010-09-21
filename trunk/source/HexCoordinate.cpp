#include "HexCoordinate.h"
//  http://homepages.inf.ed.ac.uk/rbf/CVonline/LOCAL_COPIES/AV0405/MARTIN/Hex.pdf
HexCoordinate::HexCoordinate()
{
	x=0;
	y=0;
	z=0;
}
HexCoordinate::HexCoordinate(HexCoordinate * c)
{
	x=c->x;
	y=c->y;
	z=c->z;
}
/*HexCoordinate::HexCoordinate(HexCoordinate c)
{
	x=c.x;
	y=c.y;
	z=z.z;
}*/
HexCoordinate::HexCoordinate(int x_, int y_, int z_)
{
	x=x_;
	y=y_;
	z=z_;
}
HexCoordinate
HexCoordinate::operator+(HexDirection d)
{	
	HexCoordinate co = new HexCoordinate(this->x,this->y,this->z);
	co+=d;
	return co;
}

HexCoordinate&
HexCoordinate::operator+=(HexDirection d)
{
	switch(d)
	{
		case EAST:
			this->x++;
			this->y++;
			break;
		case SOUTH_EAST:
			this->x++;
			this->z--;
			break;
		case SOUTH_WEST:
			this->y--;
			this->z--;
			break;
		case WEST:
			this->x--;
			this->y--;
			break;
		case NORTH_WEST:
			this->x--;
			this->z++;
			break;
		case NORTH_EAST:
			this->z++;
			this->y++;
			break;
	}
	return *this;
}

void
HexCoordinate::getPixelCoordinate()
{
	
}