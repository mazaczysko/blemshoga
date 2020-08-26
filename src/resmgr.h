#ifndef RESMGR_H
#define RESMGR_H

#include "allegro.h"


extern void resmgr_init_bitmap_storage( void );

/**
	Checks whether the image is known to the resource manager
*/
extern bool resmgr_has_image(const char *name);

/**
	Returns pointer to a loaded image (and loads it if necessary)
*/
extern ALLEGRO_BITMAP *resmgr_get_image(const char *name);

/**
	Loads resource by path (and registers it)
*/
extern ALLEGRO_BITMAP *resmgr_load_image(const char *path);

/**
	Unloads resource by name
*/
extern void resmgr_unload_image(const char *name);

#endif