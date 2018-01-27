#ifndef TILE_H
#define TILE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

//Tile size in pixels
#define TILE_SIZE 32

//Tile types
#define TILE_UNDEF  0
#define TILE_BLOCK  1
#define TILE_ITEM   2
#define TILE_ENTITY 3

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
	unsigned int : 0;

	struct
	{
		int x, y, z;
	} ent;
};


extern int tiles_init( );
extern void tiles_destroy( );
extern struct tile *tile( const char *name );

#endif
