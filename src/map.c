#include <stdlib.h>
#include "map.h"
#include "tile.h"
#include <stdio.h>

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
	if ( x > map.width || y > map.height || z > map.depth || x < 0 || y < 0 || z < 0 ) return NULL;
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
		if ( t != NULL && *t != NULL && ( *t )->common.type != TILE_VOID )
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

//Moves map tile
struct tile** mapmovetile( struct tile** tile, int x, int y )
{
	if( x >= map.width || x < 0 || y >= map.width || y < 0 )
		return NULL;

	struct tile **t = mapfreetile( x, y );

	if( t != NULL && tile != NULL )
	{
		*t = *tile;
		*tile = NULL;
		return t;
	}

	return NULL;
}

int mapissolid( int x, int y )
{
	struct tile **t;
	int i;

	for( i = 0; i < map.depth; i++ )
	{
		t = maptile( x, y, i );

		if( t != NULL && *t != NULL && *t != TILE_VOID)
			if( (*t)->block.solid == 1 )
				return 0;

	}

return 1;
}
