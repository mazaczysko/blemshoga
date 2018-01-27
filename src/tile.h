#ifndef TILE_H
#define TILE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#define TILE_SIZE 32

#define TILE_UNDEF  0
#define TILE_BLOCK  1
#define TILE_ITEM   2
#define TILE_ENTITY 3

//Interaction types
#define INT_PUSH 1
#define INT_LEAVE 2

struct tile
{
	//Common specification
	unsigned int id;
	int type;
	char *name;
	char *spritename;
	ALLEGRO_BITMAP *sprite;
	int animframe;
		
	//Interaction handler
	void ( *action )( struct tile **self, struct tile **other, int itype );
	char *actionname; 

	//Flags
	unsigned int ground : 1; //Is this tile a ground tile?
	unsigned int solid : 1; //Can entities walk through this tile?
	unsigned int flammable : 1; //Can this tile be set on fire?
	unsigned int entity : 1; //Is this tile a living thing?
	int : 0;

	struct
	{
		int x, y, z;
	} ent;
};


extern int tiles_init( );
extern int tiles_destroy( );
extern struct tile *tilename( const char *name );

#endif
