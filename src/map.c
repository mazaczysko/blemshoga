#include <stdlib.h>
#include <inttypes.h>
#include "map.h"
#include "tile.h"

void map_init( uint16_t width, uint16_t height, uint16_t depth )
{
  	map.width = width;
	map.height = height;
	map.depth = depth;

	//Won't be portable if NULL is not 0
	map.map = (struct tile*) calloc( map.width * map.height * map.depth, sizeof( struct tile ) );

}

//TODO add some inline routines for fetching map tiles
//NOTE: should return pointers!
//maptile(x,y,z)
//maptoptile(x,y)
//mapfreetile(x,y)
