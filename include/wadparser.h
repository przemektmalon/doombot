
// wadparser.h defines data structures to hold 
// WAD file data and functions to parse the WAD files

#ifndef WADPARSER_H
#define WADPARSER_H

#include "math.h"
#include <io.h>
#include <stdlib.h> 

// WAD lump types

#define THINGS 0
#define LINEDEFS 1
#define SIDEDEFS 2
#define VERTEXES 3
#define SEGS 4
#define SSECTORS 5
#define NODES 6
#define SECTORS 7
#define REJECT 8
#define BLOCKMAP 9
#define BEHAVIOUR 10
#define SCRIPTS 11
#define MAX_LUMP_TYPES 12

struct LineDef
{
	unsigned short beginVertex;
	unsigned short endVertex;
	unsigned short flags;
	unsigned short type;
	unsigned short sector;
	unsigned short rightSide;
	unsigned short leftSide;
};

struct Vertex
{
	signed short x;
	signed short y;
};

struct Level
{
	char* name;
	
	struct ivec2* lines;
	struct Vertex* vertices;

	struct ivec2 mapLowerLeft;
	struct ivec2 mapUpperRight;

};

struct WADHeader
{
	char type[4];
	int numEntries;
	int dirLocation;
};

struct WAD
{
	struct WADHeader header;
	struct Level* levels;
};

struct WAD* loadWAD(char* wadFile)
{
	FILE* wadf;
	wadf = fopen(wadFile,"rb");

	if (!wadf)
	{
		printf("Failed to open WAD file");
		return 0;
	}

	struct WAD* wad = (struct WAD*)malloc(sizeof(struct WAD));

	fread(&wad->header, sizeof(struct WADHeader), 1, wadf);

	printf("%c", wad->header.type[0]);
	printf("%c", wad->header.type[1]);
	printf("%c", wad->header.type[2]);
	printf("%c", wad->header.type[3]);

	fclose(wadf);

	return wad;
}

#endif