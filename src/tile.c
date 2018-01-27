#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tileaction.h"
#include "tile.h"

//TODO load from json (possibly use Lua)
static struct tile *tiles = NULL;
static int tilecnt = 0;
	/*
	{ 0, TILE_BLOCK, "stone floor", "./resources/blocks/floor_stone.png", NULL, 0, NULL, 1, 0, 0 },
	{ 1, TILE_BLOCK, "stone wall", "./resources/blocks/wall_stone.png", NULL, 0, NULL, 0, 1, 0 },
	{ 2, TILE_BLOCK, "door (closed)", "./resources/blocks/door_closed.png", NULL, 0, NULL, 0, 1, 0 },
	{ 3, TILE_BLOCK, "door (open)", "./resources/blocks/door_opened.png", NULL, 0, NULL, 0, 0, 0 },
	{ 4, TILE_BLOCK, "dead", "./resources/blocks/dead.png", NULL, 0, NULL, 1, 0, 0 },
	*/


struct tile *tilename( const char *name )
{
	for ( int i = 0; i < tilecnt; i++ )
	{
		if ( !strcmp( tiles[i].name, name ) ) return tiles + i;
	}
	
	return NULL;
}

//TODO error handling
static int loadtile( const char *path )
{
	char buf[4096];
	char *key, *val;
	int i, len;
	struct tile newtile;
	memset( &newtile, 0, sizeof( newtile ) );
	
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
		
		//Keys that don't values
		if ( !strcmp( key, "ground" ) ) newtile.ground = 1;
		if ( !strcmp( key, "solid" ) ) newtile.solid = 1;
		if ( !strcmp( key, "entity" ) ) newtile.entity = 1;
		if ( !strcmp( key, "flammable" ) ) newtile.flammable = 1;
		
		//Keys that need values
		if ( val == NULL ) continue;
		if ( !strcmp( key, "type" ) ) sscanf( val, "%d", &newtile.type );
		if ( !strcmp( key, "name" ) ) newtile.name = strdup( val );
		if ( !strcmp( key, "sprite" ) ) newtile.spritename = strdup( val );
		if ( !strcmp( key, "animframe" ) ) sscanf( val, "%d", &newtile.animframe );
		if ( !strcmp( key, "action" ) ) newtile.actionname = strdup( val );

	}
	
	fclose( f );
	
	newtile.id = tilecnt;
	
	//TEMP
	assert( newtile.spritename != NULL );
	assert( newtile.name != NULL );
	newtile.sprite = al_load_bitmap( newtile.spritename );
	assert( newtile.sprite != NULL );
	
	if ( newtile.actionname != NULL ) newtile.action = acthandler( newtile.actionname );
	
		
	
	//Save new tile
	tiles = realloc( tiles, ( tilecnt + 1 ) * sizeof( struct tile ) );
	tiles[tilecnt] = newtile;
	tilecnt++;
	return 0;
}

int tiles_init( const char *path )
{
	char filename[PATH_MAX];
	const char *ext;

	DIR *d = opendir( path );
	struct dirent *ent;
	
	assert( d != NULL );
	
	while ( ( ent = readdir( d ) ) != NULL )
	{
		//Accept only *.tile files
		ext = strrchr( ent->d_name, '.' );
		if ( ext == NULL ) continue;
		if ( strcmp( ext + 1, "tile" ) ) continue;
		snprintf( filename, PATH_MAX, "%s/%s", path, ent->d_name );
		loadtile( filename );
	}

	closedir( d );
	return 0;
}

int tiles_destroy( )
{
	unsigned int i;

	//TODO error checking
	for( i = 0; i < tilecnt; i++ )
	{
		al_destroy_bitmap( tiles[i].sprite );
		free( tiles[i].name );
		free( tiles[i].spritename );
	}

	return 0;
}
