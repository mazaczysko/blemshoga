#include <stdio.h>
#include <allegro5/allegro_audio.h>
#include "tile.h"
#include "ent.h"
#include "ai.h"

//Rat AI
static void ai_rat( struct tile ***ent )
{
	static int n = 0;
	//assert( ( ***ent ).snd[0].sound != NULL  );

	entmove( ent, n % 4 == 0 ? 1 : ( n % 4 == 2 ? -1 : 0 ), n % 4 == 1 ? 1 : ( n % 4 == 3 ? -1 : 0 ) );
	n++;
}

static struct
{
	const char *name;
	void ( *handler )( struct tile ***ent );
} handlers[] =
{
	{ "rat", ai_rat }
};
#define HANDLER_CNT ( sizeof( handlers ) / sizeof( handlers[0] ) )

//Function returning handlers by name
void ( *aihandler( const char *name ) )( struct tile ***ent )
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
