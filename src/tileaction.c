#include "tile.h"
#include "tileaction.h"

static void act_door( struct tile **self, struct tile **other, int itype )
{
	if ( itype == INT_PUSH )
		*self = tilename( "door (open)" );
	else if ( itype == INT_LEAVE )
		*self = tilename( "door (closed)" );
}

static struct
{
	const char *name;
	void ( *handler )( struct tile **self, struct tile **other, int itype );
} handlers[] =
{
	{ "door", act_door }
};
#define HANDLER_CNT ( sizeof( handlers ) / sizeof( handlers[0] ) )

//Function returning handlers by name
void ( *acthandler( const char *name ) )( struct tile **self, struct tile **other, int itype )
{
	int i;
	for ( i = 0; i < HANDLER_CNT; i++ )
	{
		if ( !strcmp( handlers[i].name, name ) )
			return handlers[i].handler;
	}
	return NULL;
}
