#ifndef MAP_H
#define MAP_H

struct
{
	int width, height, depth;
	struct tile *map;
} map;

extern void map_init( int width, int height, int depth );

//TODO replace this macro with some inline functions
#define MAP(x, y, z) (map.map[z + x * map.depth + y * map.depth * map.width])

#endif
