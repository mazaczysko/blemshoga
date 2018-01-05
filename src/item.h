#ifndef ITEM_H
#define ITEM_H

struct item
{
	//TODO add common tile data here

	int id;
	const char *name;

	const char *spritename;
	ALLEGRO_BITMAP *sprite;

};

#endif
