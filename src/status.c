#include <stdio.h>
#include <X11/Xlib.h>

#include "logging.h"

// Set up the logging level to Warning
#define LOG_GENERAL WARN

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
