#ifndef _JS_H_
#define _JS_H_

#define JS_LEFT		0x01
#define JS_RIGHT	0x02
#define JS_UP		0x04
#define JS_DOWN		0x08
#define JS_FIRE		0x10
#define JS_QUIT		0x20

void js_init(void);

/* is fire button pressed */
int js_fire(void);

/* +/- 511 */
int js_x(void);

/* +/- 511 */
int js_y(void);

/* get current state bitmask (JS_*) */
int js_state(void);

#endif
