#include <stdio.h>
#include <inttypes.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "map.h"
#include "tile.h"
#include "block.h"


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
				switch ( MAP( i, j, k ).type )
				{
					case TILE_BLOCK:
						sprite = ((struct block*) MAP( i, j, k ).tile)->sprite;
						break;

					default:
						sprite = NULL;
						break;
				}

				if ( sprite != NULL ) al_draw_bitmap( sprite, i * TILE_SIZE, j * TILE_SIZE, 0 );
			}
		}
	}
}

int main( )
{
	ALLEGRO_KEYBOARD_STATE kbstate;
	ALLEGRO_DISPLAY *win;

	//TODO error checks
	al_init( );
	al_install_keyboard( );
	al_init_image_addon( );

	map_init( 16, 16, 16 );
	blocks_init( );

	win = al_create_display( map.width * TILE_SIZE, map.height * TILE_SIZE );
	al_set_window_title( win, "blemshoga - development build from " __DATE__ " " __TIME__ );

	MAP( 0, 0, 15 ).type = TILE_BLOCK;
	MAP( 0, 0, 15 ).tile = blocks + 1;

	//Game loop
	while ( 1 )
	{
		map_render( 0, 0, 16, 16 );
		al_flip_display( );

		al_get_keyboard_state( &kbstate );
		if( al_key_down( &kbstate, ALLEGRO_KEY_ESCAPE ) )
			break;

	}

	blocks_destroy( );

	return 0;
}
