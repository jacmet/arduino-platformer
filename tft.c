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

static void update_texture(void)
{
	SDL_UpdateTexture(texture, NULL, screen, WIDTH * sizeof(screen[0]));
}

static void render_texture(void)
{
	SDL_Rect src, dst;

	SDL_RenderClear(renderer);

	/* top */
	if (scroll_top) {
		src.x = src.y = 0;
		src.w = WIDTH;
		src.h = scroll_top;

		SDL_RenderCopy(renderer, texture, &src, &src);
	}

	/* scrolling area */
	src.x = scroll_pos;
	src.y = scroll_top;
	src.w = WIDTH - scroll_pos;
	src.h = scroll_bottom + 1 - scroll_top;
	dst.x = 0;
	dst.y = scroll_top;
	dst.w = WIDTH - scroll_pos;
	dst.h = scroll_bottom + 1 - scroll_top;

	SDL_RenderCopy(renderer, texture, &src, &dst);

	if (scroll_pos) {
		src.x = 0;
		src.y = scroll_top;
		src.w = scroll_pos;
		src.h = scroll_bottom + 1 - scroll_top;
		dst.x = WIDTH - scroll_pos;
		dst.y = scroll_top;
		dst.w = scroll_pos;
		dst.h = scroll_bottom + 1 - scroll_top;
		SDL_RenderCopy(renderer, texture, &src, &dst);
	}

	/* bottom */
	if (scroll_bottom != (HEIGHT-1)) {
		src.x = 0;
		src.y = scroll_bottom;
		src.w = WIDTH;
		src.h = HEIGHT - scroll_bottom;

		SDL_RenderCopy(renderer, texture, &src, &src);
	}

	SDL_RenderPresent(renderer);
}

void tft_init(void)
{
	int i, x, y, p;

	/* Initialize SDL. */
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		exit(1);

	window = SDL_CreateWindow("tft",
				  SDL_WINDOWPOS_UNDEFINED,
				  SDL_WINDOWPOS_UNDEFINED,
				  WIDTH, HEIGHT,
				  SDL_WINDOW_FULLSCREEN_DESKTOP);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

	texture = SDL_CreateTexture(renderer,
				    SDL_PIXELFORMAT_RGB565,
				    SDL_TEXTUREACCESS_STREAMING,
				    WIDTH, HEIGHT);
	if (!window || !renderer || !texture)
		exit(2);

	scroll_pos = scroll_top = 0;
	scroll_bottom = HEIGHT-1;

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
	SDL_assert(top < bottom);
	SDL_assert(bottom < HEIGHT);

	scroll_top = top;
	scroll_bottom = bottom;
	update = 1;
}

void tft_update(void)
{
	update_texture();
	render_texture();
}
