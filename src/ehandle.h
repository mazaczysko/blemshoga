#ifndef EHANDLE_H
#define EHANDLE_H
#include "tile.h"
extern void ( *ehandler( const char *name ) )( struct tile ***self );

#endif
