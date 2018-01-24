#include <stdlib.h>
#include "map.h"
#include "tile.h"
#include <stdio.h>
#include <time.h>

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

int entitypass( int x, int y, int whence )
{
	struct tile **t;
	int i;

	if( x >= map.width || y >= map.height || x < 0 || y < 0 )
		return 1;

	for( i = 0; i < map.depth; i++ )
	{
		t = maptile( x, y, i );

		if( t != NULL && *t != NULL && *t != TILE_VOID && (*t)->block.entityinteraction != NULL )
			(*t)->block.entityinteraction( t, whence );
	}

	if( !mapissolid( x, y ) )
		return 1;

	return 0;
}

int rangedrandom( int min, int max )
{
	return rand( ) % ( max + 1 - min ) + min;
}

int mapmakeroom( int x, int y, int maxwidth, int maxheight, int dir )
{
	int minwidth = 4;
	int minheight = 4;

	int xend = x;
	int yend = y;

	int width = rangedrandom( minwidth, maxwidth );
	int height = rangedrandom( minheight, maxheight );

	switch( dir )
	{
		//North
		case 0:
			y -= height;
			xend += width;
			break;

		//South
		case 1:
			yend += height;
			xend += width;
			break;

		//West
		case 2:
			x -= width;
			yend += height;
			break;

		//East
		case 3:
			xend += width;
			yend += height;
			break;

		default:
			break;

	}

	for( int i = x; i < xend; i++ )
	{
		for( int j = y; j < yend; j++ )
		{
			*maptile( i, j, 0 ) = blocks + 1;
		}
	}

	for( int i = x + 1; i < xend - 1; i++ )
	{
		for( int j = y + 1; j < yend - 1; j++ )
		{
			*maptile( i, j, 0 ) = blocks;
		}
	}

return 0;
}
