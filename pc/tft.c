#include <stdlib.h>
/* SDL2 implementation */
#include "SDL.h"
#include "tft.h"

#define min(a, b) ( (a) < (b) ? (a) : (b) )

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;

static unsigned short screen[WIDTH * HEIGHT];

static unsigned scroll_pos;
static unsigned scroll_top;
static unsigned scroll_bottom;

static int update;

static unsigned short palette[256];

static void update_texture(void)
{
	SDL_UpdateTexture(texture, NULL, screen, WIDTH * sizeof(screen[0]));
}

static void render_texture(void)
{
	SDL_Rect src, dst;
	unsigned w = WIDTH - (scroll_bottom + scroll_top);

	SDL_RenderClear(renderer);

	/* bottom */
	if (scroll_bottom) {
		src.x = 0;
		src.y = 0;
		src.w = scroll_bottom;
		src.h = HEIGHT;

		SDL_RenderCopy(renderer, texture, &src, &src);
	}

	/* scrolling area */
	src.x = scroll_bottom + scroll_pos;
	src.y = 0;
	src.w = w - scroll_pos;
	src.h = HEIGHT;
	dst.x = scroll_bottom;
	dst.y = 0;
	dst.w = w - scroll_pos;
	dst.h = HEIGHT;

	SDL_RenderCopy(renderer, texture, &src, &dst);

	if (scroll_pos) {
		src.x = scroll_bottom;
		src.y = 0;
		src.w = scroll_pos;
		src.h = HEIGHT;
		dst.x = scroll_bottom + w - scroll_pos;
		dst.y = 0;
		dst.w = scroll_pos;
		dst.h = HEIGHT;
		SDL_RenderCopy(renderer, texture, &src, &dst);
	}

	/* top */
	if (scroll_top) {
		src.x = WIDTH - scroll_top;
		src.y = 0;
		src.w = scroll_top;
		src.h = HEIGHT;

		SDL_RenderCopy(renderer, texture, &src, &src);
	}

	SDL_RenderPresent(renderer);
}

void tft_init(void)
{
	Uint32 flags = 0;
	int i, x, y, p;

	/* Initialize SDL. */
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		exit(1);

	if (getenv("FULLSCREEN"))
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

	window = SDL_CreateWindow("tft",
				  SDL_WINDOWPOS_UNDEFINED,
				  SDL_WINDOWPOS_UNDEFINED,
				  WIDTH, HEIGHT, flags);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

	texture = SDL_CreateTexture(renderer,
				    SDL_PIXELFORMAT_RGB565,
				    SDL_TEXTUREACCESS_STREAMING,
				    WIDTH, HEIGHT);
	if (!window || !renderer || !texture)
		exit(2);

	scroll_pos = scroll_top = scroll_bottom = 0;

	memset(screen, 0xff, sizeof(screen));
	update_texture();
	render_texture();
}

void tft_fill(int x, int y, int w, int h, unsigned color)
{
	int i, j;

	w = min(w, WIDTH - x);
	h = min(h, HEIGHT - y);

	for (i = 0; i < h; i++)
		for (j = 0; j < w; j++)
			screen[x + j + (y + i) * WIDTH] = color;
}

void tft_scroll(unsigned s) // 0..WIDTH-1
{
	SDL_assert(s < WIDTH);

	scroll_pos = s;
	update = 1;
}

void tft_cfg_scroll(unsigned top, unsigned bottom) // lines
{
	SDL_assert((top + bottom) <= WIDTH);

	scroll_top = top;
	scroll_bottom = bottom;
	update = 1;
}

void tft_setpal(int idx, unsigned color)
{
	SDL_assert(idx >= 0);
	SDL_assert(idx < 256);

	palette[idx] = color;
}

void tft_blit8(int x, int y, int w, int h, unsigned char *d)
{
	int i, j;

	SDL_assert(x >= 0); SDL_assert(x+w <= WIDTH);
	SDL_assert(y >= 0); SDL_assert(y+h <= HEIGHT);
	SDL_assert(d);

	for (i = 0; i < h; i++)
		for (j = 0; j < w; j++)
			screen[(y + i) * WIDTH + x + j] = palette[*d++];

	update = 1;
}

void tft_blit8x2(int x, int y, int w, int h, unsigned char *d)
{
	int i, j;

	SDL_assert(x >= 0); SDL_assert(x+w*2 <= WIDTH);
	SDL_assert(y >= 0); SDL_assert(y+h*2 <= HEIGHT);
	SDL_assert(d);

	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			screen[(y + i*2) * WIDTH + x + j*2] = palette[*d];
			screen[(y + i*2) * WIDTH + x + j*2 + 1] = palette[*d];
			screen[(y + i*2 + 1) * WIDTH + x + j*2] = palette[*d];
			screen[(y + i*2 + 1) * WIDTH + x + j*2 + 1] = palette[*d++];
		}
	}
	update = 1;
}

void tft_update(void)
{
	update_texture();
	render_texture();
}

void tft_beep(unsigned freq, unsigned long duration)
{
}
