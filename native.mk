# Makefile for native compilation. Needs sdl2

OBJS = emu.o game.o font.o pc/js.o pc/tft.o

CFLAGS = -O2 -g -I include -DSDL_ASSERT_LEVEL=2 $(shell pkg-config --cflags sdl2)
LIBS = $(shell pkg-config --libs sdl2) -lrt

all: emu

emu: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f emu $(OBJS)

game.o: game.c include/game.h include/js.h include/tft.h \
	include/font.h include/compat.h include/wall.h \
	include/tree1.h include/tree2.h include/cloud1.h \
	include/cloud2.h include/tube1.h include/tube2.h \
	include/square.h include/bridge.h include/girl0.h \
	include/girl1.h include/girl2.h include/girl3.h \
	include/boy0.h include/boy1.h include/boy2.h \
	include/boy3.h include/dig0.h include/dig1.h \
	include/dig2.h include/dig3.h include/dig4.h \
	include/dig5.h include/dig6.h include/dig7.h \
	include/dig8.h include/dig9.h include/diamond.h \
	include/coin.h include/fries.h include/donut.h \
	include/world.h
