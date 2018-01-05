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

//Returns pointer to map tile
struct tile** maptile( int x, int y, int z )
{
	return map.map + z + x * map.depth + y * map.depth * map.width;
}

//Returns pointer to the topmost tile
struct tile** maptoptile( int x, int y )
{
	struct tile **t;
	int i;

	for ( i = map.depth - 1; i >= 0; i-- )
	{
		t = maptile( x, y, i );
		if ( t != NULL &&  ( *t )->common.type != TILE_VOID )
		 	return t;
	}

	return  NULL;
}

//Returns pointer to the bottommost free tile
struct tile** mapfreetile( int x, int y )
{
	struct tile **t;
	int i;

	for ( i = 0; i < map.depth; i++ )
	{
		t = maptile( x, y, i );
		if ( t == NULL || *t == NULL || ( *t )->common.type == TILE_VOID )
		 	return t;
	}

	return  NULL;
}

//Puts given tile on the bottommost free tile on give coordinates
struct tile** mapputtile( int x, int y, struct tile* tile )
{
	struct tile **t = mapfreetile( x, y );

	if( t != NULL )
	{
		*t = tile;
		return t;
	}

	return NULL;
}
