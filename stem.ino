#include <SPI.h> /* pull in SPI driver */
#include "game.h"

void setup(void)
{
	Serial.begin(115200);

	game_init();

	Serial.println("setup done");
}

void loop(void)
{
	game_loop();
}
