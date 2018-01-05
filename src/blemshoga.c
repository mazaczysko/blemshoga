#include <stdio.h>
#include <inttypes.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "map.h"
#include "tile.h"

//Renders given portion of map on screen
//TODO add offset
void map_render( int x, int y, int w, int h )
{
	int i, j, k;
	ALLEGRO_BITMAP *sprite;

	for ( i = 0; i < x + w && i < map.width; i++ )
	{
		for ( j = 0; j < y + h && j < map.height; j++ )
		{
			for ( k = 0; k < map.depth; k++ )
			{
				if ( MAP( i, j, k ) != NULL && MAP( i, j, k )->common.type != TILE_VOID )
				{
					sprite = MAP( i, j, k )->common.sprite;
					if ( sprite != NULL ) al_draw_bitmap( sprite, i * TILE_SIZE, j * TILE_SIZE, 0 );
				}
			}
		}
	}
}


//Returns 0 when everything is ok and 1 when some error has occurred
int gameloop( ALLEGRO_DISPLAY *win )
{
	int alive = 1;
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
		map_render( 0, 0, 16, 16 );

		//Buffer flush
		al_flip_display( );
	}

	al_destroy_event_queue( queue );

	return 0;
}

int main( )
{
	ALLEGRO_DISPLAY *win;

	//TODO error checks - it crashes when display is not present, yay!
	al_init( );
	al_install_keyboard( );
	al_init_image_addon( );

	//TEMP test init
	map_init( 16, 16, 16 );
	tiles_init( );

	//TODO error checks
	win = al_create_display( map.width * TILE_SIZE, map.height * TILE_SIZE );
	al_set_window_title( win, "blemshoga - development build from " __DATE__ " " __TIME__ );

	//TEMP test
	MAP( 0, 0, 15 ) = blocks + 1;
	MAP( 1, 0, 15 ) = blocks + 0;

	printf( "%p", MAP(0,0,15)->common.sprite );

	//Enter main game loop
	gameloop( win );

	//TODO error checking
	tiles_destroy( );
	al_destroy_display( win );

	return 0;
}
