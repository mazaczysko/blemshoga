#include <stdio.h>
#include "block.h"

struct block blocks[] = {
							{ 0, "air",        0, 0, 0, NULL, "./resources/blocks/air.png",		  NULL },
							{ 1, "stone wall", 0, 1, 0, NULL, "./resources/blocks/wall_stone.png", NULL }
						};

#define BLOCK_CNT ( sizeof(blocks)/sizeof(blocks[0]) )

int blocks_init( )
{
	int i;

	//TODO error checking
	for( i = 0; i < BLOCK_CNT; i++ )
	{
		blocks[i].sprite = al_load_bitmap( blocks[i].spritename );
	}

return 0;
}

int blocks_destroy( )
{
	int i;

	//TODO error checking
	for( i = 0; i < BLOCK_CNT; i++ )
		al_destroy_bitmap( blocks[i].sprite );

return 0;
}
