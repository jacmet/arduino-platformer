#include "game.h"

int main(int argc, char **argv)
{
	game_init();

	while (1)
		game_loop();

	return 0;
}
