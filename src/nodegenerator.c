#include "nodegenerator.h"

void initNode(Node* n, svec2 p);
int nodeExists(Node* n, int len, svec2 p);
void makeLink(Node* a, Node* b);
void linkNodes(Node* a, Node* b);
void copyNode(Node* src, Node* dst);
void copyNodeConvertTravel(Node* src, Node* dst, Node* srcListBegin, Node* dstListBegin);

void initNode(Node* n, svec2 p)
{
	n->x = p.x;
	n->y = p.y;
	for (int i = 0; i < 8; ++i)
		n->travel[i] = 0;
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

NodeList generateNodes(Level* lvl, int spacing, svec2 start)
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

	NodeList ret;
	ret.nodes = malloc((sizeof(Node)*ni));
	for (int i = 0; i < ni; ++i)
	{
		copyNodeConvertTravel(&nodes[i], &ret.nodes[i], nodes, ret.nodes);
	}
	ret.size = ni;
	return ret;
}