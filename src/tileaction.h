#ifndef TILEACTION_H
#define TILEACTION_H

#include "tile.h"

extern void ( *acthandler( const char *name ) )( struct tile **self, struct tile **other, int itype );

#endif
