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
#include "ent.h"
#include "blemshoga.h"

//TODO replace this with proper solution
struct tile player;
struct tile **pptr;

//Sets camera up to follow object
//Camera is limited to map bounds
void camfollow( int *cx, int *cy, int x, int y, int w, int h, int mx, int my )
{
	//TODO enfancy this stuff
	//if ( *cx > x - mx - w ) *cx = x - mx - w;
	//if ( *cx < x + mx - w ) *cx = x + mx - w;
	*cx = x - w / 2;
	*cy = y - h / 2;

	//Bounds
	if ( *cx < 0 ) *cx = 0;
	if ( *cy < 0 ) *cy = 0;
	if ( *cx + w >= map.width ) *cx = map.width - 1 - w;
	if ( *cy + h >= map.height ) *cy = map.height - 1 - h;
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
		//TODO error handler
		return 1;
	}

	timer = al_create_timer( 1.0 / fps );
	if ( timer == NULL )
	{
		//TODO error handler
		return 1;
	}

	queue = al_create_event_queue( );
	if ( queue == NULL )
	{
		//TODO error handler
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
					entai( );
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

int main( )
{
	srand( time( NULL ) );

	ALLEGRO_DISPLAY *win;


	//TODO error checks - it crashes when display is not present, yay!
	al_init( );
	al_install_keyboard( );
	al_init_image_addon( );
	al_init_font_addon( );
	al_init_ttf_addon( );

	assert( al_install_audio( ) );
	assert( al_init_acodec_addon( ) );
	assert( al_reserve_samples( 16 ) );

	//TEMP test init
	map_init( 32, 32, 16 );
	tiles_init( "./resources/tiles" );
	ent_init( "./resources/ent" );

	//TEMP player init
	player.ent.x = 0;
	player.ent.y = 0;
	player.name = "player";
	player.spritename = "./resources/ent/player.png";
	player.solid = 1;
	player.entity = 1;
	player.sprite = al_load_bitmap( player.spritename );



	//TODO error checks
	win = al_create_display( 20 * TILE_SIZE, 12 * TILE_SIZE + 64 );
	al_set_window_title( win, "blemshoga - dev build from " __DATE__ " " __TIME__ );

	int i, j;
	for ( i = 0; i < map.width; i++ )
		for ( j = 0; j < map.height; j++ )
			mapputtile( i, j, MAP_LFLOOR, tile( "stone floor" ) );

	pptr = mapputtile( player.ent.x, player.ent.y, MAP_LENT, &player );

	spawn( "rat", 7, 7 );
	spawn( "rat", 6, 6 );
	spawn( "rat", 4, 4 );
	spawn( "rat", 3, 3 );
	spawn( "rat", 10, 10 );


	//TEMP
	//Some horizontal wall
	for( int i = 0; i < map.width; i++ )
		mapputtile( i, 8, MAP_LSOLID, tile( "stone wall" ) );
	//Doors
	mapputtile( 8, 8, MAP_LSOLID, tile( "door (open)" ) );
	mapputtile( 2, 2, MAP_LSOLID, tile( "door (open)" ) );



	//Enter main game loop
	gameloop( win );

	//TODO error checking
	tiles_destroy( );
	al_destroy_display( win );

	return 0;
}
