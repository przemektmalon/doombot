
#ifndef NODEGENERATOR_H
#define NODEGENERATOR_H

#include "mathandutil.h"
#include "wadparser.h"

static const int dirs[16] = {1,0,0,-1,-1,0,0,1,1,1,-1,-1,1,-1,-1,1};

typedef struct Node
{
	short x;
	short y;
	struct Node* travel[8];
} Node;

typedef struct NodeList
{
	Node* nodes;
	int size;
} NodeList;

NodeList generateNodes(Level* lvl, int spacing, svec2 start);

#endif