#ifndef TILE_H
#define TILE_H

#define TILE_SIZE 32

#define TILE_VOID   0
#define TILE_BLOCK  1
#define TILE_ITEM   2
#define TILE_ENTITY 3

struct tile
{
	void *tile;
	unsigned int type;
};


#endif
