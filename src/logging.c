#include <time.h>

#include "logging.h"

// Statically allocated buffer for the timestamps in logs
// This is not thread safe, but good enough for most purposes.
static char timestr[9];

// List of escape characters for ASCII-7 control characters,
// or the ASCII null character if there is not a standard escape.
static char const * const escapes =
	"0"     // Null
	"\0"    // Start of heading
	"\0"    // Start of text
	"\0"    // End of text
	"\0"    // End of transmission
	"\0"    // Enquiry
	"\0"    // Ack
	"a"     // Bell
	"b"     // Backspace
	"t"     // Tab
	"n"     // New line
	"v"     // Vertical table
	"\0"	// ???
	"r"     // Carriage return
	"\0"    // Shift out
	"\0"    // Shift in
	"\0"    // Data link escape
	"\0"    // Device control 1
	"\0"    // Device control 2
	"\0"    // Device control 3
	"\0"    // Device control 4
	"\0"    // NACK
	"\0"    // SYN
	"\0"    // End of transmission block
	"\0"    // Cancel
	"\0"    // End of medium
	"\0"    // Subsititue
	"["     // Escape
	"\0"    // File sep
	"\0"    // group sep
	"\0"    // record sep
	"\0"    // unit sep
;

char const * get_timestamp()
{
	time_t timestamp;
	struct tm date;

	time( &timestamp );
	localtime_r( &timestamp, &date );

	strftime( timestr, 9, "%H:%M:%S", &date );

	return timestr;
}

void strdump( char const * restrict str, ssize_t len )
{
	// If a length of -1 is supplied, use strlen() to determine length
	if ( len == -1 )
	{
		len = strlen( str );
	}

	while ( len )
	{
		// Control characters are printed out as escapes, either hex
		// or standard sequences where needed.
		// Other characters just get printed.
		if ( (unsigned char)*str < 32 )
		{
			if ( escapes[(unsigned char)*str] )
			{
				fprintf( stderr, "\033[35m\\%c\033[0m", escapes[(unsigned char)*str] );
			}
			else
			{
				fprintf( stderr, "\033[35m\\%02x\033[0m", *str );
			}

			// Line endings also get rendered with an actual newline
			if ( *str == '\n' )
			{
				fprintf( stderr, "\n" );
			}
		}
		else
		{
			fprintf( stderr, "%c", *str );
		}

		++str;
		--len;
	}

	fprintf( stderr, "\n" );
}
