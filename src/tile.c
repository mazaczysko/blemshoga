#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "map.h"
#include "tileaction.h"
#include "loadtile.h"
#include "tile.h"

static struct tile *tiles = NULL;
static int tilecnt = 0;

struct tilesnd *tilesnd( struct tile *t, const char *name )
{
	if ( t == NULL || name == NULL ) return NULL;

	for ( int i = 0; i < t->sndcnt; i++ )
	{
		if ( t->snd[i].name == NULL || t->snd[i].sound == NULL ) continue;
		if ( !strcmp( t->snd[i].name, name ) )
		{
			al_play_sample( t->snd[i].sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL );
			return t->snd + i;
		}
	}

	return NULL;
}

struct tile *tile( const char *name )
{
	for ( int i = 0; i < tilecnt; i++ )
	{
		if ( !strcmp( tiles[i].name, name ) ) return tiles + i;
	}

	return NULL;
}

int tiles_init( const char *path )
{
	char filename[PATH_MAX];
	struct tile t, *tilearr;
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

		if ( !loadtile( filename, &t ) )
		{
			tilearr = realloc( tiles, ( tilecnt + 1 ) * sizeof( struct tile ) );
			assert( tilearr != NULL );
			tiles = tilearr;
			tiles[tilecnt++] = t;
		}
		else
		{
			fprintf( stderr, "error loading %s\n", filename );
		}
	}

	closedir( d );
	return 0;
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
			entarr = realloc( tiles, ( tilecnt + 1 ) * sizeof( struct tile ) );
			assert( entarr != NULL );
			tiles = entarr;
			tiles[tilecnt++] = t;
		}
		else
		{
			fprintf( stderr, "entity loading failed - '%s' skipped!\n", dent->d_name );
		}
	}

	closedir( d );
	return 0;
}

void tiles_destroy( )
{
	unsigned int i, j;

	for( i = 0; i < tilecnt; i++ )
	{
		al_destroy_bitmap( tiles[i].sprite );
		free( tiles[i].name );
		free( tiles[i].spritename );
		for( j = 0; j < tiles[i].sndcnt; j++ )
		{
			al_destroy_sample( tiles[i].snd[j].sound );
			free( tiles[i].snd[j].name );
		}
	}

	free( tiles );
}
