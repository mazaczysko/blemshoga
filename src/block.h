#ifndef BLOCK_H
#define BLOCK_H

#include <inttypes.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "tile.h"

struct block
{
	//TODO add common tile struct right here

	uint16_t id;
	const char *name;

	//Flags
	unsigned int ground : 1; //Is this tile a ground tile?
	unsigned int solid : 1; //Can entities walk through this tile?
	unsigned int flammable : 1; //Can this tile be set on fire?
	unsigned int : 0;

	//Interaction callbacks
	void ( *entityinteraction )( struct tile *entity, struct tile *tile );

	const char *spritename;
	ALLEGRO_BITMAP *sprite;
};

extern struct block blocks[];
extern int blocks_init( );
extern int blocks_destroy( );


#endif
