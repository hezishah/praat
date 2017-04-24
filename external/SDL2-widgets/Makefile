include ../../makefile.defs
OBJECTS := sdl-widgets.o shapes.o mingw32-specific.o

.SUFFIXES=
.PHONY: all clean
all: sdl-widgets.a

sdl-widgets.a: $(OBJECTS)
	$(AR) rs $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) sdl-widgets.a


sdl-widgets.a: $(OBJECTS)

$(OBJECTS): *.h ../../sys/melder.h