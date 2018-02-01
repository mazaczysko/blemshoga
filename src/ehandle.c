#include "ehandle.h"
#include "tile.h"
#include "map.h"

static void std_death( struct tile ***e )
{
	assert( e != NULL );
	tilesnd( **e, "death" );
	mapdroptile( (**e)->ent.x, (**e)->ent.y, tile( "blood" ) ); 
	
}

static void std_hit( struct tile ***e )
{
	tilesnd( **e, "smash" );
}

static struct
{
	const char *name;
	void ( *handler )( struct tile ***self  );
} handlers[] =
{
	{ "std_death", std_death },
	{ "std_hit", std_hit },
};
#define HANDLER_CNT ( sizeof( handlers ) / sizeof( handlers[0] ) )

//Function returning handlers by name
void ( *ehandler( const char *name ) )( struct tile ***self )
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
