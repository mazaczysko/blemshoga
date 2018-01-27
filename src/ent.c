#include "map.h"
#include "tile.h"
#include "ent.h"
#include "tileaction.h"
#include <assert.h>

struct tile *entities;


//Moves entity
//Map contains pointer to the actual entity data
//Since we must know where the entity is, we keep a pointer to its tile on the map
//Because this function can alter entity location, it has to be able to modify pointer decribed above
void entmove( struct tile ***eptr, int dx, int dy )
{
	struct tile **t, **src, **dest, *e, **etile;
	int i, sx, sy;

	//For debug, let's have some assertions instead
	//Programmer should know that things aren't really fine if he attempts to move stone wall
	assert( eptr != NULL );
	assert( *eptr != NULL );
	assert( **eptr != NULL );
	assert( ( **eptr )->entity != 0 );
	
	//This is so stupid, yet it has to be here :(
	if ( eptr == NULL || *eptr == NULL || **eptr == NULL || **eptr == NULL ) return;
	etile = *eptr;
	e = *etile;
	if ( !e->entity ) return;


	//dx, dy - destination xy
	//sx, sy - source xy
	sx = e->ent.x;
	sy = e->ent.y;
	dx += sx;
	dy += sy;
	if ( dx < 0 || dy < 0 || dx >= map.width || dy >= map.height ) return;
	
	//TODO skip move after certain kinds of interactions
	//TODO call proper fight system function if destination is an entity
	
	//Interact with all tiles ahead of entity
	for ( i = 0; i < map.depth; i++ )
	{
		t = maptile( dx,dy, i );
		if ( t == NULL || *t == NULL || ( *t )->action == NULL ) continue;
		( *t )->action( t, etile, ACT_PUSH );
	}
	
	//Attempt move
	if ( mapissolid( dx, dy ) ) return;
	if ( ( dest = mapfreetile( dx, dy ) ) == NULL ) return;
	e->ent.x = dx;
	e->ent.y = dy;
	src = etile;
	*dest = *etile;
	*src = NULL;
	*eptr = dest;
	
	//Leave interaction
	//Interact with all tiles ahead of entity
	for ( i = 0; i < map.depth; i++ )
	{
		t = maptile( sx, sy, i );
		if ( t == NULL || *t == NULL || ( *t )->action == NULL ) continue;
		( *t )->action( t, etile, ACT_LEAVE );
	}
}
