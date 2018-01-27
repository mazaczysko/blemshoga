#include <stdio.h>
#include <inttypes.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "map.h"
#include "tile.h"
#include "ent.h"

//TODO replace this with proper solution
struct tile player;
struct tile **pptr;

//Renders given portion of map on screen
//TODO add offset
void map_render( int x, int y, int w, int h )
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
							0,
							0,
							TILE_SIZE,
							TILE_SIZE,
							al_map_rgb( 255, 255, 255 ), //No tint for now
							0,
							0,
							i * TILE_SIZE,
							j * TILE_SIZE,
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

//Wanna run into problems?
//Because that's how you run into problems.
/*
void spawn( int x, int y, struct entity *entity )
{
	struct tile **t = mapfreetile( x, y );
	*t = entity->tile;
	(*t)->entity.entity = entity;
}
*/

//Returns 0 when everything is ok and 1 when some error has occurred
int gameloop( ALLEGRO_DISPLAY *win )
{
	int alive = 1, x = 0, y = 0;
	ALLEGRO_EVENT_QUEUE *queue = NULL;
	ALLEGRO_EVENT ev;

	if ( win == NULL )
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

	//TEMP load font
	ALLEGRO_FONT *font = al_load_font( "./resources/fonts/monospace.ttf", 12, 0 );
	assert( font != NULL );

	while ( alive )
	{
		//Handle events
		while ( al_get_next_event( queue, &ev ) )
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
					break;

				//Ignore unregistered events
				default:
					break;
			}

		}
		
		//TODO all the rendering here
		//TEMP test render
		map_render( x, y, 16, 16 );

		//TEMP debug text
		al_draw_textf( font, al_map_rgba_f(1, 1, 1, 1), 0, 0, 0, "px: %d, py: %d", player.ent.x, player.ent.y );
	
		//Buffer flush
		al_flip_display( );
	}

	al_destroy_event_queue( queue );

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
	
	//TEMP test init
	map_init( 16, 16, 16 );
	tiles_init( "./resources/tiles" );
	
	//TEMP player init
	player.ent.x = 0;
	player.ent.y = 0;
	player.type = TILE_ENTITY;
	player.name = "player";
	player.spritename = "./resources/entities/player.png";
	player.solid = 1;
	player.entity = 1;
	player.sprite = al_load_bitmap( player.spritename );
	pptr = mapputtile( player.ent.x, player.ent.y, &player );
	

	//TODO error checks
	win = al_create_display( map.width * TILE_SIZE, map.height * TILE_SIZE );
	al_set_window_title( win, "blemshoga - dev build from " __DATE__ " " __TIME__ );


	//TEMP
	//Some horizontal wall
	for( int i = 0; i < map.width; i++ )
		mapputtile( i, map.height / 2, tilename( "stone wall" ) );
	
	assert( tilename( "stone wall" ) != NULL );
	
	//Doors
	*maptile( map.width / 2 , map.height / 2, 0 ) = tilename( "stone floor" );;
	*maptile( map.width / 2 , map.height / 2, 1 ) = tilename( "door (open)" );
	*maptile( 2, 2, 1 ) = tilename( "door (open)" );


	//Enter main game loop
	gameloop( win );

	//TODO error checking
	tiles_destroy( );
	al_destroy_display( win );

	return 0;
}
