#ifndef MAP_H
#define MAP_H

#include <lua.h>

//! Returns tile stack on the Lua stack
extern void map_get_tile_stack(lua_State *L, int x, int y); 

//! Returns tile_stack_size
extern int map_get_tile_stack_size( lua_State *L, int x, int y );

//! Returns tile 
extern void map_get_tile( lua_State *L, int x, int y, int z );

#endif