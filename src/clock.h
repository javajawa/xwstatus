#ifndef _CLOCK_H
#define _CLOCK_H

#include "config.h"

#ifdef CLOCK_FREQUENCY
#if CLOCK_FREQUENCY > -1

_Static_assert( CLOCK_FREQUENCY <= UPDATE_FREQUENCY, "Can't have clock frequency higher than the global frequency" );

#include <stddef.h>
#include <stdint.h>

size_t status_time( char* buff, size_t len, uint16_t tick );

#endif // CLOCK_FREQUENCY > -1
#endif // CLOCK_FREQUENCY
#endif // _CLOCK_H
