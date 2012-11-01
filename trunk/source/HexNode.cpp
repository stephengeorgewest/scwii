#include "HexNode.h"
#include <stdio.h>
#include <stdlib.h>

//for debug created in SCWii.cpp
#include "NetworkConsole.h"
extern NetworkConsole netCon;

int nodeNum;//for Debug

HexNode::HexNode()
{
	city=0;
	//PieceList pieces;
	neighbor[NORTH_WEST]=0;
	neighbor[NORTH_EAST]=0;
	neighbor[EAST]=0;
	neighbor[SOUTH_EAST]=0;
	neighbor[SOUTH_WEST]=0;
	neighbor[WEST]=0;
	isGround=false;
	ID=nodeNum++;
	location=UNSPECIFIED;
}
HexNode::HexNode(GeographicLocation loc)
{
	city=0;
	//PieceList pieces;
	neighbor[NORTH_WEST]=0;
	neighbor[NORTH_EAST]=0;
	neighbor[EAST]=0;
	neighbor[SOUTH_EAST]=0;
	neighbor[SOUTH_WEST]=0;
	neighbor[WEST]=0;
	isGround=false;
	location=loc;
	ID=nodeNum++;
	
}
HexNode::HexNode(int id)
{
	city=0;
	//PieceList pieces;
	neighbor[NORTH_WEST]=0;
	neighbor[NORTH_EAST]=0;
	neighbor[EAST]=0;
	neighbor[SOUTH_EAST]=0;
	neighbor[SOUTH_WEST]=0;
	neighbor[WEST]=0;
	isGround=false;
	location=UNSPECIFIED;
	ID=id;
}
HexNode::~HexNode()
{
	
}
HexNode *
HexNode::addNeighbor(HexDirection dir)
{	
	HexNode * n = new HexNode;
	//create coordinate of new node.
	switch(dir)
	{
		case EAST:
			n->x=this->x+1;
			n->y=this->y;
			break;
		case WEST:
			n->x=this->x-1;
			n->y=this->y;
			break;
		case SOUTH_EAST:
			if(this->y%2==0)
			{
				n->x=this->x;
				n->y=this->y+1;
			}
			else
			{
				n->x=this->x+1;
				n->y=this->y+1;
			}
			break;
		case SOUTH_WEST:
			if(this->y%2==0)
			{
				n->x=this->x-1;
				n->y=this->y+1;
			}
			else
			{
				n->x=this->x;
				n->y=this->y+1;
			}
			break;
		case NORTH_WEST:
			if(this->y%2==0)
			{
				n->x=this->x-1;
				n->y=this->y-1;
			}
			else
			{
				n->x=this->x;
				n->y=this->y-1;
			}
			break;
		case NORTH_EAST:
			if(this->y%2==0)
			{
				n->x=this->x;
				n->y=this->y-1;
			}
			else
			{
				n->x=this->x+1;
				n->y=this->y-1;
			}
			break;
		default:
			break;
	}
	return this->addNeighbor(dir,n);
}
HexNode *
HexNode::addNeighbor(HexDirection dir, HexNode * n)
{
	
	{
		char buf[64];
		sprintf(buf, "Add Neighbor(dir %i, ~)This %i(%i,%i), add %i(%i,%i)",dir,
		this->ID,this->x,this->y, n->ID,n->x,n->y);
		netCon.sendMessage(buf);
	}
	this->connectNodes(dir,n);
	// check clockwise 3
	//if(n->ID==9)
	
	//add neighbors of established this to new n
	// clockwise 3
	HexNode * neig = this->neighbor[dir+1];
	if(neig)//may be null
	{
		netCon.sendMessage("have valid neighbor dir+1");
		n->connectNodes((~dir)-1,neig);
		
		neig = neig->neighbor[dir];
		if(neig)
		{
			n->connectNodes((~dir)-2,neig);
			
			neig = neig->neighbor[dir-1];
			if(neig)
			{
				n->connectNodes(dir,neig);
			}
		}
	}
	//ccw 2 3? We can do the last one twice
	neig = this->neighbor[dir-1];
	if(neig)
	{
		netCon.sendMessage("have valid neighbor dir-1");
		//neig may be null ... is fine
		//neig->neig... may be null ... is not fine
		
		n->connectNodes((~dir+1),neig);
		neig = neig->neighbor[dir];
		if(neig)
		{
			
			n->connectNodes((~dir+2),neig);
			
			neig = neig->neighbor[dir+1];
			if(neig)
			{
				n->connectNodes(dir,neig);
			}
		}
	}
	netCon.sendMessage("Neighbors connected");
	return n;
}

void
HexNode::connectNodes(HexDirection dir, HexNode* n2)
{	
	{
		char str[256];
		sprintf(str, "%i(%i,%i) connect nodes (dir=%i,id=%i(%i,%i)) ",
			this->ID, this->x,this->y,dir,  n2->ID, n2->x,n2->y);
		netCon.sendMessage(str);
		if((this->ID==13&&n2->ID==24)||(this->ID==24&&n2->ID==13))
			netCon.sendMessage("connecting 13+24");
	}
	this->neighbor[dir]=n2;
	n2->neighbor[~dir]=this;
}


//must be previously connected nodes
	/*void
HexNode::connectNodesPortal(HexNode* n1, HexDirection dir1, HexNode* n2, HexDirection dir2)
{
	//make later

	HexDirection dir2;
	if(dir2_==NONE)
		dir2=~dir1;
	else
		dir2=dir2_;
		
	n1->neighbor[dir1]=n2;
	n2->neighbor[dir2]=n1;
	
}*/

ostream& operator<<(ostream& output, const HexNode* n) {
    output << "\t\t" << n->neighbor[NORTH_WEST]->ID << " ^ " << n->neighbor[NORTH_EAST]->ID <<"\n" <<
		n->neighbor[WEST]->ID << " | " << n->id"(" <<  n->x << ", " << n->y <<") | " << n->neighbor[EAST]->ID <<
			"\t\t" << n->neighbor[SOUTH_WEST]->ID << " v " << n->neighbor[SOUTH_EAST]->ID;
    return output;  // for multiple << operators.
}















