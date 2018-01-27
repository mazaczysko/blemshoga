#include "tile.h"
#include "tileaction.h"

static void act_door( struct tile **self, struct tile **other, int itype )
{
	if ( itype == ACT_PUSH )
		*self = tile( "door (open)" );
	else if ( itype == ACT_LEAVE )
		*self = tile( "door (closed)" );
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
	if ( name == NULL ) return NULL;
	
	for ( i = 0; i < HANDLER_CNT; i++ )
	{
		if ( !strcmp( handlers[i].name, name ) )
			return handlers[i].handler;
	}
	return NULL;
}
