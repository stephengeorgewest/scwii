#ifndef City_h
#define City_h

class Node;
class Piece;
//class Team;


class City{
	public:
		Node * location;
		bool isPortCity;//convience
		Piece * currentProduction;
		int productionDaysRemaining;
	//	Team * owner;
	//protected:
	//private:
};

#endif