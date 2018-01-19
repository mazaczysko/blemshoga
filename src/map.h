#ifndef MAP_H
#define MAP_H

struct
{
	int width, height, depth;
	struct tile **map;
} map;

extern void map_init( int width, int height, int depth );
extern struct tile** maptile( int x, int y, int z );
extern struct tile** maptoptile( int x, int y );
extern struct tile** mapfreetile( int x, int y );
extern struct tile** mapputtile( int x, int y, struct tile* tile );
extern struct tile** mapmovetile( struct tile** tile, int x, int y );
extern int mapissolid( int x, int y );
#endif
