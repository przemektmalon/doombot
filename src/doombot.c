#include "svg.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main()
{
	svg* testsvg = svg_create(1024,1024);

	if (!testsvg)
	{
		printf("SVG creation failed!");
	}

	svg_line(testsvg, "#00ffff", 10, 100,100,900,900);
	svg_finalize(testsvg);
	svg_save(testsvg, "testsvg.svg");

	return 0;
}