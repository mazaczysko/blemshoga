#ifndef LAPI_H
#define LAPI_H

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

extern int lapi_init(void);
extern lua_State *lapi_get(void);
extern void lapi_destroy(void);

#endif