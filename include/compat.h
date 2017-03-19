#ifndef _COMPAT_H_
#define _COMPAT_H_

#ifdef ARDUINO
#include <avr/pgmspace.h>
#else
#include <unistd.h>
#define delay(x) usleep((x) * 1000)
#define PROGMEM
#define pgm_read_byte(a) (*a)
#endif

#endif
