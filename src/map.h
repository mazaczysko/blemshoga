#ifndef MAP_H
#define MAP_H

#include "tile.h"

#define MAP_LFLOOR 0
#define MAP_LENT 14
#define MAP_LSOLID 15

struct
{
	int width, height, depth;
	struct tile **map;
} map;

extern void map_init( int width, int height, int depth );
extern struct tile** maptile( int x, int y, int z );
extern struct tile** maptoptile( int x, int y );
extern struct tile** mapfreetile( int x, int y );
extern struct tile** mapputtile( int x, int y, int z, struct tile* tile );
extern struct tile** mapmovetile( struct tile** tile, int x, int y );
extern struct tile* maptaketile( int x, int y );
extern int mapissolid( int x, int y );
extern int mapdroptile( int x, int y, struct tile* item );
extern int mapmakeroom( int x, int y, int maxwidth, int maxheight, int dir );
#endif
