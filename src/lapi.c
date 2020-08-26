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

	log_debug("Lua init done");

	return 0;	
}

void lapi_destroy(void)
{
	lua_close(L);
}