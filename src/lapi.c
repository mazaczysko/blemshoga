#include "lapi.h"
#include "log.h"
#include "tile.h"

lua_State *L = NULL;

static int lapi_loadtile(lua_State *L)
{
	const char *name = lua_tostring(L, -1);
	log_debug("test: %s", name);
	tile_load(name);
	lua_pop(L, 1);
	return 0;
}

int lapi_init(void)
{
	// Init Lua
	L = luaL_newstate();
	if (!L)
	{
		log_error("lua_open() failed");
		return 1;
	}
	luaL_openlibs(L);

	// C functions
	lua_pushcfunction(L, lapi_loadtile);
	lua_setglobal(L, "load_tile");

	log_debug("Lua init done");
	return 0;	
}

lua_State *lapi_get(void)
{
	return L;
}

void lapi_destroy(void)
{
	lua_close(L);
}