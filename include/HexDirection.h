#ifndef HexDirection_H
#define HexDirection_H
//class Dir{
//public:
	enum HexDirection{
		EAST,
		SOUTH_EAST,
		SOUTH_WEST,
		WEST,
		NORTH_WEST,
		NORTH_EAST
	};
	HexDirection& operator++(HexDirection& d, int);
	HexDirection operator+(HexDirection d, int number);
	HexDirection operator-(HexDirection d, int number);
	HexDirection operator~(HexDirection dir);
//};



#endif