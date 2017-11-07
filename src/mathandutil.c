#include "mathandutil.h"

int isDigit(char c)
{
	return (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || 
			c == '6' || c == '7' || c == '8' || c == '9' || c == '0');
}

int segmentsIntersect(svec2 a1, svec2 a2, svec2 b1, svec2 b2)
{
	float s1_x, s1_y, s2_x, s2_y;
    s1_x = (float)a2.x - (float)a1.x;     s1_y = (float)a2.y - (float)a1.y;
    s2_x = (float)b2.x - (float)b1.x;     s2_y = (float)b2.y - (float)b1.y;

    float s, t;
    s = (-s1_y * (float)(a1.x - b1.x) + s1_x * (float)(a1.y - b1.y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (float)(a1.y - b1.y) - s2_y * (float)(a1.x - b1.x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        return 1;
    }

    return 0; // No collision
}