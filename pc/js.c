/* SDL2 implementation */
#include "SDL.h"
#include "js.h"

static int state;

static void poll_input(void)
{
	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
			case SDLK_ESCAPE:
			case SDLK_q:     state |= JS_QUIT; break;
			case SDLK_SPACE: state |= JS_FIRE; break;
			case SDLK_LEFT:  state |= JS_LEFT; break;
			case SDLK_RIGHT: state |= JS_RIGHT; break;
			case SDLK_UP:    state |= JS_UP; break;
			case SDLK_DOWN:  state |= JS_DOWN; break;
			}
			break;

		case SDL_KEYUP:
			switch (e.key.keysym.sym) {
			case SDLK_SPACE: state &= ~JS_FIRE; break;
			case SDLK_LEFT:  state &= ~JS_LEFT; break;
			case SDLK_RIGHT: state &= ~JS_RIGHT; break;
			case SDLK_UP:    state &= ~JS_UP; break;
			case SDLK_DOWN:  state &= ~JS_DOWN; break;
			}
			break;

		case SDL_QUIT:
			state |= JS_QUIT;
			break;

		}
	}
}

void js_init(void)
{
	/* all SDL init happens in tft.c */
}

/* is fire button pressed */
int js_fire(void)
{
	poll_input();

	return state & JS_FIRE;
}

/* +/- 511 */
int js_x(void)
{
	poll_input();

	if (state & JS_LEFT)
		return -511;

	if (state & JS_RIGHT)
		return 511;

	return 0;
}

/* +/- 511 */
int js_y(void)
{
	poll_input();

	if (state & JS_DOWN)
		return -511;

	if (state & JS_UP)
		return 511;

	return 0;
}


/* get current state bitmask (JS_*) */
int js_state(void)
{
	poll_input();

	return state;
}
