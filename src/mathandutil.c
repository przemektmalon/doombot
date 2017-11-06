#include "mathandutil.h"

int isDigit(char c)
{
	return (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || 
			c == '6' || c == '7' || c == '8' || c == '9' || c == '0');
}

int ccw(svec2 a, svec2 b, svec2 c)
{
	return ((c.y-a.y) * (b.x-a.x)) > ((b.y-a.y) * (c.x-a.x));
}

int checkIntersection(svec2 a1, svec2 a2, svec2 b1, svec2 b2)
{
	return (ccw(a1,b1,b2) != ccw(a2,b1,b2)) && (ccw(a1,a2,b1) != ccw(a1,a2,b2));
}