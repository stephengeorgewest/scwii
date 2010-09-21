#ifndef City_h
#define City_h

class HexNode;
class Piece;
//class Team;


class City{
	public:
		HexNode * location;
		bool isPortCity;//convience
		Piece * currentProduction;
		int productionDaysRemaining;
	//	Team * owner;
	//protected:
	//private:
};

#endif