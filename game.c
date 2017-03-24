#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "game.h"
#include "js.h"
#include "tft.h"
#include "font.h"
#include "compat.h"

#include "wall.h"
#include "tree1.h"
#include "tree2.h"
#include "cloud1.h"
#include "cloud2.h"
#include "tube1.h"
#include "tube2.h"
#include "square.h"
#include "bridge.h"

#include "girl0.h"
#include "girl1.h"
#include "girl2.h"
#include "girl3.h"

#include "boy0.h"
#include "boy1.h"
#include "boy2.h"
#include "boy3.h"

#include "dig0.h"
#include "dig1.h"
#include "dig2.h"
#include "dig3.h"
#include "dig4.h"
#include "dig5.h"
#include "dig6.h"
#include "dig7.h"
#include "dig8.h"
#include "dig9.h"

#include "diamond.h"
#include "coin.h"
#include "fries.h"
#include "donut.h"

#include "world.h"

enum States {
	STATE_START,
	STATE_MENU,
	STATE_MAIN,
	STATE_GAMEOVER,
};

/* desired frame time (1 / frames per second) */
#define FRAMETIME (1000/30)

#define TILE 16
#define SCORE 1

/* height (y-coordinates) to consider player dead from
   (E.G. when falling into hole) */
#define DEAD_HEIGHT (HEIGHT - (SCORE + 1) * TILE - TILE/2)

/* maximum allowed speed in Y-direction (to not fall through the floor) */
#define MAX_YSPEED 10

#define PLAY_OFS 2
#define PLAY_WIDTH (TILE - 1 - PLAY_OFS)
#define PLAY_HEIGHT (TILE - 1)

#define PWIDTH (WIDTH - 32)
#define COLS (PWIDTH / TILE)
#define ROWS ((HEIGHT / TILE) - SCORE)

#define SKY 0x5ebf
#define BLACK       0x0000
#define BLUE	      0x001f
#define CYAN		    0x07ff
#define DARKCYAN    0x03EF
#define DARKGREEN	  0x03E0
#define DARKGREY    0x7BEF
#define GRAY1		    0x8410
#define GRAY2		    0x4208
#define GRAY3		    0x2104
#define GREEN       0x07e0
#define LIGHTGREEN  0xAFE5
#define LIGHTGREY   0xC618
#define MAGENTA     0xF81F
#define MAROON      0x7800
#define NAVY        0x000F
#define OLIVE       0x7BE0
#define ORANGE      0xFD20
#define PURPLE      0x780F
#define RED         0xf800
#define WHITE       0xffff
#define YELLOW      0xffe0

static unsigned points = 0;
static int sprite = 24;
static unsigned frame;
static int scrollpos;
/* is player facing left (mirrored) ? */
static int mirror;
/* is player touching floor? */
static int on_floor;
/* tile x coordinate corresponding to start of screen */
static int tpos;
static int rompos;

struct Player
{
	int x, y;
	int speed_x, speed_y;
};

static struct Player player;

#if 0
static const unsigned char romworld[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 13, 1, 0, 6, 1,
	0, 0, 0, 0, 0, 0, 12, 12, 1, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 12, 13, 1, 0, 7, 1,
	0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 1,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 30, 3, 3, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 5, 0, 0, 1, 1, 1,
	8, 0, 0, 0, 0, 31, 5, 0, 0, 0, 0, 1,
	9, 0, 0, 0, 0, 30, 5, 0, 0, 2, 2, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 0, 1,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 15, 1,
	0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 20, 1,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
	0, 0, 8, 0, 0, 0, 0, 1, 1, 1, 1, 1,
	0, 0, 9, 0, 0, 0, 0, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};
#endif
static unsigned char screen[ROWS * (COLS + 1)];

