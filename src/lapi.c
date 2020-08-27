#include "lapi.h"
#include "log.h"

lua_State *L = NULL;

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

	// Load blemshoga.lua
	if (luaL_dofile(L, "resources/lua/blemshoga.lua"))
	{
		log_error("blemshoga.lua error: %s", lua_tostring(L, -1));
		lua_pop(L, 1);
		return 1;
	}

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