#ifndef ENTITY_H
#define ENTITY_H

struct entity
{
	//TODO add common tile data here

	int id;
	const char *name;

	const char *spritename;
	ALLEGRO_BITMAP *sprite;
};

#endif