/* render column of index tile data to buf[TILE] */
static void render_tile_col(unsigned char *buf, int index, int col)
{
	const unsigned char *data = NULL;
	int i, ofs;

	switch (index) {
	case 1: data = tile_wall;  ofs=7; break;
	case 2: data = tile_tube1; ofs=15; break;
	case 3: data = tile_tube2; ofs=19; break;
	case 4: data = tile_square; ofs=22; break;
	case 5: data = tile_bridge; ofs=25; break;
	case 6: data = tile_tree1; ofs=10; break;
	case 7: data = tile_tree2; ofs=10; break;
	case 8: data = tile_cloud1; ofs=13; break;
	case 9: data = tile_cloud2; ofs=13; break;

	case 12: data = tile_diamond; ofs=60; break;
	case 13: data = tile_coin; ofs=65; break;
	case 14: data = tile_fries; ofs=70; break;
	case 15: data = tile_donut; ofs=77; break;

	case 20: data = tile_girl0; ofs=50; break;
	case 21: data = tile_girl1; ofs=50; break;
	case 22: data = tile_girl2; ofs=50; break;
	case 23: data = tile_girl3; ofs=50; break;
	case 24: data = tile_boy0; ofs=50; break;
	case 25: data = tile_boy1; ofs=50; break;
	case 26: data = tile_boy2; ofs=50; break;
	case 27: data = tile_boy3; ofs=50; break;

	case 100: data = tile_dig0; ofs=4; break;
	case 101: data = tile_dig1; ofs=4; break;
	case 102: data = tile_dig2; ofs=4; break;
	case 103: data = tile_dig3; ofs=4; break;
	case 104: data = tile_dig4; ofs=4; break;
	case 105: data = tile_dig5; ofs=4; break;
	case 106: data = tile_dig6; ofs=4; break;
	case 107: data = tile_dig7; ofs=4; break;
	case 108: data = tile_dig8; ofs=4; break;
	case 109: data = tile_dig9; ofs=4; break;
	}

	if (data) {
		/* render column 'col' */
		data += col * TILE/2;

		for (i = 0; i < TILE; i++) {
			unsigned char t = pgm_read_byte(&data[i / 2]);

			if (i & 1)
				t = t >> 4;
			else
				t = t & 0xf;

			if (t)
				t += ofs;
			*buf++ = t;
		}
	} else {
		memset(buf, 0, TILE);
	}
}

static void draw_tile_player(int col, int row, int index, int playerx, int playery, int player, int mirror)
{
	unsigned char tilebuf[TILE], playerbuf[TILE];
	int x;

	col *= TILE;
	row *= TILE;

	playerx -= col;
	playery -= row;

	for (x = 0; x < TILE; x++) {
		render_tile_col(tilebuf, index, x);

		if (x >= playerx && x < (playerx + TILE) && playery < TILE && (playery + TILE) > 0) {
			int start, end, startp, y;

			/* start/end in tilebuf coords */
			if (playery < 0) {
				start = 0;
				startp = -playery;
				end = playery + TILE;
			} else {
				start = playery;
				startp = 0;
				end = TILE;
			}

			if (end > start) {
				int play_x = x - playerx;
				if (mirror)
					play_x = TILE - 1 - play_x;
				render_tile_col(playerbuf, player, play_x);
			}

			for (y = start; y < end; y++) {
				if (playerbuf[startp])
					tilebuf[y] = playerbuf[startp];

				startp++;
			}
		}

		tft_blit8(col + x, row, 1, TILE, tilebuf);
	}

}

static void draw_tile(int col, int row, int index)
{
	draw_tile_player(col, row, index, -TILE, -TILE, 0, 0);
}

static void draw_tile_col(int col, int row, int index, int pos)
{
	unsigned char buf[16];

	render_tile_col(buf, index, pos);

	row *= TILE;
	col *= TILE;
	col += pos;

	tft_blit8(col, row, 1, TILE, buf);
}

static void draw_playerx2(int sx, int sy, int index, int background)
{
	unsigned char buf[TILE];
	int x, y;

	for (x = 0; x < TILE; x++) {
		render_tile_col(buf, index, x);

		for (y = 0; y < TILE; y++)
			if (!buf[y])
				buf[y] = background;

		tft_blit8x2(sx + x*2, sy, 1, TILE, buf);
	}
}

static void draw_player(int sx, int sy, int index, int background)
{
	unsigned char buf[TILE];
	int x, y;

	for (x = 0; x < TILE; x++) {
		render_tile_col(buf, index, x);

		for (y = 0; y < TILE; y++)
			if (!buf[y])
				buf[y] = background;

		tft_blit8(sx + x, sy, 1, TILE, buf);
	}
}

static void draw_points(void)
{
	char buf[8];
	int y;

	sprintf(buf, "%05u", points);

	for (y = 0; buf[y]; y++)
		draw_player(WIDTH-20, (5+y)*TILE, 100 + buf[y] - '0', 7);
}

static int solid_tile(unsigned char tile)
{
	return tile > 0 && tile < 6;
}

static int powerup_tile(unsigned char tile)
{
	return tile >= 12 && tile <= 15;
}

