#include "map.h"
#include "tile.h"
#include "ent.h"
#include "tileaction.h"
#include "ai.h"
#include "loadtile.h"
#include <assert.h>
#include <stdio.h>
#include <dirent.h>

//Entity templates
static struct tile *entt = NULL;
static int enttcnt = 0;

//Entity tiles
static struct tile *ent = NULL;
static int entcnt = 0;

//Entity handles
static struct tile ***enth = NULL;

//Trigger AI handlers for all entites
//Hentai haahah
void entai( )
{
	int i;

	for ( i = 0; i < entcnt; i++ )
	{
		if ( ent[i].ent.ai != NULL ) ent[i].ent.ai( enth + i );
	}
}

//Spawns entity of given name at specific coordinates
void spawn( const char *name, int x, int y )
{
	struct tile **dest = mapfreetile( x, y );
	struct tile *e = NULL;
	void *ptr = NULL;

	int i;

	if ( dest == NULL )
		return;

	assert( entt != NULL );

	for( i = 0; i < enttcnt; i++ )
	{
			assert( name != NULL );
			assert( entt[i].name != NULL );
			if( !strcmp( entt[i].name, name ) )
			{
				e = entt + i;
				break;
			}
	}

	//Reallocate arrays
	struct tile *oldent = ent;
 	ptr = realloc( ent, ( entcnt + 1 ) * sizeof( struct tile ) );
	if ( ptr == NULL ) return;
	ent = ptr;

	//Update old map pointers (realloc doesn't guarantee to return identical pointer)
	for ( i = 0; i < entcnt; i++ )
	{
		*enth[i] = ent + ( *enth[i] - oldent );
	}

	struct tile ***oldenth = enth;
	ptr = realloc( enth, ( entcnt + 1 ) * sizeof( struct tile** ) );
	if ( ptr == NULL ) return;
	enth = ptr;

	//Update old handles - handle array may have moved
	for ( i = 0; i < entcnt; i++ )
	{
		ent[i].ent.handle = enth + ( ent[i].ent.handle - oldenth );
	}

	//Create copy of the template
	e = memcpy( ent + entcnt, e, sizeof( struct tile ) );

	//TODO e->ent.z
	e->ent.x = x;
	e->ent.y = y;

	//Put entity on map and properly link its handle
	*dest = ent + entcnt;
	e->ent.handle = enth + entcnt;
	enth[entcnt] = dest;
	entcnt++;
}

//Loads entity files from directory
//DO NOT CALL DURING THE GAME (realloc)
int ent_init( const char *path )
{
	char filename[PATH_MAX];
	const char *ext;
	struct tile t, *entarr;

	DIR *d = opendir( path );
	struct dirent *dent;

	assert( d != NULL );

	while ( ( dent = readdir( d ) ) != NULL )
	{
		//Accept only *.ent files
		ext = strrchr( dent->d_name, '.' );
		if ( ext == NULL ) continue;
		if ( strcmp( ext + 1, "ent" ) ) continue;

		assert( snprintf( filename, PATH_MAX, "%s/%s", path, dent->d_name ) != 0 );
		if ( !loadtile( filename, &t ) )
		{
			entarr = realloc( entt, ( enttcnt + 1 ) * sizeof( struct tile ) );
			assert( entarr != NULL );
			entt = entarr;
			entt[enttcnt++] = t;
		}
		else
		{
			fprintf( stderr, "entity loading failed - '%s' skipped!\n", dent->d_name );
		}
	}

	closedir( d );
	return 0;
}

//Moves entity
//Map contains pointer to the actual entity data
//Since we must know where the entity is, we keep a pointer to its tile on the map
//Because this function can alter entity location, it has to be able to modify pointer decribed above
void entmove( struct tile ***eptr, int dx, int dy )
{
	struct tile **t, **src, **dest, *e, **etile;
	int i, sx, sy;

	//Pointer summary:
	//src - pointer to the place where entity used to be
	//dest - pointer to where entity wants to go (or went)
	//e - pointer to the actual entity data
	//etile - pointer to the current entity tile
	//eptr - pointer to some function's pointer to the entity on map - should be updated when etile is updated

	//For debug, let's have some assertions instead
	//Programmer should know that things aren't really fine if he attempts to move stone wall
	assert( eptr != NULL );
	assert( *eptr != NULL );
	assert( **eptr != NULL );
	assert( ( **eptr )->entity != 0 );

	//This is so stupid, yet it has to be here :(
	if ( eptr == NULL || *eptr == NULL || **eptr == NULL ) return;
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
	assert( etile != NULL );
	assert( *etile != NULL );
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
	src = etile;    //Source is current entity position
	*dest = *etile; //Write proper entity pointer to destination (struct tile*)
	*src = NULL;    //Leave source tile empty
	
	//Update all entity pointers after move
	*eptr = dest;
	etile = *eptr;
	e = *etile;
	
	//Leave interaction
	//Interact with all tiles ahead of entity
	assert( etile != NULL );
	assert( *etile != NULL );
	for ( i = 0; i < map.depth; i++ )
	{
		t = maptile( sx, sy, i );
		if ( t == NULL || *t == NULL || ( *t )->action == NULL ) continue;
		( *t )->action( t, etile, ACT_LEAVE );
	}
}
