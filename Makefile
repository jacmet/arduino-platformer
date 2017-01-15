OBJS = emu.o font.o js.o tft.o

CFLAGS = -O2 -g -I include $(shell pkg-config --cflags sdl2)
LIBS = $(shell pkg-config --libs sdl2)

all: emu

emu: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f emu $(OBJS)
