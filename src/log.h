#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include "utils.h"

extern void log_init( FILE *file, const char *progname );
extern void log_message( FILE *file, const char *prefix, const char *suffix, const char *format, ... );
extern void log_destroy(void);

extern FILE *log_file;

#define log_error( ... ) log_message( log_file, TEXT_RED __FILE__ ":" TO_STRING(__LINE__) " ERROR: " TEXT_RESET, "\n", __VA_ARGS__)
#define log_warning( ... ) log_message( log_file, TEXT_YELLOW __FILE__ ":" TO_STRING(__LINE__) " WARNING: " TEXT_RESET, "\n", __VA_ARGS__)
#define log_info( ... ) log_message( log_file, TEXT_BLUE __FILE__ ":" TO_STRING(__LINE__) " INFO: " TEXT_RESET, "\n", __VA_ARGS__)
#define log_debug( ... ) log_message( log_file, TEXT_CYAN __FILE__ ":" TO_STRING(__LINE__) " DEBUG: " TEXT_RESET, "\n", __VA_ARGS__)


#endif