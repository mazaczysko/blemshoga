#ifndef MAP_H
#define MAP_H

struct
{
	uint16_t width, height, depth;
	struct tile *map;
} map;

extern void map_init( uint16_t width, uint16_t height, uint16_t depth );

#define MAP(x, y, z) (map.map[z + x * map.depth + y * map.depth * map.width])

#endif
