#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "tile.h"

void doorint( struct tile **tile, int whence )
{
	if( whence )
		*tile = blocks + 3;
	else
		*tile = blocks + 2;
}

struct tile blocks[] =
{
	{ { 0, TILE_BLOCK, "stone floor", "./resources/blocks/floor_stone.png", NULL }, { 1, 0, 0, NULL } },
	{ { 1, TILE_BLOCK, "stone wall", "./resources/blocks/wall_stone.png", NULL }, { 0, 1, 0, NULL } },
	{ { 2, TILE_BLOCK, "closed door", "./resources/blocks/door_closed.png", NULL }, { 0, 1, 0, &doorint } },
	{ { 3, TILE_BLOCK, "opened door", "./resources/blocks/door_opened.png", NULL }, { 0, 0, 0, &doorint } },
};
#define BLOCK_CNT ( sizeof( blocks ) / sizeof( blocks[0] ) )

struct tile entities[] =
{
	{ { 0, TILE_ENTITY, "player", "./resources/entities/player.png", NULL }, { 0, 1, 1, NULL } },
};
#define ENTITY_CNT ( sizeof( entities ) / sizeof( entities[0] ) )

int tiles_init( )
{
	unsigned int i;

	//TODO error checking
	for ( i = 0; i < BLOCK_CNT; i++ )
		blocks[i].common.sprite = al_load_bitmap( blocks[i].common.spritename );

	for ( i = 0; i < ENTITY_CNT; i++ )
		entities[i].common.sprite = al_load_bitmap( entities[i].common.spritename );

	return 0;
}

int tiles_destroy( )
{
	unsigned int i;

	//TODO error checking
	for( i = 0; i < BLOCK_CNT; i++ )
		al_destroy_bitmap( blocks[i].common.sprite );

	for( i = 0; i < ENTITY_CNT; i++ )
	 	al_destroy_bitmap( entities[i].common.sprite );

	return 0;
}
