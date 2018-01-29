#include "map.h"
#include "tile.h"
#include "ent.h"
#include "tileaction.h"
#include "ai.h"
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

//Load entity from file
static int loadent( const char *path )
{
	char buf[4096];
	char *key, *val;
	int i, len;
	struct tile newtemp, *temparr;
	memset( &newtemp, 0, sizeof( newtemp ) );

	//Open tile file
	FILE *f = fopen( path, "rt" );
	if ( f == NULL ) return 1;

	while ( fgets( buf, 4096, f ) != NULL )
	{
		//Trim buffer
		len = strlen( buf );
		for ( i = 0; i < len; i++ )
		{
			if ( strchr( "\n\r", buf[i] ) != NULL )
			{
				buf[i] = 0;
				break;
			}
		}

		//Divide into key and value pairs
		key = buf;
		val = strchr( buf, ' ' );
		if ( val != NULL ) *val++ = 0;

		//Keys that don't need values
		if ( !strcmp( key, "solid" ) ) newtemp.solid = 1;
		if ( !strcmp( key, "flammable" ) ) newtemp.flammable = 1;

		//Keys that need values
		if ( val == NULL ) continue;
		if ( !strcmp( key, "name" ) ) newtemp.name = strdup( val );
		if ( !strcmp( key, "sprite" ) ) newtemp.spritename = strdup( val );
		if ( !strcmp( key, "ai" ) ) newtemp.ent.ainame = strdup( val );
		if ( !strcmp( key, "hp" ) ) sscanf( val, "%d", &newtemp.ent.maxhp );
	}

	fclose( f );

	//Proper ID
	newtemp.id = enttcnt;

	//Default settings
	newtemp.ent.hp = newtemp.ent.maxhp;
	newtemp.entity = 1;
	newtemp.animframe = 0;
	newtemp.type = TILE_ENTITY;

	//Load things based on read data
	if ( newtemp.spritename == NULL ) return 1;
	if ( newtemp.name == NULL ) return 1;
	assert( ( newtemp.sprite = al_load_bitmap( newtemp.spritename ) ) != NULL );
	if ( newtemp.sprite == NULL ) return 1;
	newtemp.ent.ai = aihandler( newtemp.ent.ainame );

	//Reallocate tile array
	temparr = realloc( entt, ( enttcnt + 1 ) * sizeof( struct tile ) );
	if ( temparr == NULL ) return 1;

	//Save new entity template
	entt = temparr;
	entt[enttcnt] = newtemp;
	enttcnt++;
	return 0;
}

//Loads entity files from directory
int ent_init( const char *path )
{
	char filename[PATH_MAX];
	const char *ext;

	DIR *d = opendir( path );
	struct dirent *ent;

	assert( d != NULL );

	while ( ( ent = readdir( d ) ) != NULL )
	{
		//Accept only *.ent files
		ext = strrchr( ent->d_name, '.' );
		if ( ext == NULL ) continue;
		if ( strcmp( ext + 1, "ent" ) ) continue;

		assert( snprintf( filename, PATH_MAX, "%s/%s", path, ent->d_name ) != 0 );
		if ( loadent( filename ) )
		{
			fprintf( stderr, "entity loading failed - '%s' skipped!\n", ent->d_name );
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
