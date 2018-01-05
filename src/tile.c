#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "tile.h"
struct tile blocks[] =
{
	{ { 0, TILE_BLOCK, "stone floor", "./resources/blocks/floor_stone.png", NULL }, { 1, 0, 0, NULL } },
	{ { 1, TILE_BLOCK, "stone wall", "./resources/blocks/wall_stone.png", NULL }, { 0, 1, 0, NULL } },
	//{ 1, "stone wall",      0,   1,   0,   NULL, "./resources/blocks/wall_stone.png",       NULL },
};
#define BLOCK_CNT ( sizeof( blocks ) / sizeof( blocks[0] ) )

int tiles_init( )
{
	unsigned int i;

	//TODO error checking
	for ( i = 0; i < BLOCK_CNT; i++ )
		blocks[i].common.sprite = al_load_bitmap( blocks[i].common.spritename );

	return 0;
}

int tiles_destroy( )
{
	unsigned int i;

	//TODO error checking
	for( i = 0; i < BLOCK_CNT; i++ )
		al_destroy_bitmap( blocks[i].common.sprite );

	return 0;
}