static void add_world_row(void)
{
	int y;

	memmove(screen, &screen[ROWS], ROWS*COLS);

	for (y = 0; y < ROWS/2; y++) {
		unsigned char d = pgm_read_byte(&tile_world[rompos++]);
		screen[ROWS*COLS + 2*y] = d & 0xf;
		screen[ROWS*COLS + 2*y + 1] = d >> 4;
	}
}

void game_init(void)
{
	int x, y;

	js_init();
	tft_init();

	tft_setpal(0, SKY);
	tft_setpal(1, RED);
	tft_setpal(2, GREEN);
	tft_setpal(3, BLUE);
	tft_setpal(4, YELLOW);
	tft_setpal(5, WHITE);
	tft_setpal(6, LIGHTGREEN);
	tft_setpal(7, BLACK);
	tft_setpal(8, WALL_COLOR1);
	tft_setpal(9, WALL_COLOR2);
	tft_setpal(10, WALL_COLOR3);
	tft_setpal(11, TREE1_COLOR1);
	tft_setpal(12, TREE1_COLOR2);
	tft_setpal(13, TREE1_COLOR3);
	tft_setpal(14, CLOUD1_COLOR1);
	tft_setpal(15, CLOUD1_COLOR2);
	tft_setpal(16, TUBE1_COLOR1);
	tft_setpal(17, TUBE1_COLOR2);
	tft_setpal(18, TUBE1_COLOR3);
	tft_setpal(19, TUBE1_COLOR4);
	tft_setpal(20, TUBE2_COLOR1);
	tft_setpal(21, TUBE2_COLOR2);
	tft_setpal(22, TUBE2_COLOR3);
	tft_setpal(23, SQUARE_COLOR1);
	tft_setpal(24, SQUARE_COLOR2);
	tft_setpal(25, SQUARE_COLOR3);
	tft_setpal(26, BRIDGE_COLOR1);
	tft_setpal(27, BRIDGE_COLOR2);
	tft_setpal(28, BRIDGE_COLOR3);

	tft_setpal(51, GIRL0_COLOR1);
	tft_setpal(52, GIRL0_COLOR2);
	tft_setpal(53, GIRL0_COLOR3);
	tft_setpal(54, GIRL0_COLOR4);
	tft_setpal(55, GIRL0_COLOR5);
	tft_setpal(56, GIRL0_COLOR6);

	tft_setpal(61, DIAMOND_COLOR1);
	tft_setpal(62, DIAMOND_COLOR2);
	tft_setpal(63, DIAMOND_COLOR3);

	tft_setpal(66, COIN_COLOR1);
	tft_setpal(67, COIN_COLOR2);
	tft_setpal(68, COIN_COLOR3);
	tft_setpal(69, COIN_COLOR4);

	tft_setpal(71, FRIES_COLOR1);
	tft_setpal(72, FRIES_COLOR2);
	tft_setpal(73, FRIES_COLOR3);
	tft_setpal(74, FRIES_COLOR4);
	tft_setpal(75, FRIES_COLOR5);
	tft_setpal(76, FRIES_COLOR6);
	tft_setpal(77, FRIES_COLOR7);

	tft_setpal(78, DONUT_COLOR1);
	tft_setpal(79, DONUT_COLOR2);
	tft_setpal(80, DONUT_COLOR3);
	tft_setpal(81, DONUT_COLOR4);
	tft_setpal(82, DONUT_COLOR5);
	tft_setpal(83, DONUT_COLOR6);
	tft_setpal(84, DONUT_COLOR7);
	tft_setpal(85, DONUT_COLOR7);
}

