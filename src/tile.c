#include "tile.h"
#include <string.h>
#include "utils.h"
#include "log.h"
#include "resmgr.h"

extern tile *tile_load(const char *path)
{
	char *text = slurp_file(path);
	if (!text)
	{
		log_error("could not load tile '%s'", path);
		return NULL;
	}

	tile *t = calloc(1, sizeof(tile));
	t->lua_source = text;

	// Acquire Lua context
	lua_State *L = lapi_get();

	// Set the global `tile` that will be written to to nil
	lua_pushnil(L);
	lua_setglobal(L, "tile");

	// Run the Lua code
	if (luaL_dostring(L, t->lua_source))
	{
		log_error("Lua error: %s", lua_tostring(L, -1));
		lua_pop(L, 1);
		return NULL;
	}

	// Get name
	lua_getglobal(L, "tile");
	lua_getfield(L, -1, "name");
	t->name = strdup(lua_tostring(L, -1));
	lua_pop(L, 2);

	// Put the tile in the global TILES table
	lua_getglobal(L, "TILES");
	lua_getglobal(L, "tile");
	lua_setfield(L, -2, t->name);
	lua_pop(L, 1);

	// Preload all sprites
	lua_getglobal(L, "tile");
	lua_getfield(L, -1, "sprites");
	if (!lua_istable(L, -1))
	{
		log_warning("tile does not declare sprites!");
	}
	else
	{
		log_debug("preloading sprites for '%s'", t->name);
		lua_pushnil(L);
		while (lua_next(L, -2))
		{
			// Key is now at -2
			// Value is now at -1
			const char *path = lua_tostring(L, -1);
			resmgr_load_bitmap(path);
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
	}

	log_info("loaded tile '%s'", path);
	log_debug("loaded name: %s", t->name);

	return t;
}

extern void tile_destroy(tile *t)
{
	if (t)
	{
		free(t->lua_source);
		free(t);
	}
}