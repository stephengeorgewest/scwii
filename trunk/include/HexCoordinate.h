#ifndef HexCoordinate_h
#define HexCoordinate_h

#include "HexDirection.h"

class HexCoordinate{
	public:
		int x;
		int y;
		int z;
		HexCoordinate();
		HexCoordinate(int x_, int y_, int Z_);
		//HexCoordinate(HexCoordinate c);
		HexCoordinate(HexCoordinate * c);
		HexCoordinate operator+( HexDirection d);
		HexCoordinate& operator+=(HexDirection d);
		
		void getPixelCoordinate();
};

#endif