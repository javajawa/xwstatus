#ifndef _CONFIG_H
#define _CONFIG_H

#define NO_X11

#define UPDATE_FREQUENCY   8 // Eight updates per second
#define CLOCK_FREQUENCY    2 // Update clock twice a second
#define HOSTNAME_FREQUENCY 0 // Update hostname on start-up only

// Set up the logging level to Warning
#define LOG_GENERAL  VERB

// Max acceptable length of the title, plus null terminator
#define BUFFER_LEN ( 2 << 6 ) + 1

#define CEILING(x,y) (((x) + (y) - 1) / (y))

#endif
