# Toplevel makefile. use with make TARGET=native|arduino

ifeq ($(TARGET),native)
include native.mk
else ifeq ($(TARGET),arduino)
include arduino.mk
else
$(error TARGET not defined. Pass TARGET=native or TARGET=arduino)
endif

include/%.h: gfx/%.png
	./png2h.py $^ $(basename $(notdir $^)) > $@
