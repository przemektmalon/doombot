#include "svg.h"
#include "nodegenerator.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

int main()
{
	WAD* wad = loadWAD("doom1.wad");

	svec2 s; s.x = 1056; s.y = -3616;
	NodeList nodes = generateNodes(wad->levels, 30, s);

	svg* mapsvg = svg_create(2000,2000);

	for (int i = 0; i < wad->levels[0].numColliders; ++i)
	{
		svec2 l1;
		svec2 l2;
		l1 = wad->levels[0].vertices[wad->levels[0].colliders[i].x];
		l2 = wad->levels[0].vertices[wad->levels[0].colliders[i].y];

		l1.x = (int)(((float)l1.x/(float)(wad->levels[0].mapUpperRight.x - wad->levels[0].mapLowerLeft.x)) * 1024.f);
		l1.y = (int)(((float)l1.y/(float)(wad->levels[0].mapUpperRight.y - wad->levels[0].mapLowerLeft.y)) * 1024.f);

		l2.x = (int)(((float)l2.x/(float)(wad->levels[0].mapUpperRight.x - wad->levels[0].mapLowerLeft.x)) * 1024.f);
		l2.y = (int)(((float)l2.y/(float)(wad->levels[0].mapUpperRight.y - wad->levels[0].mapLowerLeft.y)) * 1024.f);

		//printf("%i : %i to  %i : %i\n", l1.x, l1.y, l2.x, l2.y);

		svg_line(mapsvg, "#0000f0", 5, l1.x + 500, l1.y + 1800, l2.x + 500, l2.y + 1800);
	}

	for (int i = 0; i < nodes.size; ++i)
	{
		svec2 l1;

		l1.x = nodes.nodes[i].x;
		l1.y = nodes.nodes[i].y;

		l1.x = (int)(((float)l1.x/(float)(wad->levels[0].mapUpperRight.x - wad->levels[0].mapLowerLeft.x)) * 1024.f);
		l1.y = (int)(((float)l1.y/(float)(wad->levels[0].mapUpperRight.y - wad->levels[0].mapLowerLeft.y)) * 1024.f);

		//svg_line(mapsvg, "#ff0000", 10, l1.x + 500, l1.y + 1800, l1.x + 505, l1.y + 1805);
		svg_circle(mapsvg, "#0000ff", "#ff0000", 1, l1.x + 500, l1.y + 1800, 2);
	}

	svg_finalize(mapsvg);
	svg_save(mapsvg, "E1M1.svg");
	svg_free(mapsvg);

	return 0;
}