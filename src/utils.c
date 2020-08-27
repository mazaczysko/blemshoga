#include "utils.h"
#include "log.h"
#include "lapi.h"
#include <stdio.h>
#include <stdlib.h>


char *slurp_file(const char *path)
{
	FILE *f = fopen(path, "rt");
	if (!f) return NULL;

	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *buf = calloc(size + 1, sizeof(char));
	if (!buf) return NULL;

	fread(buf, sizeof(char), size, f);

	fclose(f);	
	return buf;
}

int safe_lua_dofile(lua_State *L, const char *path)
{
	log_debug("running '%s'", path);
	int err = luaL_dofile(L, path);
	if (err)
	{
		log_error("%s: %s", path, lua_tostring(L, -1));
		lua_pop(L, 1);
	}

	return err;
}