static void main_init(void)
{
	int x, y;

	rompos = 0;
	for (x = 0; x < (COLS+1); x++)
		add_world_row();

	for (x = 0; x < COLS; x++) {
		for (y = 0; y < SCORE; y++)
			draw_tile(x, y, 0);

		for (y = 0; y < ROWS; y++)
			draw_tile(x, SCORE+y, screen[y + x*ROWS]);
	}

	player.x = 20;
	player.y = 10;
	player.speed_x = player.speed_y = 0;

	points = 0;
	tpos = 0;
	scrollpos = 0;
	mirror = 0;
	on_floor = 0;

	tft_cfg_scroll(WIDTH-PWIDTH, 0);

	tft_fill(PWIDTH, 0, 1, HEIGHT, GRAY1);
	tft_fill(PWIDTH+1, 0, 1, HEIGHT, GRAY2);
	tft_fill(PWIDTH+2, 0, 1, HEIGHT, GRAY3);
	tft_fill(PWIDTH+3, 0, WIDTH-PWIDTH-3, HEIGHT, BLACK);

	font_puts("SIBEs", WIDTH-29, 10, 1, 1, WHITE, BLACK);
	font_putc('S', WIDTH-27+0*FONT_W, 22, 1, 1, 0x7f77, TRANSP);
	font_putc('T', WIDTH-27+1*FONT_W, 22, 1, 1, 0xffcf, TRANSP);
	font_putc('E', WIDTH-27+2*FONT_W, 22, 1, 1, 0xeb6d, TRANSP);
	font_putc('M', WIDTH-27+3*FONT_W, 22, 1, 1, 0x547d, TRANSP);
	font_puts("Feest", WIDTH-29, 34, 1, 1, WHITE, BLACK);

	draw_points();
}

static int main_loop(void)
{
	int wy, wx;
	int state = js_state();
	int x = player.x / TILE;
	int y = player.y / TILE;

	if (player.speed_y < MAX_YSPEED)
		player.speed_y += 1;
	player.speed_x = 0;

	if (on_floor && state & JS_UP) {
		player.speed_y -= 12;
		tft_beep(100, 50);
	}
	if (state & JS_RIGHT)
		player.speed_x += 1;

	if (state & JS_LEFT)
		player.speed_x -= 1;

	wx = (player.x + 8) / TILE;
	wy = (player.y + 8) / TILE;

	player.x += player.speed_x;
	player.y += player.speed_y;

	on_floor = 0;

	if (player.speed_x < 0) {
		/* check left */
		if (player.x < (TILE - PLAY_OFS) ||
			(solid_tile(screen[(wx - 1) * ROWS + wy]) && player.x < wx*TILE)) {
			player.x = (wx * TILE);// - PLAY_OFS;
			player.speed_x = 0;
		}
	} else {
		if (solid_tile(screen[(wx + 1) * ROWS + wy]) &&
			(player.x + 15) > (wx+1) * TILE) {
			player.x = wx * TILE + PLAY_OFS;
			player.speed_x = 0;
		}
	}

	if (player.speed_y < 0) {
		/* check above */
		if (solid_tile(screen[wx * ROWS + wy - 1]) && player.y <= wy*TILE) {
			player.y = wy*TILE;
			player.speed_y = 0;
		}
	} else {
		/* check below */
		if (solid_tile(screen[wx * ROWS + wy + 1]) &&
			(player.y + 15) > (wy + 1)*TILE) {
			player.y = (wy + 1) * TILE - 15;
			player.speed_y = 0;

			on_floor = 1;
		}
	}

	wx = (player.x + 8) / TILE;
	wy = (player.y + 8) / TILE;

	if (powerup_tile(screen[wx * ROWS + wy])) {
		switch (screen[wx * ROWS + wy]) {
		case 12: points += 1; break;
		case 13: points += 2; break;
		case 14: points += 5; break;
		case 15: points += 10; break;
		}
		screen[wx * ROWS + wy] = 0;
		tft_beep(500, 20);
		draw_points();
	}

	/* fall into hole? */
	if (player.y > DEAD_HEIGHT)
		return 1;

	/* redraw */
	if (!on_floor)
		sprite |= 3;
	else {
		sprite &= ~3;
		if (player.speed_x)
			sprite |= (frame & 4) ? 1 : 2;
	}

	if (player.speed_x > 0)
		mirror = 0;
	else if (player.speed_x < 0)
		mirror = 1;

	{
		int xx = (x + tpos) % COLS;
		int px = (player.x + tpos * TILE) % PWIDTH;

		draw_tile_player(xx, SCORE + y, screen[y + x*ROWS], px, player.y + SCORE*TILE, sprite, mirror);
		draw_tile_player(xx, SCORE + y+1, screen[y + 1 + x*ROWS], px, player.y + SCORE*TILE, sprite, mirror);
		x++;
		xx++;

		/* handle wraparound */
		if (xx >= COLS) {
			xx -= COLS;
			draw_tile_player(xx, SCORE + y, screen[y + x*ROWS], px - PWIDTH, player.y + SCORE*TILE, sprite, mirror);
			draw_tile_player(xx, SCORE + y+1, screen[y + 1 + x*ROWS], px - PWIDTH, player.y + SCORE*TILE, sprite, mirror);
		} else {
			draw_tile_player(xx, SCORE + y, screen[y + x*ROWS], px, player.y + SCORE*TILE, sprite, mirror);
			draw_tile_player(xx, SCORE + y+1, screen[y + 1 + x*ROWS], px, player.y + SCORE*TILE, sprite, mirror);
		}
	}

	/* halfway past screen? scroll */
	if (player.x > PWIDTH/2) {
		for (y = 0; y < ROWS; y++)
			draw_tile_col(scrollpos/TILE, SCORE+y,
				      screen[y + COLS*ROWS], scrollpos & (TILE-1));

		scrollpos++;
		if (scrollpos == PWIDTH)
			scrollpos = 0;

		if ((scrollpos & (TILE-1)) == 0) {
			player.x -= TILE;
			tpos++;
			add_world_row();
		}
		tft_scroll(scrollpos);
	}

	if (state & JS_QUIT)
		return -1;

	return 0;
}


