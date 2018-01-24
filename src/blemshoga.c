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
		if ( !entitypass( entity->x, entity->y + 1 ) && mapmovetile( maptoptile( entity->x, entity->y ), entity->x, entity->y + 1 ) != NULL )
				entity->y++;
			break;

		case ALLEGRO_KEY_RIGHT:
			if ( !entitypass( entity->x + 1, entity->y ) && mapmovetile( maptoptile( entity->x, entity->y ), entity->x + 1, entity->y ) != NULL )
				entity->x++;
				break;


		case ALLEGRO_KEY_LEFT:
			if ( !entitypass( entity->x - 1, entity->y ) && mapmovetile( maptoptile( entity->x, entity->y ), entity->x - 1, entity->y ) != NULL )
				entity->x--;

			break;

		case ALLEGRO_KEY_UP:
			if ( !entitypass( entity->x, entity->y - 1 ) && mapmovetile( maptoptile( entity->x, entity->y ), entity->x, entity->y - 1  ) != NULL )
				entity->y--;

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
	srand( time( NULL ) );

	ALLEGRO_DISPLAY *win;


	//TODO error checks - it crashes when display is not present, yay!
	al_init( );
	al_install_keyboard( );
	al_init_image_addon( );

	//TEMP test init
	map_init( 16, 16, 16 );
	tiles_init( );
	player.x = map.width / 2;
	player.y = map.height / 2 + 2 ;
	player.tile = entities;

	//TODO error checks
	win = al_create_display( map.width * TILE_SIZE, map.height * TILE_SIZE );
	al_set_window_title( win, "blemshoga - development build from " __DATE__ " " __TIME__ );

	//TEMP test
	// mapputtile( 0, 0, blocks + 1 );
	// for( int i = 1; i < map.width; i++ )
	// 	mapputtile( i, 0, blocks + 0 );
    //
	 for( int i = 0; i < map.width; i++ )
	 	mapputtile( i, map.height / 2, blocks + 1 );
    //
	// for( int i = 3; i < map.height; i++ )
	// 	mapputtile( map.width/2 - 1, i, blocks + 1 );

	mapmakeroom( 0 , 0, 5, 5, 1 );	//South
	mapmakeroom( 0 , map.height , 6, 6, 0 );	//North
	mapmakeroom( map.width - 5, map.height - 5, 5, 5, 3 );	//East
	mapmakeroom( map.width , 0, 6, 6, 2 );	//West

	*maptile( map.width / 2 , map.height / 2, 0 ) = blocks;
	*maptile( map.width / 2 , map.height / 2, 1 ) = blocks + 2;


	mapputtile( player.x, player.y, player.tile );

	//Enter main game loop
	gameloop( win );

	//TODO error checking
	tiles_destroy( );
	al_destroy_display( win );

	return 0;
}
