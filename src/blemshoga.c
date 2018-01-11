#include <stdio.h>
#include <inttypes.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "map.h"
#include "tile.h"

struct entity player;

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
				if (  t != NULL && *t != NULL && ( *t )->common.type !=  TILE_VOID )
				{
					sprite = ( *t )->common.sprite;
					if ( sprite != NULL ) al_draw_bitmap( sprite, i * TILE_SIZE, j * TILE_SIZE, 0 );
				}
			}
		}
	}
}


void entitymove( ALLEGRO_EVENT ev, struct entity *entity )
{
	switch ( ev.keyboard.keycode )
	{
		case ALLEGRO_KEY_DOWN:
			if( entity->y + 1 < map.height && entity->y + 1 > 0 )
			{
				mapmovetile( maptoptile( entity->x, entity->y ), entity->x, entity->y + 1 );
				entity->y++;
				map_render( entity->x, entity->y, 16, 16 );
			}
			break;

		case ALLEGRO_KEY_RIGHT:
			if( entity->x + 1 < map.width && entity->x + 1 > 0 )
			{
				mapmovetile( maptoptile( entity->x, entity->y ), entity->x + 1, entity->y );
				entity->x++;
				map_render( entity->x, entity->y, 16, 16 );
			}
			break;

		case ALLEGRO_KEY_LEFT:
			if( entity->x - 1 >= 0 )
			{
				mapmovetile( maptoptile( entity->x, entity->y ), entity->x - 1, entity->y );
				entity->x--;
				map_render( entity->x, entity->y, 16, 16 );
			}
			break;

		case ALLEGRO_KEY_UP:
			if( entity->y - 1 >= 0 )
			{
				mapmovetile( maptoptile( entity->x, entity->y ), entity->x, entity->y - 1);
				entity->y--;
				map_render( entity->x, entity->y, 16, 16 );
			}
			break;

		default:
			break;
	}
}

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
					entitymove( ev, &player );
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
	player.x = 2;
	player.y = 0;
	player.tile = entities;
	map_init( 16, 16, 16 );
	tiles_init( );

	//TODO error checks
	win = al_create_display( map.width * TILE_SIZE, map.height * TILE_SIZE );
	al_set_window_title( win, "blemshoga - development build from " __DATE__ " " __TIME__ );

	//TEMP test
	mapputtile( 0, 0, blocks + 1 );
	mapputtile( 1, 0, blocks + 0 );
	mapputtile( player.x, player.y, player.tile );

	//Enter main game loop
	gameloop( win );

	//TODO error checking
	tiles_destroy( );
	al_destroy_display( win );

	return 0;
}
