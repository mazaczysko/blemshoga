#ifndef ITEM_H
#define ITEM_H

struct item
{
	//TODO add common tile data here

	uint16_t id;
	const char *name;

	const char *spritename;
	ALLEGRO_BITMAP *sprite;

};

#endif
