#include "tile.h"
#include <string.h>
#include "utils.h"
#include "log.h"
#include "resmgr.h"

static int tile_destroy(lua_State *L)
{
	tile *t = lua_touserdata(L, -1);

	if (t)
	{
		free(t->lua_source);
	}

	lua_pop(L, 1);
	return 0;
}

extern tile *tile_load(const char *path)
{
	char *text = slurp_file(path);
	if (!text)
	{
		log_error("could not load tile '%s'", path);
		return NULL;
	}

	// Acquire Lua context
	lua_State *L = lapi_get();

	// Set the global `tile` that will be written to to nil
	lua_pushnil(L);
	lua_setglobal(L, "tile");

	// Run the Lua code
	if (luaL_dostring(L, text))
	{
		log_error("Lua error: %s", lua_tostring(L, -1));
		lua_pop(L, 1);
		return NULL;
	}

	// Create userdata
	lua_getglobal(L, "tile");
	tile *t = lua_newuserdata(L, sizeof(tile));
	t->lua_source = text;
	
	// Create metatable for the userdata
	lua_newtable(L);
	lua_pushcfunction(L, tile_destroy);
	lua_setfield(L, -2, "__gc");
	lua_setmetatable(L, -2);

	// Add userdata to the tile
	lua_setfield(L, -2, "c_data");

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

