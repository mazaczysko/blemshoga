#include "resmgr.h"

bool resmgr_has_image(const char *name)
{

}

ALLEGRO_BITMAP *resmgr_get_image(const char *name)
{

}

ALLEGRO_BITMAP *resmgr_load_image(const char *path)
{
    ALLEGRO_BITMAP *bmp = al_load_bitmap( path );
    
    if( !bmp )
    {
        // fprintf( stderr, "%s: error loading bitmap '%s'!", argv[0])
    }
}

void resmgr_unload_image(const char *name)
{

}