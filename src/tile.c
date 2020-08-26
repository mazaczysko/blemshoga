#include "tile.h"
#include "utils.h"
#include "log.h"

extern tile *tile_load(const char *path)
{
	char *text = slurp_file(path);
	if (!text)
	{
		log_error("could not load tile '%s'", path);
		return NULL;
	}


}