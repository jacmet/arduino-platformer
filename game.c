#include <stdio.h>
#include <string.h>

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

#include "fries.h"
#include "donut.h"

#define WIDTH 320
#define HEIGHT 240

#define TILE 16
#define SCORE 3

#define PLAY_OFS 2
#define PLAY_WIDTH (TILE - 1 - PLAY_OFS)
#define PLAY_HEIGHT (TILE - 1)

#define COLS (WIDTH / TILE)
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

static int worldpos = 0;

struct Player
{
	int x, y;
	int speed_x, speed_y;
};

static struct Player player;

static unsigned char world[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 20, 1, 0, 2, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 3, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 30, 7, 7, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 9, 0, 0, 1, 1, 1,
	4, 0, 0, 0, 0, 31, 9, 0, 0, 0, 0, 1,
	5, 0, 0, 0, 0, 30, 9, 0, 0, 6, 6, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 1,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 20, 1,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
	0, 0, 4, 0, 0, 0, 0, 1, 1, 1, 1, 1,
	0, 0, 5, 0, 0, 0, 0, 1, 1, 1, 1, 1,
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

/* render column of index tile data to buf[TILE] */
static void render_tile_col(unsigned char *buf, int index, int col)
{
	const unsigned char *data = NULL;
	int i, j, ofs;

	switch (index) {
	case 1: data = tile_wall;  ofs=7; break;
	case 2: data = tile_tree1; ofs=10; break;
	case 3: data = tile_tree2; ofs=10; break;
	case 4: data = tile_cloud1; ofs=13; break;
	case 5: data = tile_cloud2; ofs=13; break;
	case 6: data = tile_tube1; ofs=15; break;
	case 7: data = tile_tube2; ofs=19; break;
	case 8: data = tile_square; ofs=22; break;
	case 9: data = tile_bridge; ofs=25; break;
	case 20: data = tile_girl0; ofs=50; break;
	case 21: data = tile_girl1; ofs=50; break;
	case 22: data = tile_girl2; ofs=50; break;
	case 23: data = tile_girl3; ofs=50; break;
	case 24: data = tile_boy0; ofs=50; break;
	case 25: data = tile_boy1; ofs=50; break;
	case 26: data = tile_boy2; ofs=50; break;
	case 27: data = tile_boy3; ofs=50; break;

	case 100: data = tile_dig0; ofs=5; break;
	case 101: data = tile_dig1; ofs=5; break;
	case 102: data = tile_dig2; ofs=5; break;
	case 103: data = tile_dig3; ofs=5; break;
	case 104: data = tile_dig4; ofs=5; break;
	case 105: data = tile_dig5; ofs=5; break;
	case 106: data = tile_dig6; ofs=5; break;
	case 107: data = tile_dig7; ofs=5; break;
	case 108: data = tile_dig8; ofs=5; break;
	case 109: data = tile_dig9; ofs=5; break;
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

void game_init(void)
{
	int x, y;

	tft_init();

	tft_setpal(0, SKY);
	tft_setpal(1, RED);
	tft_setpal(2, GREEN);
	tft_setpal(3, BLUE);
	tft_setpal(4, YELLOW);
	tft_setpal(5, WHITE);
	tft_setpal(6, LIGHTGREEN);
	tft_setpal(7, SKY);
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

	for (x = 0; x < COLS; x++)
		for (y = 0; y < ROWS; y++)
			draw_tile(x, SCORE+y, world[worldpos + y + x*ROWS]);

	font_puts("GAME", 0, 0, 10, 4, 0xf800, TRANSP);
	tft_cfg_scroll(32, HEIGHT-1);

	player.x = 20;
	player.y = 10;
}

void game_loop(void)
{
	int state;
	int on_floor = 0;
	int scrollpos = 0;
	int wpos = 0;

	int frame = 0;
	int mirror = 0;

	do {
		int x, y;
		int wx, wy;
		int sprite;

		state = js_state();

		x = player.x / TILE;
		y = player.y / TILE;

		player.speed_y += 1;
		player.speed_x = 0;//(player.speed_x * 3) / 4;

		if (on_floor && state & JS_UP)
			player.speed_y -= 12;

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
			if ((player.x - wpos) < (TILE - PLAY_OFS) ||
			    (world[(wx - 1) * ROWS + wy] == 1 && player.x < wx*TILE)) {
				player.x = (wx * TILE);// - PLAY_OFS;
				player.speed_x = 0;
			}
		} else {
			if (world[(wx + 1) * ROWS + wy] == 1 &&
			    (player.x + 15) > (wx+1) * TILE) {
				player.x = wx * TILE + PLAY_OFS;
				player.speed_x = 0;
			}
		}


		if (player.speed_y < 0) {
			/* check above */
			if (world[wx * ROWS + wy - 1] == 1 && player.y <= wy*TILE) {
				player.y = wy*TILE;
				player.speed_y = 0;
			}
		} else {
			/* check below */
			if (world[wx * ROWS + wy + 1] == 1 &&
			    (player.y + 15) > (wy + 1)*TILE) {
				player.y = (wy + 1) * TILE - 15;
				player.speed_y = 0;

				if (0)
				{
					static int first = 0;
					if ((first & 15) == 0)
						worldpos = (worldpos + ROWS) % (sizeof(world) - COLS*ROWS);

					for (y = 0; y < ROWS; y++)
						draw_tile_col(first/16, SCORE+y, world[worldpos + y + (COLS-1)*ROWS], first&15);
					first = (first + 1) % WIDTH;
					tft_scroll(first);
				}
				on_floor = 1;
			}
		}

		/* redraw */
		if (!on_floor)
			sprite = 23;
		else {
			if (!player.speed_x)
				sprite = 20;
			else
				sprite = (frame & 2) ? 21 : 22;
		}

		if (player.speed_x > 0)
			mirror = 0;
		else if (player.speed_x < 0)
			mirror = 1;

		{
			int xx = x % 20;

		draw_tile_player(xx, SCORE + y, world[y + x*ROWS], player.x % WIDTH, player.y+48, sprite, mirror);
		draw_tile_player(xx, SCORE + y+1, world[y + 1 + x*ROWS], player.x % WIDTH, player.y+48, sprite, mirror);
		x++;
		xx++;
		xx = xx % 20;
		draw_tile_player(xx, SCORE + y, world[y + x*ROWS], player.x % WIDTH, player.y+48, sprite, mirror);
		draw_tile_player(xx, SCORE + y+1, world[y + 1 + x*ROWS], player.x % WIDTH, player.y+48, sprite, mirror);
		}

		/* halfway past screen? scroll */
		if ((player.x - wpos) > WIDTH/2) {
			for (y = 0; y < ROWS; y++)
				draw_tile_col(scrollpos/TILE, SCORE+y,
					      world[y + (wpos/TILE + COLS-1)*ROWS], scrollpos & (TILE-1));

			wpos++;
			scrollpos++;
			if (scrollpos == WIDTH)
				scrollpos = 0;

			tft_scroll(scrollpos);
		}

		tft_update();
		frame++;
		delay(35);

	} while (!(state & JS_QUIT));
}
