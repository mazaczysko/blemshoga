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
		if ( ent[i].active && ent[i].ent.ai != NULL )
			ent[i].ent.ai( enth + i );
	}
}

//Spawns entity of given name at specific coordinates
void spawn( const char *name, int x, int y )
{
	struct tile **dest = maptile( x, y, MAP_LENT );
	struct tile *e = NULL;
	void *ptr = NULL;

	int i, slot;

	if ( dest == NULL )
		return;

	assert( entt != NULL );

	//Find right template
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

	//Look for free slot
	for ( i = 0; i < entcnt; i++ )
	{
		if ( !ent[i].active )
		{
			slot = i;
			break;
		}
	}
	
	//If no free slots, allocate one
	if ( i == entcnt )
	{
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
		
		slot = entcnt;
		entcnt++;
	}

	//Create copy of the template
	e = memcpy( ent + slot, e, sizeof( struct tile ) );
	
	e->ent.x = x;
	e->ent.y = y;
	e->ent.hp = e->ent.maxhp;

	//Put entity on map and properly link its handle
	*dest = ent + slot;
	e->ent.handle = enth + slot;
	enth[slot] = dest;
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

//UNTESTED
//Checks if entity A is hostile to entity B
//Returns 0 or 1 on success and -1 on failure
//Or fails an asssertion if NDEBUG is not defined
int entckhostile( struct tile *a, struct tile *b )
{
	//For debug yay
	assert( a != NULL );
	assert( b != NULL );
	assert( a->entity );
	assert( b->entity );
	
	if ( a == NULL || b == NULL || !a->entity || !b->entity || !a->active || !b->active ) return -1;
	
	return ( a->ent.hosgrp & b->ent.grp ) != 0;
}

//UNTESTED
//Kills an entity
void entkill( struct tile ***eptr )
{
	assert( eptr != NULL );
	struct tile **etile = *eptr;
	assert( etile != NULL );
	struct tile *e = *etile;
	assert( e != NULL );
	
	//TODO
	//fprintf( stderr, "An entity dies..." );
	if ( e->ent.deathh != NULL ) e->ent.deathh( e->ent.handle );
	
	//This is gonna be so f*cked up...
	
	//Mark entity slot as free and remove handle just in case
	e->active = 0;
	e->ent.handle = NULL;
	
	//Remove from map	
	*etile = NULL;
}

//UNTESTED
//Enforces entity A to attack entity B
//Uses information from 
//Returns -1 on error or damage dealt on success
int entattack( struct tile *a, struct tile *b )
{
	//For debug yay
	assert( a != NULL );
	assert( b != NULL );
	assert( a->entity );
	assert( b->entity );
	
	if ( a == NULL || b == NULL || !a->entity || !b->entity || !a->active || !b->active ) return -1;
	
	//Is this a critical hit?
	int crit = ( ( random( ) % 10000 ) / 10000.0 ) < a->ent.combat.critical;
	
	//Check dodge
	if ( ( rand( ) % 10000 / 10000.0 ) < ( a->ent.combat.precision - b->ent.combat.evasion ) )
		return 0;

	
	//Calculate attack power and bias (+-10%) - TODO it should depend on more things
	double attack = a->ent.combat.strength * a->ent.combat.attack;
	double abias = ( ( ( rand( ) % 10000 ) / 10000.0 ) * 0.2 - 0.1 ) * attack;
	if ( crit ) attack *= 1.5;
	attack += abias;
	
	//Calculate defense power and bias (+-10%) - TODO it should depend on more things
	double dbias = ( ( ( rand( ) % 10000 ) / 10000.0 ) * 0.2 - 0.1 ) * b->ent.combat.armor;
	attack -= b->ent.combat.armor - dbias;
	//TEMP
	fprintf( stderr, "%f damage dealt!\n", attack );
	attack = (int)attack;
	if ( attack < 0 ) attack = 0;
	
	//Actions
	if ( b->ent.combat.defh != NULL ) b->ent.combat.defh( b->ent.handle );
	if ( a->ent.combat.atkh != NULL ) a->ent.combat.atkh( b->ent.handle );
	
	//Die if killed (how clever)
	b->ent.hp -= attack;
	if ( b->ent.hp <= 0 ) entkill( b->ent.handle );
	return attack;
}

//Moves entity
//Map contains pointer to the actual entity data
//Since we must know where the entity is, we keep a pointer to its tile on the map
//Because this function can alter entity location, it has to be able to modify pointer decribed above
void entmove( struct tile ***eptr, int dx, int dy )
{
	struct tile **t, **src, **dest, *e, **etile;
	int sx, sy, i;

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
	if ( eptr == NULL || *eptr == NULL || **eptr == NULL || !( **eptr )->active ) return;
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

	//Interact with all tiles ahead of entity - should indeed affect all the layers. Maybe we'll get some boxes sometime?
	assert( etile != NULL );
	assert( *etile != NULL );
	for ( i = 0; i < map.depth; i++ )
	{
		t = maptile( dx, dy, i );
		if ( t == NULL || *t == NULL || ( *t )->action == NULL ) continue;
		( *t )->action( t, etile, ACT_PUSH );
	}
	
	//Attack hostile entities
	t = maptile( dx, dy, MAP_LENT );
	if ( t != NULL && *t != NULL )
	{
		//TODO waht if user wants to attack a neutral entity?
		if ( entckhostile( e, *t ) )
			entattack( e, *t );
	}

	//Attempt move
	if ( mapissolid( dx, dy ) ) return;
	if ( ( dest = maptile( dx, dy, MAP_LENT ) ) == NULL ) return;
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
	//Interact with solid tiles ahead of entity
	assert( etile != NULL );
	assert( *etile != NULL );

	t = maptile( sx, sy, MAP_LSOLID );
	if ( t != NULL && *t != NULL && ( *t )->action != NULL )
		( *t )->action( t, etile, ACT_LEAVE );
}

void ent_destroy( )
{
	unsigned int i;

	for( i = 0; i < enttcnt; i++ )
	{
		al_destroy_bitmap( entt[i].sprite );
		free( entt + i );
	}


}
