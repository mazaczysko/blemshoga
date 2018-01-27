#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "tile.h"

void door_int( struct tile **self, struct tile **other, int itype )
{
	if ( itype == INT_PUSH )
	{
		*self = blocks + 3;
	}
	else if ( itype == INT_LEAVE )
	{
		*self = blocks + 2;
	}
}

//TODO learn f*cking english
/*
void hit( struct tile **tile, struct entity *attacer )
{
	(*tile)->entity.entity->hp -= attacer->dmg;
	if( (*tile)->entity.entity->hp <= 0 )
		*tile = blocks + 4;
}
*/

//TODO load from json (possibly use Lua)
struct tile blocks[] =
{
	{ 0, TILE_BLOCK, "stone floor", "./resources/blocks/floor_stone.png", NULL, NULL, 1, 0, 0 },
	{ 1, TILE_BLOCK, "stone wall", "./resources/blocks/wall_stone.png", NULL, NULL, 0, 1, 0 },
	{ 2, TILE_BLOCK, "door (closed)", "./resources/blocks/door_closed.png", NULL, door_int, 0, 1, 0 },
	{ 3, TILE_BLOCK, "door (open)", "./resources/blocks/door_opened.png", NULL, door_int, 0, 0, 0 },
	{ 4, TILE_BLOCK, "dead", "./resources/blocks/dead.png", NULL, NULL, 1, 0, 0 },
};
#define BLOCK_CNT ( sizeof( blocks ) / sizeof( blocks[0] ) )

//TODO fix this
struct tile entities[] =
{
};
#define ENTITY_CNT ( sizeof( entities ) / sizeof( entities[0] ) )

//TEMP
struct tile *tile_byname( const char *name )
{
	for ( int i = 0; i < BLOCK_CNT; i++ )
	{
		if ( !strcmp( blocks[i].name, name ) ) return blocks + i;
	}
	return NULL;
}

int tiles_init( )
{
	unsigned int i;

	//TODO error checking
	for ( i = 0; i < BLOCK_CNT; i++ )
		blocks[i].sprite = al_load_bitmap( blocks[i].spritename );

	for ( i = 0; i < ENTITY_CNT; i++ )
		entities[i].sprite = al_load_bitmap( entities[i].spritename );

	return 0;
}

int tiles_destroy( )
{
	unsigned int i;

	//TODO error checking
	for( i = 0; i < BLOCK_CNT; i++ )
		al_destroy_bitmap( blocks[i].sprite );

	for( i = 0; i < ENTITY_CNT; i++ )
	 	al_destroy_bitmap( entities[i].sprite );

	return 0;
}
