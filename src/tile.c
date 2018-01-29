#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tileaction.h"
#include "tile.h"

static struct tile *tiles = NULL;
static int tilecnt = 0;

struct tile *tile( const char *name )
{
	for ( int i = 0; i < tilecnt; i++ )
	{
		if ( !strcmp( tiles[i].name, name ) ) return tiles + i;
	}

	return NULL;
}

static int loadtile( const char *path )
{
	char buf[4096];
	char *key, *val;
	int i, len;
	struct tile newtile, *tilearr;
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

		//Keys that don't need values
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

	//Proper ID
	newtile.id = tilecnt;

	//Load things based on read data
	if ( newtile.spritename == NULL ) return 1;
	if ( newtile.name == NULL ) return 1;
	newtile.sprite = al_load_bitmap( newtile.spritename );
	newtile.action = acthandler( newtile.actionname );
	if ( newtile.sprite == NULL ) return 1;

	//Reallocate tile array
	tilearr = realloc( tiles, ( tilecnt + 1 ) * sizeof( struct tile ) );
	if ( tilearr == NULL ) return 1;

	//Save new tile
	tiles = tilearr;
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

		assert( snprintf( filename, PATH_MAX, "%s/%s", path, ent->d_name ) != 0 );
		if ( loadtile( filename ) )
		{
			fprintf( stderr, "tile loading failed - '%s' skipped!\n", ent->d_name );
		}
	}

	closedir( d );
	return 0;
}

void tiles_destroy( )
{
	unsigned int i;

	for( i = 0; i < tilecnt; i++ )
	{
		al_destroy_bitmap( tiles[i].sprite );
		free( tiles[i].name );
		free( tiles[i].spritename );
	}

	free( tiles );
}
