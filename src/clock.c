#include "clock.h"
#include "logging.h"

#include <time.h>

static struct tm date;

size_t status_time( char* buff, size_t len, uint16_t tick )
{
	if ( ! ( tick % CEILING( UPDATE_FREQUENCY, CLOCK_FREQUENCY ) ) )
	{
		time_t timestamp;

		time( &timestamp );
		localtime_r( &timestamp, &date );
	}

	return strftime( buff, len, "%H:%M:%S", &date );
}
