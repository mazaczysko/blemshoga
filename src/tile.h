#ifndef TILE_H
#define TILE_H

#include "allegro.h"
#include "lapi.h"

/**
*/
typedef struct tile
{
	ALLEGRO_BITMAP *sprite;
	
	char *lua_source;
	char *name;

} tile;

/**
	Loads a tile based on Lua file
*/
extern tile *tile_load(const char *path);


#endif