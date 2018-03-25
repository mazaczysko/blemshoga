#include "map.h"
#include "tile.h"
#include <math.h>
#include <unistd.h>
#include <stdio.h>

struct room
{
	int x, y;
	int w, h;
	int id;
};

int rangedrandom( int min, int max )
{
	return rand( ) % ( max + 1 - min ) + min;
}

struct room mkroom( int x, int y, int w, int h, int id )
{
	struct room room =
	{
		x, y, w, h, id,
	};

	return room;

}

int roomcoll( struct room a, struct room *b, int margin )
{
	return ( abs(a.x - b->x) < (a.w + b->w) / 2 + margin ) && ( abs(a.y - b->y) < (a.h + b->h) / 2 + margin );
}

struct room* spread( int gx, int gy, float gr, int cnt )
{
	 int i = 0, j, w, h, ok;
	 float ang;
	 struct room *rooms = calloc( cnt, sizeof( struct room ) );

	 while( i < cnt )
	 {
		 int x,y,r;
		ok = 1;
	 	ang = ((rand( ) % 10000) / 10000.0) * 2 * M_PI;
		r = gr* ((rand( ) % 10000) / 10000.0);
		x = gx+ floor( cos(ang) * r );
		y = gy+ floor( sin(ang) * r );
		w = rangedrandom( 2, 10 );
		h = rangedrandom( 2, 10 );

		for( j = 0; j < i; j++ )
			if( roomcoll( mkroom( x, y, w, h, i ), rooms + j, 5 ) )
				ok = 0;

		if( ok )
		{
			rooms[i] = mkroom( x, y, w, h, i );
			i++;
		}
	 }

	 return rooms;
}

void drawroom( struct room* rooms, int cnt )
{
	int i, j, k;

	if( rooms == NULL )
		return;

	for( i = 0; i < cnt; i++ )
	{
		for( j = floor( rooms[i].x - rooms[i].w / 2.0 ) - 1; j < floor( rooms[i].x + rooms[i].w / 2.0 ) + 1; j++ )
		{
			for( k = floor( rooms[i].y - rooms[i].h / 2.0 ) - 1; k < floor( rooms[i].y + rooms[i].h / 2.0 ) + 1; k++ )
			{
					if( j == floor( rooms[i].x - rooms[i].w / 2.0 ) - 1 || j == floor( rooms[i].x + rooms[i].w / 2.0 ) || k  == floor( rooms[i].y - rooms[i].h / 2.0 ) - 1 || k == floor( rooms[i].y + rooms[i].h / 2.0 ) )
					{
						mapputtile( j, k, MAP_LSOLID, tile( "stone wall" ) );
					}
					else
					{
						mapputtile( j, k, MAP_LFLOOR, tile( "stone floor" ) );
					}
			}
		}
	}
}

int genlvl( int gx, int gy, int gr, int room_cnt )
{
	srand( time(NULL) );
	struct room *rooms = spread( gx, gy, gr, room_cnt );

	drawroom( rooms, room_cnt );

return 0;
}
