#include "svg.h"
#include "wadparser.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main()
{
	WAD* wad = loadWAD("doom1.wad");

	svg* mapsvg = svg_create(2000,2000);

	for (int i = 0; i < wad->levels[0].numLines; ++i)
	{
		Vertex l1;
		Vertex l2;
		l1 = wad->levels[0].vertices[wad->levels[0].lines[i].beginVertex];
		l2 = wad->levels[0].vertices[wad->levels[0].lines[i].endVertex];

		l1.x = (int)(((float)l1.x/(float)(wad->levels[0].mapUpperRight.x - wad->levels[0].mapLowerLeft.x)) * 1024.f);
		l1.y = (int)(((float)l1.y/(float)(wad->levels[0].mapUpperRight.y - wad->levels[0].mapLowerLeft.y)) * 1024.f);

		l2.x = (int)(((float)l2.x/(float)(wad->levels[0].mapUpperRight.x - wad->levels[0].mapLowerLeft.x)) * 1024.f);
		l2.y = (int)(((float)l2.y/(float)(wad->levels[0].mapUpperRight.y - wad->levels[0].mapLowerLeft.y)) * 1024.f);

		//printf("%i : %i to  %i : %i\n", l1.x, l1.y, l2.x, l2.y);

		svg_line(mapsvg, "#000000", 5, l1.x + 500, l1.y + 1800, l2.x + 500, l2.y + 1800);
	}



	svg_finalize(mapsvg);
	svg_save(mapsvg, "E1M1.svg");
	svg_free(mapsvg);

	return 0;
}