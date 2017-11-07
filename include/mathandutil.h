#ifndef MATHANDUTIL_H
#define MATHANDUTIL_H

typedef struct svec2
{
	signed short x;
	signed short y;
} svec2;

typedef struct ivec2
{
	int x;
	int y;
} ivec2;

typedef struct fvec2
{
	float x;
	float y;
} fvec2;

int isDigit(char c);
int segmentsIntersect(svec2 a1, svec2 a2, svec2 b1, svec2 b2);

#endif