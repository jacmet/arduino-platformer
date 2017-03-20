#ifndef _FONT_H_
#define _FONT_H_

#ifdef __cplusplus
extern "C" {
#endif

#define FONT_W	6
#define FONT_H	8

#define TRANSP	0xf81f /* purple */

void font_putc(char c, int x, int y, int sx, int sy, unsigned fg,
			   unsigned bg);

void font_puts(char *s, int x, int y, int sx, int sy, unsigned fg,
			   unsigned bg);

#ifdef __cplusplus
};
#endif

#endif
