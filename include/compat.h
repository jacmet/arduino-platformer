#ifndef _COMPAT_H_
#define _COMPAT_H_

#ifdef ARDUINO
#include <Arduino.h>
#include <avr/pgmspace.h>
#else
#include <unistd.h>
#define delay(x) usleep((x) * 1000)

#include <time.h>
static unsigned long millis(void)
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec * 1000) + (ts.tv_nsec/1000000);
}
#define PROGMEM
#define pgm_read_byte(a) (*a)
#endif

#endif
