/* Arduino implementation */
#include <Arduino.h>
#include "js.h"

/* analog */
#define XPIN	0
#define YPIN	1

/* digital */
#define FIREPIN	2

#define ADC_CENTER	512
#define ADC_THRESHOLD	100


void js_init(void)
{
	pinMode(FIREPIN, INPUT_PULLUP);
}

/* is fire button pressed */
int js_fire(void)
{
	return !digitalRead(FIREPIN);
}

/* +/- 511 */
int js_x(void)
{
	return -(analogRead(XPIN) - ADC_CENTER);
}

/* +/- 511 */
int js_y(void)
{
	return -(analogRead(YPIN) - ADC_CENTER);
}

/* get current state bitmask (JS_*) */
int js_state(void)
{
	int x, y, state = 0;

	if (js_fire())
		state |= JS_FIRE;

	x = js_x();
	if (x < -ADC_THRESHOLD)
		state |= JS_LEFT;

	if (x > ADC_THRESHOLD)
		state |= JS_RIGHT;

	y = js_y();
	if (y < -ADC_THRESHOLD)
		state |= JS_DOWN;

	if (y > ADC_THRESHOLD)
		state |= JS_UP;

	return state;
}
