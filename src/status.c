#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <X11/Xlib.h>

#include "logging.h"

// Set up the logging level to Warning
#define LOG_GENERAL  VERB

// Max acceptable length of the title, plus null terminator
#define BUFFER_LEN ( 2 << 6 ) + 1
#define TICKS_PER_SECOND 8

void main_loop( Display* display );

int main( void )
{
	int ret;
	Display * const display = XOpenDisplay( NULL );

	if ( display == NULL )
	{
		log( LOG_GENERAL, CRIT, "cannot open display" );
		return 1;
	}

	logs( LOG_GENERAL, VERB, "opened X11 display" );

	main_loop( display );

	ret = XCloseDisplay( display );

	if ( ret == 0 )
	{
		logs( LOG_GENERAL, VERB, "closed X11 display" );
	}
	else
	{
		log( LOG_GENERAL, CRIT, "error closing X11 display" );
	}

	return 0;
}

size_t status_time( char* buff, int len )
{
	time_t timestamp;
	struct tm date;

	time( &timestamp );
	localtime_r( &timestamp, &date );

	return strftime( buff, len, "%H:%M:%S", &date );
}

void main_loop( Display* display )
{
	uint16_t current_tick = 0;
	struct timespec tick = { 0, 1000000000 / TICKS_PER_SECOND };
	char buffer[ BUFFER_LEN ];
	size_t len, return_value;
	char* ptr;

	// TODO: Signal Handler to break the loop
	while ( 1 )
	{
		++current_tick;
		logfs( LOG_GENERAL, VERB, "starting tick %u", current_tick );

		// Reset the buffer.
		len = BUFFER_LEN - 1;
		ptr = buffer;

		// Load in the time
		return_value = status_time( ptr, len );
		len -= return_value;
		ptr += return_value;


		// Set the status
		*ptr = 0;
		// TODO: error codes in this.
		XStoreName( display, DefaultRootWindow( display ), buffer );
		XSync( display, 0 );

		// Done for this tick
		logfs( LOG_GENERAL, VERB, "completed tick %u", current_tick );

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
