OBJS = emu.o pc/font.o pc/js.o pc/tft.o

CFLAGS = -O2 -g -I include -DSDL_ASSERT_LEVEL=2 $(shell pkg-config --cflags sdl2)
LIBS = $(shell pkg-config --libs sdl2)

all: emu

emu: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f emu $(OBJS)
