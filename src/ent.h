#ifndef ENT_H
#define ENT_H

#include "tile.h"

extern int ent_init( const char *path );
extern void entmove( struct tile ***eptr, int dx, int dy );
extern void spawn( const char *name, int x, int y );
extern void entai( );

#endif
