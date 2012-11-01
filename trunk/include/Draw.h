#ifndef Draw_h
#define Draw_h

#include "HexNode.h"
#include "Terrain.h"

#include <grrlib.h>
class Draw{
public:
	DrawTerrain(Terrain* t, int start_x, int start_y);
	DrawMap(Terrain *t );
	DrawNode(HexNode * n, int x, int y);
	
}
#endif