#include "wadparser.h"

int compareString(char* a, char* b, int len);
int isLevelValid(Level* level);

int compareString(char* a, char* b, int len)
{
	int ret = 1;
	for(int i = 0; i < len; ++i)
	{
		ret &= (a[i] == b[i]);
	}
	return ret;
}

int isLevelValid(Level* level)
{
	if (level == 0)
		return 0;
	else
		return level->lines != 0 && level->vertices != 0 && level->name[0] != '\0';
}

WAD* loadWAD(char* wadFile)
{
	FILE* wadf;
	wadf = fopen(wadFile,"rb");

	if (!wadf)
	{
		printf("Failed to open WAD file");
		return 0;
	}

	WAD* wad = (WAD*)malloc(sizeof(WAD));

	fread(&wad->header, sizeof(WADHeader), 1, wadf);

	char* data = malloc(wad->header.dirLocation-12);
	fread(data, wad->header.dirLocation-12, 1, wadf);

	DirectoryEntry* lumpInfos = malloc(sizeof(DirectoryEntry)*wad->header.numEntries);

	fread(lumpInfos, sizeof(DirectoryEntry) * wad->header.numEntries, 1, wadf);

	Level* currentLevel = malloc(sizeof(Level));

	svec2 inf; inf.x = 16000; inf.y = inf.x;
	svec2 minf; minf.x = -inf.x; minf.y = -inf.x;

	currentLevel->mapLowerLeft = inf;
	currentLevel->mapUpperRight = minf;

	int levelIndex = 0;

	for (int i = 0; i < wad->header.numEntries; ++i)
	{
		int isNameLump = 0;

		if (lumpInfos[i].name[0] == 'E')
			if (isDigit(lumpInfos[i].name[1]))
				if (lumpInfos[i].name[2] == 'M')
					if (isDigit(lumpInfos[i].name[3]))
						isNameLump = 1;

		if (lumpInfos[i].name[0] == 'M')
			if (lumpInfos[i].name[1] == 'A')
				if (lumpInfos[i].name[2] == 'P')
					if (isDigit(lumpInfos[i].name[3]))
						if (isDigit(lumpInfos[i].name[4]))
							isNameLump = 1;

		if (isNameLump)
		{
			strcpy(currentLevel->name, &lumpInfos[i].name[0]);
		}
		else
		{
			if (compareString(lumpInfos[i].name, "LINEDEFS", 8))
			{
				currentLevel->numLines = lumpInfos[i].size / sizeof(LineDef);
				currentLevel->lines = malloc(lumpInfos[i].size);
				memcpy(currentLevel->lines, &data[lumpInfos[i].filepos-12], lumpInfos[i].size);
			}
			else if (compareString(lumpInfos[i].name, "SEDEDEFS", 8))
			{
				//currentLevel->sides = malloc(lumpInfos[i].size);
			}
			else if (compareString(lumpInfos[i].name, "VERTEXES", 8))
			{
				currentLevel->numVerts = lumpInfos[i].size / sizeof(svec2);
				currentLevel->vertices = malloc(lumpInfos[i].size);
				memcpy(currentLevel->vertices, &data[lumpInfos[i].filepos-12], lumpInfos[i].size);

				for (int j = 0; j < currentLevel->numVerts; ++j)
				{
					if (currentLevel->vertices[j].x < currentLevel->mapLowerLeft.x)
						currentLevel->mapLowerLeft.x = currentLevel->vertices[j].x;

					if (currentLevel->vertices[j].y < currentLevel->mapLowerLeft.y)
						currentLevel->mapLowerLeft.y = currentLevel->vertices[j].y;

					if (currentLevel->vertices[j].x > currentLevel->mapUpperRight.x)
						currentLevel->mapUpperRight.x = currentLevel->vertices[j].x;

					if (currentLevel->vertices[j].y > currentLevel->mapUpperRight.y)
						currentLevel->mapUpperRight.y = currentLevel->vertices[j].y;
				}
			}
			else if (compareString(lumpInfos[i].name, "THINGS", 6))
			{
				//currentLevel->things = malloc(lumpInfos[i].size);
			}
		}

		if (isLevelValid(currentLevel))
		{
			wad->levels[levelIndex] = *currentLevel; // TODO: change currentLevel to a stack var not ptr
			++levelIndex;
			currentLevel = malloc(sizeof(Level));
		}
	}

	if (!isLevelValid(currentLevel))
		free(currentLevel);

	free(data);

	fclose(wadf);

	return wad;
}


