#ifndef TILE_H
#define TILE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>

//Tile size in pixels
#define TILE_SIZE 32

struct tilesnd
{
	ALLEGRO_SAMPLE *sound;
	char *name;
};

struct tile
{
	//Common specification
	unsigned int id;
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

	//Sounds
	struct tilesnd *snd;
	int sndcnt;

	struct
	{
		struct tile ***handle;
		char *ainame;
		void ( *ai )( struct tile ***ent );

		int maxhp, hp;
		int x, y, z;
	} ent;
};


extern int tiles_init( );
extern void tiles_destroy( );
extern struct tile *tile( const char *name );
extern struct tilesnd *tilesnd( struct tile *t, const char *name );

#endif
