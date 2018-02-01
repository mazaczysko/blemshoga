#include "music.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

ALLEGRO_SAMPLE *music;
ALLEGRO_SAMPLE_ID sid;
static int playing = 0;

//TODO redo all this
void music_toggle( )
{
	if ( playing ) music_stop( );
	else music_play( );
}

void music_play( )
{
	if ( !playing )
	{
		playing = 1;
		al_play_sample( music, 1.0, 1.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &sid );
	}
}

void music_stop( )
{
	if ( playing )
	{
		al_stop_sample( &sid );
		playing = 0;
	}
}

void music_init( )
{
	music = al_load_sample( "resources/sounds/theme.wav" );
}
