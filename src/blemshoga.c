#include <stdio.h>
#include <inttypes.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "map.h"
#include "tile.h"
#include "lvgen.h"
#include "music.h"
#include "ent.h"
#include "blemshoga.h"

//TODO replace this with proper solution
struct tile player;
struct tile **pptr;

//Sets camera up to follow object
//Camera is limited to map bounds
void camfollow( int *cx, int *cy, int x, int y, int w, int h, int mx, int my )
{
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define CLAMP(x, a, b) (MAX(MIN((x), (b)), (a)))

	const int frame = 3;
	int sx = x - *cx;
	int sy = y - *cy;

	sx = CLAMP(sx, frame, w - frame);
	sy = CLAMP(sy, frame, h - frame);
	*cx = CLAMP(x - sx, 0, map.width - 1 - w);
	*cy = CLAMP(y - sy, 0, map.height - 1 - h);
	
#undef MIN
#undef MAX
#undef CLAMP
}

//Renders given portion of map on screen
void map_render( int x, int y, int w, int h, int offx, int offy )
{
	struct tile ** t;
	int i, j, k;
	ALLEGRO_BITMAP *sprite;
	al_clear_to_color( al_map_rgb( 0, 0, 0) );
	for ( i = 0; i < x + w && i < map.width; i++ )
	{
		for ( j = 0; j < y + h && j < map.height; j++ )
		{
			for ( k = 0; k < map.depth; k++ )
			{
				t = maptile( i, j, k );
				if ( t != NULL && *t != NULL )
				{
					sprite = ( *t )->sprite;
					if ( sprite != NULL )
					{
						al_draw_tinted_scaled_rotated_bitmap_region(
							sprite,
							( *t )->animframe * TILE_SIZE,
							0,
							( *t )->animframe * TILE_SIZE + TILE_SIZE,
							TILE_SIZE,
							al_map_rgb( 255, 255, 255 ), //No tint for now
							0,
							0,
							offx + ( i - x ) * TILE_SIZE,
							offy + ( j - y ) * TILE_SIZE,
							1.0,
							1.0,
							0.0,
							0 );
					}
				}
			}
		}
	}
}


void kbdaction( ALLEGRO_EVENT ev )
{
	switch ( ev.keyboard.keycode )
	{
		case ALLEGRO_KEY_DOWN:
			entmove( &pptr, 0, 1 );
			break;

		case ALLEGRO_KEY_RIGHT:
			entmove( &pptr, 1, 0 );
			break;

		case ALLEGRO_KEY_LEFT:
			entmove( &pptr, -1, 0 );
			break;

		case ALLEGRO_KEY_UP:
			entmove( &pptr, 0, -1 );
			break;

		case ALLEGRO_KEY_M:
			music_toggle( );
			break;

		default:
			break;
	}
}

//Key up actions
void kbuaction( ALLEGRO_EVENT ev )
{
	switch ( ev.keyboard.keycode )
	{
		//Keys that should trigged AI - TEMP
		case ALLEGRO_KEY_DOWN:
		case ALLEGRO_KEY_RIGHT:
		case ALLEGRO_KEY_LEFT:
		case ALLEGRO_KEY_UP:
		case ALLEGRO_KEY_U:
			entai( );
			break;

		default:
			break;
	}
}


//Returns 0 when everything is ok and 1 when some error has occurred
int gameloop( ALLEGRO_DISPLAY *win )
{
	int alive = 1;
	const float fps = 2.f;
	ALLEGRO_EVENT_QUEUE *queue = NULL;
	ALLEGRO_TIMER *timer;
	ALLEGRO_EVENT ev;

	if ( win == NULL )
	{
		fprintf( stderr, "%s: unexpected window error!\n", __FILE__ );
		return 1;
	}

	timer = al_create_timer( 1.0 / fps );
	if ( timer == NULL )
	{
		fprintf( stderr, "%s: error creating timer!\n", __FILE__ );
		return 1;
	}

	queue = al_create_event_queue( );
	if ( queue == NULL )
	{
		fprintf( stderr, "%s: error creating event queue!\n", __FILE__ );
		return 1;
	}

	al_register_event_source( queue, al_get_display_event_source( win ) );
	al_register_event_source( queue, al_get_keyboard_event_source( ) );
	al_register_event_source( queue, al_get_timer_event_source( timer ) );
	al_start_timer( timer );

	while ( alive )
	{
		//Wait for event
		al_wait_for_event( queue, &ev );

		//Handle events
		do
		{
			switch ( ev.type )
			{
				//Quit on window close
				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					alive = 0;
					break;

				//Key down event
				case ALLEGRO_EVENT_KEY_DOWN:
					kbdaction( ev );
					break;

				//Key up event
				case ALLEGRO_EVENT_KEY_UP:
					kbuaction( ev );
					break;

				//Ignore unregistered events
				default:
					break;
			}


		} while ( al_get_next_event( queue, &ev ) );

		//Center camera on player
		int cx, cy;
		camfollow( &cx, &cy, player.ent.x, player.ent.y, 20, 12, 3, 3 );
		map_render( cx, cy, 20, 12, 0, 0 );
		al_flip_display( );
	}

	al_destroy_event_queue( queue );
	al_destroy_timer( timer );

	return 0;
}



