#include "map.h"
#include "tile.h"
#include <stdio.h>

int rangedrandom( int min, int max )
{
	return rand( ) % ( max + 1 - min ) + min;
}

//If all area is void returns 0, in other case 1
int mapisvoid( int x, int y, int endx, int endy )
{

	int i, j, k;

	//Uauaua...
	if( x >= map.width || x < 0 || y >= map.height || y < 0 || endx >= map.width || endx < 0 || endy >= map.height || endy < 0 || endx < x || endy < y )
		return 1;

	for( i = x; i < endx; i++ )
		for( j = y; j < endy; j++ )
			for( k = 0; k < map.depth; k++ )
				if( maptile( i, j, k ) != NULL )
					return 1;

	return 0;
}

int makeroom( int x, int y, int maxwidth, int maxheight, int dir )
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

	//Ehhh...
	if( x >= map.width || x < 0 || y >= map.height || y < 0 || xend >= map.width || xend < 0 || yend >= map.height || yend < 0 )
		return 1;

	for( int i = x; i < xend; i++ )
	{
		for( int j = y; j < yend; j++ )
		{
			mapputtile( i, j, MAP_LSOLID, tile( "stone wall" ) );
		}
	}

	for( int i = x + 1; i < xend - 1; i++ )
	{
		for( int j = y + 1; j < yend - 1; j++ )
		{
			mapputtile( i, j, MAP_LSOLID, NULL );
			mapputtile( i, j, MAP_LFLOOR, tile( "stone floor") );
		}
	}

	return 0;
}

int makecorridor( int x, int y, int maxlength, int dir)
{
	int i, j;

	int xend = x;
	int yend = y;

	//Minimal lenght = 2
	int length = rangedrandom( 2, maxlength );

	switch( dir )
	{
		//North
		case 0:
			y -= length;
			break;

		//South
		case 1:
			yend += length;
			break;

		//West
		case 2:
			x -= length;
			break;

		//East
		case 3:
			xend += length;
			break;

		default:
			break;

	}


	//Ughh...
	if( x >= map.width || x < 0 || y >= map.height || y < 0 || xend >= map.width || xend < 0 || yend >= map.height || yend < 0 )
		return 1;

	if( mapisvoid( x, y, xend, yend ) )
		return 1;

	for( i = x; i <= xend; i++ )
	{
		for( j = y; j <= yend; j++ )
		{
			mapputtile( i, j, MAP_LFLOOR, tile( "stone floor" ) );
		}
	}

	//Corridor borders
	switch ( dir )
	{
		case 0:
			for( i = y; i <= yend; i++ )
			{
				mapputtile( x - 1, i, MAP_LSOLID, tile( "stone wall" ) );
				mapputtile( x + 1, i, MAP_LSOLID, tile( "stone wall" ) );
			}
		break;

		case 1:
			for( i = y; i <= yend; i++ )
			{
				mapputtile( x - 1, i, MAP_LSOLID, tile( "stone wall" ) );
				mapputtile( x + 1, i, MAP_LSOLID, tile( "stone wall" ) );
			}
			break;

		case 2:
			for( i = x; i <= xend; i++ )
			{
				mapputtile( i, y - 1, MAP_LSOLID, tile( "stone wall" ) );
				mapputtile( i, y + 1, MAP_LSOLID, tile( "stone wall" ) );
			}
			break;

		case 3:
			for( i = x; i <= xend; i++ )
			{
				mapputtile( i, y - 1, MAP_LSOLID, tile( "stone wall" ) );
				mapputtile( i, y + 1, MAP_LSOLID, tile( "stone wall" ) );
			}
			break;

		default:
			break;

	}

	//for( i =)
	return 0;
}
