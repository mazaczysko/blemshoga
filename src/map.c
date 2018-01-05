#include <stdlib.h>
#include "map.h"
#include "tile.h"

void map_init( int width, int height, int depth )
{
  	map.width = width;
	map.height = height;
	map.depth = depth;

	//Won't be portable if NULL is not 0
	map.map = calloc( map.width * map.height * map.depth, sizeof( struct tile* ) );

}

//TODO add some inline routines for fetching map tiles
//NOTE: should return pointers!
//maptile(x,y,z)
//maptoptile(x,y)
//mapfreetile(x,y)
