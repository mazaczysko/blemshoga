#include <stdlib.h>
#include "map.h"
#include "tile.h"
#include <stdio.h>
#include <time.h>

struct map map;

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
		if ( t != NULL && *t != NULL )
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
		if ( t != NULL && *t == NULL )
		 	return t;
	}

	return  NULL;
}

//Puts given tile on the bottommost free tile on give coordinates
struct tile** mapputtile( int x, int y, int z, struct tile* tile )
{
	struct tile **t = maptile( x, y, z );

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

//I don't want to know the reason for inverted logics in this function....
int mapissolid( int x, int y )
{
	struct tile **t;
	int i;

	for( i = 0; i < map.depth; i++ )
	{
		t = maptile( x, y, i );

		if( t != NULL && *t != NULL )
			if( (*t)->solid == 1 )
				return 1;

	}

	return 0;
}

//'Takes' and returns pointer to the topmost itemlike tile
struct tile* maptaketile( int x, int y )
{
	struct tile **t, *item;
	int i;

	for ( i = MAP_LENT - 1; i > MAP_LFLOOR; i-- )
	{
		printf("i=%d\n", i );
		t = maptile( x, y, i );
		if ( t != NULL && *t != NULL )
		{
			item = *t;
			*t = NULL;

		 	return item;
		}
	}

	return  NULL;
}

//Put itemlike tile into first free layer
int mapdroptile( int x, int y, struct tile* item )
{
	struct tile **t;
	int i;

	for ( i = MAP_LFLOOR + 1; i < MAP_LENT ; i++ )
	{
		t = maptile( x, y, i );
		if ( t != NULL && *t == NULL )
		{	*t = item;
			return 0;
		}
	}

	return 1;
}
