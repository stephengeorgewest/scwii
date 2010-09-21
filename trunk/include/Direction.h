#ifndef Direction_H
#define Direction_H
//class Dir{
//public:
	enum Direction{
		EAST,
		SOUTH_EAST,
		SOUTH_WEST,
		WEST,
		NORTH_WEST,
		NORTH_EAST
	};
	Direction& operator++(Direction& d, int);
	Direction operator+(Direction d, int number);
	Direction operator-(Direction d, int number);
	Direction operator~(Direction dir);
//};



#endif