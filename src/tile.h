#ifndef TILE_H
#define TILE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#define TILE_SIZE 32

#define TILE_VOID   0
#define TILE_BLOCK  1
#define TILE_ITEM   2
#define TILE_ENTITY 3

struct tile
{
	//Common specification
	struct
	{
		unsigned int id;
		unsigned int type;
		const char *name;
		const char *spritename;
		ALLEGRO_BITMAP *sprite;
	} common;

	//Block specification
	struct
	{
		//Flags
		unsigned int ground : 1; //Is this tile a ground tile?
		unsigned int solid : 1; //Can entities walk through this tile?
		unsigned int flammable : 1; //Can this tile be set on fire?
		unsigned int : 0;

		//Interaction callbacks
		void ( *entityinteraction )( struct tile *entity, struct tile *tile );
	} block;


};

struct
{
	int x;
	int y;

	struct tile* tile;

} player;


extern struct tile blocks[];
extern struct tile entities[];
extern int tiles_init( );
extern int tiles_destroy( );

#endif
