
// wadparser.h defines data structures to hold 
// WAD file data and functions to parse the WAD files

#ifndef WADPARSER_H
#define WADPARSER_H

#include "math.h"
#include <io.h>
#include <stdio.h>
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

typedef struct LineDef
{
	unsigned short beginVertex;
	unsigned short endVertex;
	unsigned short flags;
	unsigned short type;
	unsigned short sector;
	unsigned short rightSide;
	unsigned short leftSide;
} LineDef;

typedef struct Vertex
{
	signed short x;
	signed short y;
} Vertex;

typedef struct Level
{
	char name[8];
	
	int numLines;
	struct LineDef* lines;
	int numVerts;
	struct Vertex* vertices;

	struct ivec2 mapLowerLeft;
	struct ivec2 mapUpperRight;

} Level;

typedef struct DirectoryEntry
{
	int filepos;
	int size;
	char name[8];
} DirectoryEntry;

typedef struct WADHeader
{
	char type[4];
	int numEntries;
	int dirLocation;
} WADHeader;

typedef struct WAD
{
	struct WADHeader header;
	struct Level levels[10]; 
} WAD;

WAD* loadWAD(char* wadFile);

#endif