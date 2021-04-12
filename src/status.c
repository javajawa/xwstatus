#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <X11/Xlib.h>

#include "logging.h"
#include "config.h"
#include "clock.h"

#ifdef NO_X11
void main_loop( void );
#define LOG_OUTPUT VERB
#else
void main_loop( Display* display );
#endif

int main( void )
{
#ifndef NO_X11 
	int ret;
	Display * const display = XOpenDisplay( NULL );

	if ( display == NULL )
	{
		logs( LOG_GENERAL, CRIT, "cannot open display" );
		return 1;
	}

	logs( LOG_GENERAL, VERB, "opened X11 display" );
#endif

#ifdef NO_X11
	main_loop();
#else
	main_loop( display );
#endif

#ifndef NO_X11
	ret = XCloseDisplay( display );

	if ( ret == 0 )
	{
		logs( LOG_GENERAL, VERB, "closed X11 display" );
	}
	else
	{
		log( LOG_GENERAL, CRIT, "error closing X11 display" );
	}
#endif

	return 0;
}

#ifdef NO_X11
void main_loop( void )
#else
void main_loop( Display* display )
#endif
{
	uint16_t current_tick = 0;
	struct timespec tick = { 0, 1000000000 / UPDATE_FREQUENCY };
	char buffer[ BUFFER_LEN ];
	size_t len, return_value;
	char* ptr;

	// TODO: Signal Handler to break the loop
	while ( 1 )
	{
		logfs( LOG_GENERAL, VERB, "starting tick %u", current_tick );

		// Reset the buffer.
		len = BUFFER_LEN - 1;
		ptr = buffer;

		// Load in the time
		if ( CLOCK_FREQUENCY != -1 )
		{
			return_value = status_time( ptr, len, current_tick );
			len -= return_value;
			ptr += return_value;
		}
		// Add the null terminator
		*ptr = 0;

#ifdef NO_X11
		logfs( LOG_OUTPUT, INFO, "value: %s", buffer );
#else
		// Set the buffer into the name
		// TODO: error codes in this.
		XStoreName( display, DefaultRootWindow( display ), buffer );
		XSync( display, 0 );
#endif

		// Done for this tick
		logfs( LOG_GENERAL, VERB, "completed tick %u", current_tick );

		++current_tick;

		if ( nanosleep( &tick, NULL ) == -1 )
		{
			if ( errno == EINVAL )
			{
				logs( LOG_GENERAL, WARN, "loop interrupted, exiting" );
				break;
			}
			log( LOG_GENERAL, WARN, "" );
		}
	}
}
