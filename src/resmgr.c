#include <errno.h>
#include "allegro.h"
#include "resmgr.h"
#include "log.h"

static struct
{
    int count;
    char **paths;
    ALLEGRO_BITMAP **bitmaps;
} bitmap_storage;

//Set default values for bitmap_storage struct components
void resmgr_init_bitmap_storage( void )
{
    log_info( "Initializing bitmap_storage...");

    bitmap_storage.count = 0;
    bitmap_storage.paths = NULL;
    bitmap_storage.bitmaps = NULL;
}

static int resmgr_get_bitmap_offset(const char *path)
{
    for( int i = 0; i < bitmap_storage.count; i++ )
        if( !strcmp( bitmap_storage.paths[i], path ) )
            return i;
    
    return -1;
}

bool resmgr_has_bitmap( const char *path )
{
    return resmgr_get_bitmap_offset(path) != -1;
}

ALLEGRO_BITMAP *resmgr_load_bitmap( const char *path )
{
    log_info( "Loading '%s'...", path );

    char *c = getcwd(NULL, 0);
        log_debug("cwd: %s", c);
    free(c);

    ALLEGRO_BITMAP *bmp = al_load_bitmap( path );
    
    if( !bmp )
    {
        log_error( "error loading bitmap: '%s'!", path );
        return NULL;
    }
    
    // Realloc bitmaps array
    ALLEGRO_BITMAP **tmp_ptr = realloc( bitmap_storage.bitmaps, ( bitmap_storage.count + 1 ) * sizeof( ALLEGRO_BITMAP* ) );
    if( !tmp_ptr )
    {
        log_error( "resmgr_load_bitmap() could not reallocate 'bitmap_storage.bitmaps' array");
        return NULL;
    }

    //Realloc paths array
    char **tmp = realloc( bitmap_storage.paths, ( bitmap_storage.count + 1 ) * sizeof( const char* ) );
    if( !tmp )
    {
        if (tmp_ptr != bitmap_storage.bitmaps) free(tmp_ptr);
        log_error( "resmgr_load_bitmap() could not reallocate 'bitmap_storage.paths' array");
        return NULL;
    }
    
    /*
        To jest tak jakbyś budował dom i położył krzywo cegłę:
        - "O nie! Jest krzywo! Dzwonię po buldożer!"
    */

    bitmap_storage.bitmaps = tmp_ptr;
    bitmap_storage.bitmaps[bitmap_storage.count] = bmp;
    bitmap_storage.paths = tmp;
    bitmap_storage.paths[bitmap_storage.count] = strdup(path);
    
    if( !bitmap_storage.bitmaps[bitmap_storage.count] )
    {
    }
    return bitmap_storage.bitmaps[bitmap_storage.count++];
}


ALLEGRO_BITMAP *resmgr_get_bitmap( const char *path )
{
    int offset = resmgr_get_bitmap_offset(path);
    return (offset == -1) ? resmgr_load_bitmap(path) : bitmap_storage.bitmaps[offset];
}


void resmgr_unload_bitmap( const char *path )
{
    log_info( "Unloading '%s'...", path );

    int pos = resmgr_get_bitmap_offset(path);
    if( pos == -1 ) return;

    al_destroy_bitmap(bitmap_storage.bitmaps[pos]);
    free(bitmap_storage.paths[pos]);
    bitmap_storage.bitmaps[pos] = bitmap_storage.bitmaps[bitmap_storage.count - 1];
    bitmap_storage.paths[pos] = bitmap_storage.paths[bitmap_storage.count - 1];
    bitmap_storage.count--;
}

void resmgr_destroy_bitmap_storage()
{
    while (bitmap_storage.count)
        resmgr_unload_bitmap(bitmap_storage.paths[0]);
    free( bitmap_storage.bitmaps );
    free( bitmap_storage.paths );
}