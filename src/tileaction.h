#ifndef TILEACTION_H
#define TILEACTION_H

#include "tile.h"

//Interaction types
#define ACT_PUSH 1
#define ACT_LEAVE 2

extern void ( *acthandler( const char *name ) )( struct tile **self, struct tile **other, int itype );

#endif
