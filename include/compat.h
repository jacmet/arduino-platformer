#ifndef _COMPAT_H_
#define _COMPAT_H_

#ifdef ARDUINO
#include <avr/pgmspace.h>
#else
#define PROGMEM
#define pgm_read_byte(a) (*a)
#endif

#endif
