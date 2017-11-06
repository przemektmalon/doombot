#include "svg.h"
#include "wadparser.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct Node
{
	short x;
	short y;
	struct Node* travel[8];
} Node;

void initNode(Node* n, svec2 p)
{
	n->x = p.x;
	n->y = p.y;
	for (int i = 0; i < 8; ++i)
		n->travel[i] = 0;
}

static const int dirs[16] = {1,0,0,-1,-1,0,0,1,1,1,-1,-1,1,-1,-1,1};

int ccw(svec2 a, svec2 b, svec2 c)
{
	return ((c.y-a.y) * (b.x-a.x)) > ((b.y-a.y) * (c.x-a.x));
}

int checkIntersection(svec2 a1, svec2 a2, svec2 b1, svec2 b2)
{
	return (ccw(a1,b1,b2) != ccw(a2,b1,b2)) && (ccw(a1,a2,b1) != ccw(a1,a2,b2));
}

int nodeExists(Node* n, int len, svec2 p)
{
	for (int i = 0; i < len; ++i)
	{
		if (n[i].x == p.x && n[i].y == p.y)
			return i;
	}
	return -1;
}

void makeLink(Node* a, Node* b)
{
	int i = 0;
	for (i; i < 8; ++i)
	{
		if (a->travel[i] == 0)
			break;
		if (a->travel[i]->x == b->x && a->travel[i]->y == b->y)
			return;
	}
	a->travel[i] = b;
}

void linkNodes(Node* a, Node* b)
{
	makeLink(a,b);
	makeLink(b,a);
}

void copyNode(Node* src, Node* dst)
{
	dst->x = src->x;
	dst->y = src->y;
	for (int i = 0; i < 8; ++i)
		dst->travel[i] = src->travel[i];
}

void copyNodeConvertTravel(Node* src, Node* dst, Node* srcListBegin, Node* dstListBegin)
{
	dst->x = src->x;
	dst->y = src->y;
	for (int i = 0; i < 8; ++i)
		dst->travel[i] = (Node*)(((int)dstListBegin + ((int)src - (int)srcListBegin)));
}

int numNodes = 0;

Node* generateNodes(Level* lvl, int spacing, svec2 start)
{
	Node nodes[10000];
	int ni = 0;

	int expand[10000];
	int toExp = 0;

	int nextExpand[10000];
	int nextExpandCount = 0;

	initNode(&nodes[ni], start);

	expand[0] = ni;
	++ni; toExp = 1;

	while (toExp != 0)
	{
		for (int i = 0; i < toExp; ++i)
		{
			Node* enode = nodes + expand[i];
			for (int d = 0; d < 8; ++d)
			{
				svec2 nextPos; 
				nextPos.x = enode->x + (spacing * ((ivec2*)dirs)[d].x);
				nextPos.y = enode->y + (spacing * ((ivec2*)dirs)[d].y);

				Node nextNode; initNode(&nextNode, nextPos);

				svec2 ns; ns.x = enode->x; ns.y = enode->y;
				svec2 ne = nextPos;

				int intersecting = 0;
				for (int w = 0; w < lvl->numLines; ++w)
				{
					svec2 ws = lvl->vertices[lvl->lines[w].beginVertex];
					svec2 we = lvl->vertices[lvl->lines[w].endVertex];

					intersecting |= checkIntersection(ws,we,ns,ne);

					if (intersecting)
						break;
				}

				if (intersecting)
					continue;

				int checkNodeExists = nodeExists(nodes,ni,nextPos);
				if (checkNodeExists != -1) // Node does exist
				{
					linkNodes(&nodes[checkNodeExists], enode);
				}
				else // Node doesn't exist
				{
					copyNode(&nextNode, &nodes[ni]);
					linkNodes(&nodes[ni], enode);

					nextExpand[nextExpandCount] = ni;
					++nextExpandCount;

					++ni;
				}
			}
		}

		for (int i = 0; i < nextExpandCount; ++i)
		{
			expand[i] = nextExpand[i];
		}
		toExp = nextExpandCount;
		nextExpandCount = 0;
	}

	numNodes = ni;

	Node* ret = malloc((sizeof(Node)*ni) + 1);
	for (int i = 0; i < ni; ++i)
	{
		copyNodeConvertTravel(&nodes[i], &ret[i], nodes, ret);
	}
	ret[ni].x == -99999;
	return ret;
}

int main()
{
	WAD* wad = loadWAD("doom1.wad");

	svec2 s; s.x = 1056; s.y = -3616;
	Node* nodes = generateNodes(wad->levels, 50, s);

	svg* mapsvg = svg_create(2000,2000);

	for (int i = 0; i < wad->levels[0].numLines; ++i)
	{
		svec2 l1;
		svec2 l2;
		l1 = wad->levels[0].vertices[wad->levels[0].lines[i].beginVertex];
		l2 = wad->levels[0].vertices[wad->levels[0].lines[i].endVertex];

		l1.x = (int)(((float)l1.x/(float)(wad->levels[0].mapUpperRight.x - wad->levels[0].mapLowerLeft.x)) * 1024.f);
		l1.y = (int)(((float)l1.y/(float)(wad->levels[0].mapUpperRight.y - wad->levels[0].mapLowerLeft.y)) * 1024.f);

		l2.x = (int)(((float)l2.x/(float)(wad->levels[0].mapUpperRight.x - wad->levels[0].mapLowerLeft.x)) * 1024.f);
		l2.y = (int)(((float)l2.y/(float)(wad->levels[0].mapUpperRight.y - wad->levels[0].mapLowerLeft.y)) * 1024.f);

		//printf("%i : %i to  %i : %i\n", l1.x, l1.y, l2.x, l2.y);

		svg_line(mapsvg, "#000000", 5, l1.x + 500, l1.y + 1800, l2.x + 500, l2.y + 1800);
	}

	for (int i = 0; i < numNodes; ++i)
	{
		svec2 l1;

		l1.x = nodes[i].x;
		l1.y = nodes[i].y;

		l1.x = (int)(((float)l1.x/(float)(wad->levels[0].mapUpperRight.x - wad->levels[0].mapLowerLeft.x)) * 1024.f);
		l1.y = (int)(((float)l1.y/(float)(wad->levels[0].mapUpperRight.y - wad->levels[0].mapLowerLeft.y)) * 1024.f);


		svg_line(mapsvg, "#ff0000", 10, l1.x + 500, l1.y + 1800, l1.x + 505, l1.y + 1805);
	}

	svg_finalize(mapsvg);
	svg_save(mapsvg, "E1M1.svg");
	svg_free(mapsvg);




	return 0;
}