static void menu_init(void)
{
	tft_fill(0, 0, WIDTH, HEIGHT, 0);

	font_puts("SIBEs", 51, 26, 8, 3, 0x7777, TRANSP);
	font_puts("SIBEs", 50, 25, 8, 3, 0xffff, TRANSP);

	font_puts("STEM Spelletje", 41, 61, 3, 4, 0x7777, TRANSP);
	font_puts("STEM Spelletje", 40, 60, 3, 4, 0xffff, TRANSP);
	font_putc('S', 40+0*3*FONT_W, 60, 3, 4, 0x7f77, TRANSP);
	font_putc('T', 40+1*3*FONT_W, 60, 3, 4, 0xffcf, TRANSP);
	font_putc('E', 40+2*3*FONT_W, 60, 3, 4, 0xeb6d, TRANSP);
	font_putc('M', 40+3*3*FONT_W, 60, 3, 4, 0x547d, TRANSP);

	font_puts("Kies en speler", 76, 111, 2, 2, 0x7777, TRANSP);
	font_puts("Kies en speler", 75, 110, 2, 2, 0xffff, TRANSP);

	sprite &= ~3;
}

static int menu_loop(void)
{
	int boy, boybg, girl, girlbg;
	unsigned boytx, girltx;
	int state = js_state();

	boy = 24;
	girl = 20;
	boybg = girlbg = 7;
	boytx = girltx = WHITE;

	if (sprite == boy) {
		boy |= (frame & 4) ? 1 : 2;
		boybg = 1;
		boytx = RED;

		if (state & JS_RIGHT) {
			sprite = 20;
			tft_beep(100, 50);
		}
	} else {
		girl |= (frame & 4) ? 1 : 2;
		girlbg = 1;
		girltx = RED;

		if (state & JS_LEFT) {
			sprite = 24;
			tft_beep(100, 50);
		}
	}

	draw_playerx2(80, 140, boy, boybg);
	font_puts("Sibe", 84, 175, 1, 2, boytx, TRANSP);

	draw_playerx2(200, 140, girl, girlbg);
	font_puts("Lone", 204, 175, 1, 2, girltx, TRANSP);

	return (state & JS_FIRE);
}

static void gameover_init(void)
{
	tft_scroll(0);

	tft_beep(50, 300);
	font_puts("GAME", 20, 10, 11, 14, RED, TRANSP);
	font_puts("OVER", 20, 130, 11, 14, RED, TRANSP);

	delay(300);
	tft_beep(50, 300);
}

static int gameover_loop(void)
{
	static int pressed;

	/* wait until fire is pressed and again released */
	if (!pressed)
		pressed = js_state() & JS_FIRE;
	else
		if (!(js_state() & JS_FIRE)) {
			pressed = 0;
			return 1;
		}

	return 0;
}

void game_loop(void)
{
	static int state;
	unsigned delta, start = millis();

	switch (state) {
	case STATE_START:
		menu_init();
		state = STATE_MENU;
		break;

	case STATE_MENU:
		if (menu_loop()) {
		    main_init();
			state = STATE_MAIN;
		}
		break;

	case STATE_MAIN:
		switch (main_loop()) {
		case -1:
			exit(0);

		case 0:
			break;

		default:
			gameover_init();
			state = STATE_GAMEOVER;
		}
		break;

	case STATE_GAMEOVER:
		if (gameover_loop())
			state = STATE_START;
		break;
	}

	tft_update();
	frame++;

	delta = millis() - start;
	if (delta < FRAMETIME)
		delay(FRAMETIME - delta);
}
