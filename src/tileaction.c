#include "tile.h"
#include "tileaction.h"

static void act_door( struct tile **self, struct tile **other, int itype )
{
	if ( itype == ACT_PUSH )
	{
		*self = tile( "door (open)" );
		tilesnd( *self, "door_open" );
		assert( other != NULL );
		assert( *other != NULL );
		( *other )->animframe = 1;
	}
	else if ( itype == ACT_LEAVE )
	{
		*self = tile( "door (closed)" );
		tilesnd( *self, "door_close" );
		assert( other != NULL );
		assert( *other != NULL );
		( *other )->animframe = 0;
	}
}

static void act_vase( struct tile **self, struct tile **other, int itype )
{
	if ( itype == ACT_PUSH )
	{
		tilesnd( *self, "break" );
		( *self )->animframe = 1;
	}
}

static struct
{
	const char *name;
	void ( *handler )( struct tile **self, struct tile **other, int itype );
} handlers[] =
{
	{ "door", act_door },
	{ "vase", act_vase },
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
