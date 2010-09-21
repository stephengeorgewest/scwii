#include "Node.h"
#include <stdio.h>
#include <stdlib.h>

//for debug created in SCWii.cpp
#include "NetworkConsole.h"
extern NetworkConsole netCon;

int nodeNum;//for Debug

Node::Node()
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
}
Node::Node(int id)
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
	ID=id;
}
Node *
Node::addNeighbor(Direction dir)
{	
	Node * n = new Node;
	return this->addNeighbor(dir,n);
}
Node *
Node::addNeighbor(Direction dir, Node * n)
{
	this->connectNodes(dir,n);
	
	// check clockwise 3
	//if(n->ID==9)
	{
		char buf[64];
		sprintf(buf, "\nThis %i, add %i, dir %i", this->ID,n->ID,dir);
		netCon.send_message(buf);
	}
	Node * neig = this->neighbor[dir+1];
	if(neig)//may be null
	{
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
	return n;
}

void
Node::connectNodes(Direction dir, Node* n2)
{	
	this->neighbor[dir]=n2;
	n2->neighbor[~dir]=this;
}


//must be previously connected nodes
	/*void
Node::connectNodesPortal(Node* n1, Direction dir1, Node* n2, Direction dir2)
{
	//make later

	Direction dir2;
	if(dir2_==NONE)
		dir2=~dir1;
	else
		dir2=dir2_;
		
	n1->neighbor[dir1]=n2;
	n2->neighbor[dir2]=n1;
	
}*/
