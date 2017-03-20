#include "compat.h"
#include "tft.h"
#include "font.h"
#include "fontdata.h"

void font_putc(char c, int x, int y, int sx, int sy, unsigned fg,
	       unsigned bg)
{
	int w, h;

	if (c < 32 || c > 127)
		c = '?';

	c -= 32;

	for (w = 0; w < FONT_W; w++) {
		unsigned char t = pgm_read_byte(&fontdata[(unsigned char)c][w]);
		int yy = y;
		for (h = 0; h < FONT_H; h++) {
			if (t & 1)
				tft_fill(x, yy, sx, sy, fg);
			else if (bg != TRANSP)
				tft_fill(x, yy, sx, sy, bg);
			yy += sy;
			t >>= 1;
		}
		x += sx;
	}
}

void font_puts(char *s, int x, int y, int sx, int sy, unsigned fg,
	       unsigned bg)
{
	while (*s) {
		font_putc(*s, x, y, sx, sy, fg, bg);
		x += FONT_W * sx;
		s++;
	}
}
