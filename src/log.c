#include <stdio.h>
#include <stdarg.h>
#include "utils.h"
#include "log.h"

static const char *log_progname = NULL;
FILE *log_file = NULL;

void log_init( FILE *file, const char *progname )
{
    log_file = file;
    log_progname = progname;
}

void log_message( FILE *file, const char *prefix, const char *suffix, const char *format, ... )
{
    va_list ap;
    va_start( ap, format );
    
    fprintf( file, "%s", prefix );
    vfprintf( file, format, ap );
    fprintf( file, "%s", suffix );
    
    va_end( ap );
}

void log_destroy( void )
{
    log_file = NULL;
    log_progname = NULL;
}

