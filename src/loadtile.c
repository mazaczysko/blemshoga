#include "tile.h"
#include "tileaction.h"
#include "ai.h"
#include <stdio.h>
#include <dirent.h>

static int loadsnd( FILE *f, struct tile *t, int cnt )
{
	char buf[1024];
	char *name, *path;
	int len, j, i = 0;

	t->snd = calloc( cnt, sizeof( struct tilesnd ) );
	t->sndcnt = cnt;

	while ( fgets( buf, 1024, f ) != NULL && i < cnt )
	{
		//Skip comments
		if ( buf[0] == ';' ) continue;

		//Trim buffer
		len = strlen( buf );
		for ( j = 0; j < len; j++ )
		{
			if ( strchr( "\n\r", buf[j] ) != NULL )
			{
				buf[j] = 0;
				break;
			}
		}

		//Divide into key and value pairs
		name = buf;
		path = strchr( buf, ' ' );
		if ( path != NULL ) *path++ = 0;

		t->snd[i].sound = al_load_sample( path );
		t->snd[i].name = strdup( name );
		if ( t->snd[i].sound == NULL || t->snd[i].name == NULL ) fprintf( stderr, "cannot load '%s'\n", path );		

		i++;
	}
	return 0;
}

//Loads tile from file
int loadtile( const char *path, struct tile *t )
{
	char buf[4096];
	char *key, *val;
	int i, len;
	static int id = 0;

	assert( t != NULL );
	memset( t, 0, sizeof( struct tile ) );

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

		//Load sounds
		if ( !strcmp( key, "--snd" ) ) loadsnd( f, t, atoi( val ) );

		//Keys that don't need values
		if ( !strcmp( key, "ground" ) ) t->ground = 1;
		if ( !strcmp( key, "solid" ) ) t->solid = 1;
		if ( !strcmp( key, "flammable" ) ) t->flammable = 1;

		//Entity
		if ( !strcmp( key, "entity" ) ) t->entity = 1;

		//Keys that need values
		if ( val == NULL ) continue;
		if ( !strcmp( key, "name" ) ) t->name = strdup( val );
		if ( !strcmp( key, "sprite" ) ) t->spritename = strdup( val );
		if ( !strcmp( key, "animframe" ) ) sscanf( val, "%d", &t->animframe );
		if ( !strcmp( key, "action" ) ) t->actionname = strdup( val );

		//Entity
		if ( !strcmp( key, "ai" ) ) t->ent.ainame = strdup( val );
		if ( !strcmp( key, "hp" ) ) sscanf( val, "%d", &t->ent.maxhp );

	}

	//Close file
	fclose( f );

	//Proper ID
	t->id = id;

	//Load things based on read data
	if ( t->spritename == NULL ) return 1;
	if ( t->name == NULL ) return 1;
	t->sprite = al_load_bitmap( t->spritename );
	t->action = acthandler( t->actionname );
	if ( t->sprite == NULL ) return 1;
	t->ent.ai = aihandler( t->ent.ainame );


	return 0;
}
