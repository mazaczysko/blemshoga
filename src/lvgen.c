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

struct net
{
	struct room *r1, *r2;
	float dist;
};

struct corr
{
	int ax, ay;
	int bx, by;
	int mx, my; //Middle
};

int rangedrandom( int min, int max )
{
	return rand( ) % ( max + 1 - min ) + min;
}

int qcmp (const void * a, const void * b)
{
	//Increasing
 	struct net* a2 = ( struct net*)a;
    struct net* b2 = ( struct net*)b;
    if( a2->dist < b2->dist ) return -1;
    else if( a2->dist == b2->dist ) return 0;
    else return 1;
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
		w = rangedrandom( 4, 10 );
		h = rangedrandom( 4, 10 );

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

void drawcorr( struct corr *corr, int cnt )
{
	int i, j;

	if( corr == NULL )
		return;

	//Corridors
	for( i = 0; i < cnt - 1; i++ )
	{
		printf("Corr nr %d: ax=%d, ay=%d, bx=%d, by=%d, mx=%d, my=%d\n", i, corr[i].ax, corr[i].ay, corr[i].bx, corr[i].by, corr[i].mx, corr[i].my );
		printf("Drawing Verticals\n");
		//Vertical
		if( corr[i].mx == corr[i].ax )
		{
			if( corr[i].ay < corr[i].my )
			{
				for( j = corr[i].ay; j <= corr[i].my; j++ )
				{
					mapputtile( corr[i].mx, j, MAP_LFLOOR, tile( "stone floor") );
					if( *maptile( corr[i].mx, j, MAP_LSOLID ) == tile( "stone wall") )
						mapputtile( corr[i].mx, j, MAP_LSOLID, tile( "door (closed)" ) );

					printf("x=%d, y=%d\n", corr[i].mx, j );
				}
			}
			else
			{
				for( j = corr[i].ay; j >= corr[i].my; j-- )
				{
					mapputtile( corr[i].mx, j, MAP_LFLOOR, tile( "stone floor") );
					if( *maptile( corr[i].mx, j, MAP_LSOLID ) == tile( "stone wall") )
						mapputtile( corr[i].mx, j, MAP_LSOLID, tile( "door (closed)" ) );

					printf("x=%d, y=%d\n", corr[i].mx, j );
				}
			}
		}
		else
		{
			if( corr[i].by < corr[i].my )
			{
				for( j = corr[i].by; j <= corr[i].my; j++ )
				{
					mapputtile( corr[i].mx, j, MAP_LFLOOR, tile( "stone floor") );
					if( *maptile( corr[i].mx, j, MAP_LSOLID ) == tile( "stone wall") )
						mapputtile( corr[i].mx, j, MAP_LSOLID, tile( "door (closed)" ) );

					printf("x=%d, y=%d\n", corr[i].mx, j );
				}
			}
			else
			{
				for( j = corr[i].by; j >= corr[i].my; j-- )
				{
					mapputtile( corr[i].mx, j, MAP_LFLOOR, tile( "stone floor") );
					if( *maptile( corr[i].mx, j, MAP_LSOLID ) == tile( "stone wall") )
						mapputtile( corr[i].mx, j, MAP_LSOLID, tile( "door (closed)" ) );

					printf("x=%d, y=%d\n", corr[i].mx, j );
				}
			}
		}

		printf("Drawing Horizontal\n");
		//Horizontal
		if( corr[i].my == corr[i].ay )
		{
			if( corr[i].ax < corr[i].mx )
			{
				for( j = corr[i].ax; j <= corr[i].mx; j++ )
				{
						mapputtile( j, corr[i].my, MAP_LFLOOR, tile( "stone floor") );
						if( *maptile( j, corr[i].my, MAP_LSOLID ) == tile( "stone wall") )
							mapputtile( j, corr[i].my, MAP_LSOLID, tile( "door (closed)" ) );

						printf("x=%d, y=%d\n", j, corr[i].my);
				}
			}
			else
			{
				for( j = corr[i].ax; j >= corr[i].mx; j-- )
				{
						mapputtile( j, corr[i].my,  MAP_LFLOOR, tile( "stone floor") );
						if( *maptile( j, corr[i].my, MAP_LSOLID ) == tile( "stone wall") )
							mapputtile( j, corr[i].my, MAP_LSOLID, tile( "door (closed)" ) );

						printf("x=%d, y=%d\n", j, corr[i].my);
				}
			}
		}
		else
		{
			if( corr[i].bx < corr[i].mx )
			{
				for( j = corr[i].bx; j <= corr[i].mx; j++ )
				{
						mapputtile( j, corr[i].my, MAP_LFLOOR, tile( "stone floor") );
						if( *maptile( j, corr[i].my, MAP_LSOLID ) == tile( "stone wall") )
							mapputtile( j, corr[i].my, MAP_LSOLID, tile( "door (closed)" ) );

						printf("x=%d, y=%d\n", j, corr[i].my);
				}
			}
			else
			{
				for( j = corr[i].bx; j >= corr[i].mx; j-- )
				{
						mapputtile( j, corr[i].my, MAP_LFLOOR, tile( "stone floor") );
						if( *maptile( j, corr[i].my, MAP_LSOLID ) == tile( "stone wall") )
							mapputtile( j, corr[i].my, MAP_LSOLID, tile( "door (closed)" ) );

						printf("x=%d, y=%d\n", j, corr[i].my);
				}
			}
		}
		printf("\n");
	}

	//Borders
	for( i = 0; i < cnt - 1; i++ )
	{
		//Vertical
		if( corr[i].mx == corr[i].ax )
		{
			if( corr[i].ay < corr[i].my )
			{
				for( j = corr[i].ay; j <= corr[i].my + 1; j++ )
				{
					if( *maptile( corr[i].mx - 1, j, MAP_LFLOOR ) == NULL )
						mapputtile( corr[i].mx - 1, j, MAP_LSOLID, tile( "stone wall") );

					if( *maptile( corr[i].mx + 1, j, MAP_LFLOOR ) == NULL )
							mapputtile( corr[i].mx + 1, j, MAP_LSOLID, tile( "stone wall" ) );
				}
			}
			else
			{
				for( j = corr[i].ay; j >= corr[i].my - 1; j-- )
				{
					if( *maptile( corr[i].mx - 1, j, MAP_LFLOOR ) == NULL )
						mapputtile( corr[i].mx - 1, j, MAP_LSOLID, tile( "stone wall") );

					if( *maptile( corr[i].mx + 1, j, MAP_LFLOOR ) == NULL )
							mapputtile( corr[i].mx + 1, j, MAP_LSOLID, tile( "stone wall" ) );
				}
			}
		}
		else
		{
			if( corr[i].by < corr[i].my )
			{
				for( j = corr[i].by; j <= corr[i].my + 1; j++ )
				{
					if( *maptile( corr[i].mx - 1, j, MAP_LFLOOR ) == NULL )
						mapputtile( corr[i].mx - 1, j, MAP_LSOLID, tile( "stone wall") );

					if( *maptile( corr[i].mx + 1, j, MAP_LFLOOR ) == NULL )
							mapputtile( corr[i].mx + 1, j, MAP_LSOLID, tile( "stone wall" ) );
				}
			}
			else
			{
				for( j = corr[i].by; j >= corr[i].my - 1; j-- )
				{
					if( *maptile( corr[i].mx - 1, j, MAP_LFLOOR ) == NULL )
						mapputtile( corr[i].mx - 1, j, MAP_LSOLID, tile( "stone wall") );

					if( *maptile( corr[i].mx + 1, j, MAP_LFLOOR ) == NULL )
							mapputtile( corr[i].mx + 1, j, MAP_LSOLID, tile( "stone wall" ) );
				}
			}
		}

		//Horizontal
		if( corr[i].my == corr[i].ay )
		{
			if( corr[i].ax < corr[i].mx )
			{
				for( j = corr[i].ax; j <= corr[i].mx + 1; j++ )
				{
						if( *maptile( j, corr[i].my - 1, MAP_LFLOOR ) == NULL )
							mapputtile( j, corr[i].my - 1, MAP_LSOLID, tile( "stone wall" ) );

						if( *maptile( j, corr[i].my + 1, MAP_LFLOOR ) == NULL )
							mapputtile( j, corr[i].my + 1, MAP_LSOLID, tile( "stone wall" ) );
				}
			}
			else
			{
				for( j = corr[i].ax; j >= corr[i].mx - 1; j-- )
				{
						if( *maptile( j, corr[i].my - 1, MAP_LFLOOR ) == NULL )
							mapputtile( j, corr[i].my - 1, MAP_LSOLID, tile( "stone wall" ) );

						if( *maptile( j, corr[i].my + 1, MAP_LFLOOR ) == NULL )
							mapputtile( j, corr[i].my + 1, MAP_LSOLID, tile( "stone wall" ) );
				}
			}
		}
		else
		{
			if( corr[i].bx < corr[i].mx )
			{
				for( j = corr[i].bx; j <= corr[i].mx + 1; j++ )
				{
						if( *maptile( j, corr[i].my - 1, MAP_LFLOOR ) == NULL )
							mapputtile( j, corr[i].my - 1, MAP_LSOLID, tile( "stone wall" ) );

						if( *maptile( j, corr[i].my + 1, MAP_LFLOOR ) == NULL )
							mapputtile( j, corr[i].my + 1, MAP_LSOLID, tile( "stone wall" ) );
				}
			}
			else
			{
				for( j = corr[i].bx; j >= corr[i].mx - 1; j-- )
				{

						if( *maptile( j, corr[i].my - 1, MAP_LFLOOR ) == NULL )
							mapputtile( j, corr[i].my - 1, MAP_LSOLID, tile( "stone wall" ) );

						if( *maptile( j, corr[i].my + 1, MAP_LFLOOR ) == NULL )
							mapputtile( j, corr[i].my + 1, MAP_LSOLID, tile( "stone wall" ) );
				}
			}
		}
	}
}

struct net* mknet( struct room *rooms, int cnt)
{
	int i, j, k, c = 0, ok;
	float dist;
	struct net *net = calloc( cnt*(cnt - 1)/2, sizeof( struct net ) );

	for( i = 0; i < cnt; i++ )
	{
		for( j = 0; j < cnt; j++ )
		{
			ok = 1;

			if( i != j )
			{
				for( k = 0; k < c; k++ )
				{
					if( ( rooms[i].id == net[k].r1->id && rooms[j].id == net[k].r2->id ) || ( rooms[i].id == net[k].r2->id && rooms[j].id == net[k].r1->id ) )
					{
						ok = 0;
					}
				}
				if( ok )
				{
					dist = (float) sqrt( pow( rooms[i].x - rooms[j].x, 2.0) + pow( rooms[i].y - rooms[j].y, 2.0 ) );
					net[c].r1 = rooms + i;
					net[c].r2 = rooms + j;
					net[c].dist = dist;
					printf("%d-%d\n", rooms[i].id, rooms[j].id );
					c++;
				}

			}
		}
	}
	printf("\n\n");

	qsort( net, c, sizeof( struct net ), qcmp );
	for( i = 0; i < c; i++ )
		printf("%f\n", net[i].dist );

	return net;
}

struct net *spantree( struct room *rooms, struct net *net, int cnt )
{
	int i, j = 0, k, id;
	struct net *spantree = calloc( cnt - 1, sizeof( struct net ) );

	for( i = 0; i < cnt*(cnt - 1)/2; i++)
	{
		if( net[i].r1->id != net[i].r2->id )
		{
			memcpy( spantree + j, net + i, sizeof( struct net ) );
			printf( "span:%d-%d, %d, %d ", spantree[j].r1->id, spantree[j].r2->id, spantree[j].r1->x, spantree[j].r2->y );
			printf("no cycle\n");
			j++;
			id = net[i].r2->id;
			for( k = 0; k < cnt; k++ )
			{
				if( rooms[k].id == id )
				{
				 	rooms[k].id = net[i].r1->id;
			 	}

			}

		}
		printf( "step: %d\n", i );
		for( int z = 0; z < cnt*(cnt - 1)/2; z++ )
		{
			printf("%d-%d\n", net[z].r1->id, net[z].r2->id );
		}
		if( j == cnt - 1 )
		{
			printf("MSP of %d edges\n", j );
			return spantree;
		}
	}
	printf( "%d\n", j);
	return spantree;
}

struct corr* net2corr( struct net *net, int cnt )
{
	struct corr *corr = calloc( cnt - 1, sizeof( struct corr ) );
	int i;

	for( i = 0; i < cnt - 1; i++ )
	{
		corr[i].ax = floor( net[i].r1->x );
		corr[i].ay = floor( net[i].r1->y );
		corr[i].bx = floor( net[i].r2->x );
		corr[i].by = floor( net[i].r2->y );

		if( (rand( ) % 10000) / 10000.0 < 0.5 )
		{
			corr[i].mx = corr[i].ax;
			corr[i].my = corr[i].by;
		}
		else
		{
			corr[i].mx = corr[i].bx;
			corr[i].my = corr[i].ay;
		}
	}
	return corr;
}


int genlvl( int gx, int gy, int gr, int room_cnt, struct tile *player, struct tile ***pptr )
{
	srand( time(NULL) );
	struct room *rooms = spread( gx, gy, gr, room_cnt );
	struct net *net = mknet( rooms, room_cnt );

	net = spantree( rooms, net, room_cnt);

	struct corr *corr = net2corr( net, room_cnt );

	drawroom( rooms, room_cnt );
	drawcorr( corr, room_cnt );

	// Put the player in the first room
	int px = rooms[0].x;
	int py = rooms[0].y;
	player->ent.x = px;
	player->ent.y = py;
	*pptr = mapputtile(px, py, MAP_LENT, player );
	
	free(net);
	free(rooms);

return 0;
}
