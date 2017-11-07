
// part of a simple SVG writer from http://www.code-in-c.com/writing-svg-library-c/

#ifndef SVG_H
#define SVG_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct svg
{
    char* svg;
    int height;
    int width;
    bool finalized;
} svg;

svg* svg_create(int width, int height);
void svg_finalize(svg* psvg);
void svg_free(svg* psvg);
void svg_line(svg* psvg, char* stroke, int strokewidth, int x1, int y1, int x2, int y2);
void svg_circle(svg* psvg, char* fill, char* stroke, int strokewidth, int x, int y, int r);
void svg_save(svg* psvg, char* filepath);

#endif