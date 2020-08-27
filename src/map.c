#include "map.h"
#include "allegro.h"
#include "utils.h"

void map_get_tile_stack( lua_State *L, int x, int y )
{
	lua_getglobal(L, "MAP");
	lua_pushinteger(L, x);
	lua_pushinteger(L, y);
	lua_call(L, 2, 1);
}

int map_get_tile_stack_size( lua_State *L, int x, int y )
{
	map_get_tile_stack( L, x, y );
	lua_len( L, -1 );
	lua_replace( L, -2 );
	int len = lua_tointeger( L, -1 );
	lua_pop( L, 1 );
	return len;
}

void map_get_tile( lua_State *L, int x, int y, int z )
{
	map_get_tile_stack( L, x, y );
	lua_gettable( L, z + 1 );
	lua_replace( L, -2 );
}
