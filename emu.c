#include <stdio.h>
#include <string.h>
#include <unistd.h>

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
#include "girl.h"
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
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 1,

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

static unsigned char tile0[256] = { };
static unsigned char tile1[256] = {
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x1,
	0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x1,
	0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x1,
	0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x1,
	0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x1,
	0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x1,
	0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x1,
	0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x1,
	0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x1,
	0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x1,
	0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x1,
	0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x1,
	0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
};

static unsigned char tile2[256] = {
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x0,
	0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x0,
	0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x0,
	0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x0,
	0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x0,
	0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x0,
	0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x0,
	0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x0,
	0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x0,
	0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x0,
	0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x0,
	0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x0,
	0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x5, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};

static void draw_tile(int col, int row, int index)
{
	unsigned char buf[256], *data;
	int i, x, y;

	switch (index) {
	case 0: data = tile0; break;
	case 2: data = tile2; break;
	default:
		data = tile1; break;
	}

	memset(buf, 7, sizeof(buf));

	for (i = 0; i < sizeof(buf); i++)
		if (data[i])
			buf[i] = data[i];

	row *= TILE;
	col *= TILE;

	tft_blit8(col, row, TILE, TILE, buf);
}

static void draw_tile16(int col, int row, int index)
{
	unsigned char buf[256];
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
	case 20: data = tile_girl; ofs=50; break;
	case 30: data = tile_fries; ofs=70; break;
	case 31: data = tile_donut; ofs=77; break;

	}

	if (data) {
		for (i = 0, j = 0; i < sizeof(buf)/2; i++) {
			unsigned char t1 = (data[i] & 0xf);
			unsigned char t2 = (data[i] >> 4);
			if (t1)
				t1 += ofs;
			if (t2)
				t2 += ofs;
			buf[j++] = t1;
			buf[j++] = t2;
		}
	} else {
		memset(buf, 0, sizeof(buf));
	}

	row *= TILE;
	col *= TILE;

	tft_blit8(col, row, TILE, TILE, buf);
}

static void draw_tile16row(int col, int row, int index, int pos)
{
	unsigned char buf[16];
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
	case 20: data = tile_girl; ofs=50; break;
	}

	if (data) {
		for (i = 0; i < sizeof(buf); i++) {
			unsigned char t = data[i * 8 + pos/2];

			if (pos & 1)
				t = t >> 4;
			else
				t = t & 0xf;
			if (t)
				t += ofs;

			buf[i] = t;
		}
	} else {
		memset(buf, 0, sizeof(buf));
	}

	row *= TILE ;
	col *= TILE;
	col += pos;

	tft_blit8(col, row, 1, TILE, buf);
}

int main(int argc, char **argv)
{
	int x, y;
	int state;
	int on_floor = 0;

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

	tft_setpal(51, GIRL_COLOR1);
	tft_setpal(52, GIRL_COLOR2);
	tft_setpal(53, GIRL_COLOR3);
	tft_setpal(54, GIRL_COLOR4);
	tft_setpal(55, GIRL_COLOR5);
	tft_setpal(56, GIRL_COLOR6);

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
			draw_tile16(x, SCORE+y, world[worldpos + y + x*ROWS]);

	font_puts("GAME", 0, 0, 10, 4, 0xf800, TRANSP);
	tft_cfg_scroll(32, HEIGHT-1);
	player.x = 20;
	player.y = 10;

	do {
		int wx, wy;

		state = js_state();

		/* erase old player position */
		x = player.x / TILE;
		y = player.y / TILE;
//		draw_tile16(x, SCORE+y, world[worldpos + y + x*ROWS]);
//		draw_tile16(x+1, SCORE+y, world[worldpos + y + (x+1)*ROWS]);
		y++;
//		draw_tile16(x, SCORE+y, world[worldpos + y + x*ROWS]);
//		draw_tile16(x+1, SCORE+y, world[worldpos + y + (x+1)*ROWS]);

		player.speed_y += 1;
		player.speed_x = (player.speed_x * 3) / 4;

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
			if (player.x < (TILE - PLAY_OFS) ||
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
				{
					static int first = 0;
					if ((first & 15) == 0)
						worldpos = (worldpos + ROWS) % (sizeof(world) - COLS*ROWS);

					for (y = 0; y < ROWS; y++)
						draw_tile16row(first/16, SCORE+y, world[worldpos + y + (COLS-1)*ROWS], first&15);
					first = (first + 1) % WIDTH;
					tft_scroll(first);
				}
				on_floor = 1;
			}
		}

//		tft_fill(player.x, 48 + player.y, 16, 16, YELLOW);
		tft_update();
		usleep(35000);

	} while (!(state & JS_QUIT));

	return 0;
}
