#ifndef UTILS_H
#define UTILS_H

#include <lua.h>

#define TEXT_RESET    "\x1B[0m"
#define TEXT_RED      "\x1B[31m"
#define TEXT_GREEN    "\x1B[32m"
#define TEXT_YELLOW   "\x1B[33m"
#define TEXT_BLUE     "\x1B[34m"
#define TEXT_MAGENTA  "\x1B[35m"
#define TEXT_CYAN     "\x1B[36m"
#define TEXT_WHITE    "\x1B[37m"

#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

/**
	Reads entire file into a string

	Returns NULL on error
*/
extern char *slurp_file(const char *path);

extern int safe_lua_dofile(lua_State *L, const char *path);

#endif