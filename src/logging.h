#ifndef _LOGGING_H
#define _LOGGING_H

/**
 * Log levels
 *
 * You get up a logger along the lines of
 *
 *  #define LOG_NETWORK  WARN
 *
 * which creates a 'logger' which will log at level 'WARN' or higher.
 * A number of macros are provided to log information.
 *
 * For example:
 *
 *  logf( LOG_NETWORK, ALRT, "Failed to open %", name )
 *
 * Will log something like:
 *
 *  15:20:20 [program:foo.c:57] Failed to open somefile: file not found
 *
 * using the value of errno to populate the error message.
 *
 * The macros act as follows:
 *
 *  logs()  - Log a fixed message
 *  logfs() - Log a printf style message
 *  log()   - Log a fixed message and the value of errno
 *  logf()  - Log a printf style message
 *
 *  logf( [LOG], [LEVEL], [FORMAT], [...PRINTF PARAMS] )
 *
 * Similar macros are provided for fatal errors, err() and errf().
 * These log the error, call the clean_exit() function to allow the
 * program to clear up state, and then invokes exit() with the code given.
 *
 *  err( EXIT CODE, MESSAGE )
 *  err( EXIT CODE, FORMAT, [... PRINTF PARAMS] )
 */

// Require that a program name is defined.
// This is done by default in the Makefile
#ifndef PROGRAM_NAME
	#error "No program name defined"
#endif

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>

/**
 * Gets the current time of day in a pre-allocated string.
 */
char const * get_timestamp();
/**
 * Outputs a character buffer to stderr.
 * ASCII-7 control characters are replaced with their escapes (such as "\t"),
 * or the \x[hex] if there is no standard escape.
 */
void strdump( char const * restrict, ssize_t );

/**
 * Your program must define this function to tidy up files,
 * sockets and so forth if you use either of the "err{f,s}" macros.
 */
void clean_exit();

#define STRINGIFY(x) #x
#define STR(x) STRINGIFY(x)

// The 5 log levels we support.
#define NONE 0
#define CRIT 1
#define ALRT 2
#define WARN 3
#define INFO 4
#define VERB 5
#define LOG_MAX 6

// When compiling with the DEBUG flag, we append the file and line number
// to the program name for the log prefixes
#ifdef DEBUG
	#define _LOG_MARKER  PROGRAM_NAME ":" __FILE__ ":" STR(__LINE__)
#else
	#define _LOG_MARKER  PROGRAM_NAME
#endif


#ifdef THREADING
	char const * get_thread_name();

	#define logfs( log, level, format, ... ) \
		_Static_assert( level > NONE && level < LOG_MAX, "Log Level not valid at " __FILE__ ":" STR(__LINE__) ); \
		if ( level <= log ) { \
			fprintf( \
				stderr, \
				"%s [" _LOG_MARKER "] %s " format "\n", \
				get_timestamp(), \
				get_current_thread_name(), \
				__VA_ARGS__ \
			);\
		}

#else

	#define logfs( log, level, format, ... ) \
		_Static_assert( level > NONE && level < LOG_MAX, "Log Level not valid" ); \
		if ( level <= log ) { \
			fprintf( \
				stderr, \
				"%s [" _LOG_MARKER "] " format "\n", \
				get_timestamp(), \
				__VA_ARGS__ \
			);\
		}

#endif


#define log(  log, level, mess )        logfs( log, level, "%s: %s", mess, strerror( errno ) )
#define logs( log, level, mess )        logfs( log, level, "%s",     mess )
#define logf( log, level, format, ... ) logfs( log, level, format ": %s", __VA_ARGS__, strerror( errno ) )

#define errfs( code, format, ... ) \
	logfs( CRIT, CRIT, format ": %s", __VA_ARGS__, strerror( errno ) ) \
	clean_exit(); \
	exit( code )

#define err( code, mess ) errfs( code, "%s", mess )

#endif
