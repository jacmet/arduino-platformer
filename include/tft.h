#ifndef _TFT_H_
#define _TFT_H_

#define WIDTH 320
#define HEIGHT 240

void tft_init(void);
void tft_fill(int x, int y, int w, int h, unsigned color);
void tft_scroll(unsigned s); // 0..WIDTH-1
// number of lines at the top/bottom (cols left/right after rotation) to
// NOT scroll. top+bottom <= WIDTH
void tft_cfg_scroll(unsigned top, unsigned bottom); // lines

void tft_update(void);

void tft_setpal(int idx, unsigned color);
/* blit 8bit image of dimensions w x h to x,y */
void tft_blit8(int x, int y, int w, int h, unsigned char *d);
/* blit 8bit image of dimensions w x h to x,y and upscale to 2w x 2h */
void tft_blit8x2(int x, int y, int w, int h, unsigned char *d);

#endif
