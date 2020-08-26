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

	// Init tiles array
	lua_newtable(L);
	lua_setglobal(L, "TILES");

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