#ifndef _TFT_H_
#define _TFT_H_

#define WIDTH 320
#define HEIGHT 240

void tft_init(void);
void tft_fill(int x, int y, int w, int h, unsigned color);
void tft_scroll(unsigned s); // 0..WIDTH-1
void tft_cfg_scroll(unsigned top, unsigned bottom); // lines

void tft_update(void);

void tft_setpal(int idx, unsigned color);
void tft_blit8(int x, int y, int w, int h, unsigned char *d);

#endif