int main( int argc, char** argv )
{
	srand( time( NULL ) );

	ALLEGRO_DISPLAY *win;


	//TODO move to special routine
	if( !al_init( ) )
	{
		fprintf(stderr, "%s: error initializing allegro!\n", argv[0] );
		exit( EXIT_FAILURE );
	}

	if( !al_install_keyboard( ) )
	{
		fprintf(stderr, "%s: error installing keyboard!\n", argv[0] );
		exit( EXIT_FAILURE );
	}

	if( !al_init_image_addon( ) )
	{
		fprintf(stderr, "%s: error initializing image addon!\n", argv[0] );
		exit( EXIT_FAILURE );
	}

	if( !al_init_font_addon( ) )
	{
		fprintf(stderr, "%s: error initializing font addon!\n", argv[0] );
		exit( EXIT_FAILURE );
	}

	if( !al_install_audio( ) )
	{
		fprintf(stderr, "%s: error installing audio\n", argv[0] );
		exit( EXIT_FAILURE );
	}

	if( !al_init_acodec_addon( ) )
	{
		fprintf(stderr, "%s: error initializing acodec addon!\n", argv[0] );
		exit( EXIT_FAILURE );
	}

	if( !al_reserve_samples( 16 ) )
	{
		fprintf(stderr, "%s: error reserving samples!\n", argv[0] );
		exit( EXIT_FAILURE );
	}



	//TEMP test init
	map_init( 50, 50, 16 );
	tiles_init( "./resources/tiles" );
	ent_init( "./resources/ent" );

	//TEMP player init
	player.ent.x = 25;
	player.ent.y = 25;
	player.name = "player";
	player.spritename = "./resources/ent/player.png";
	player.solid = 1;
	player.entity = 1;
	player.sprite = al_load_bitmap( player.spritename );
	player.ent.grp = 1 << 0;
	player.ent.hosgrp = ~0;
	player.ent.combat.strength = 5;
	player.ent.combat.attack = 1.25;
	player.active = 1;

	music_init( );
	music_play( );


	if( ( win = al_create_display( 20 * TILE_SIZE, 12 * TILE_SIZE + 64 ) ) == NULL )
	{
		fprintf(stderr, "%s: error creating window!\n", argv[0] );
		return 1;
	}

	//Come on... It cannot crash
	al_set_window_title( win, "blemshoga - Miłosz & Jaca Soft inc. - dev build from " __DATE__ " " __TIME__ );


	//TEMP commented for generator tests
	//int i, j;
	//for ( i = 0; i < map.width; i++ )
	//	for ( j = 0; j < map.height; j++ )
	//		mapputtile( i, j, MAP_LFLOOR, tile( "stone floor" ) );

	
    //
	//mapdroptile( 5, 5, tile( "vase" ) );
	// spawn( "box", 11, 11 );
    //
	// spawn( "rat", 7, 7 );
	// spawn( "rat", 6, 6 );
	// spawn( "rat", 4, 4 );
	// spawn( "rat", 3, 3 );
	// spawn( "rat", 10, 10 );

	// //TEMP
	// //Some horizontal wall
	// for( int i = 0; i < map.width; i++ )
	// 	mapputtile( i, 8, MAP_LSOLID, tile( "stone wall" ) );
	// //Doors
	// mapputtile( 8, 8, MAP_LSOLID, tile( "door (open)" ) );
	// mapputtile( 2, 2, MAP_LSOLID, tile( "door (open)" ) );

	genlvl( 25, 25, 20, 7, &player, &pptr );

	spawn( "rat", player.ent.x - 1, player.ent.y - 1 );
	spawn( "rat", player.ent.x + 1, player.ent.y);
	spawn( "box", player.ent.x - 2, player.ent.y);
	mapdroptile( player.ent.x, player.ent.y + 2, tile("vase"));

	//Enter main game loop
	gameloop( win );

	//TODO error checking
	tiles_destroy( );
	music_destroy( );
	al_destroy_display( win );

	return 0;
}
