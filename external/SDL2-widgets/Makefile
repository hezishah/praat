include ../../makefile.defs
OBJECTS := sdl-widgets.o shapes.o 
.SUFFIXES=
.PHONY: all clean
all: sdl-widgets.a testsw$(EXE_EXT)
CPPFLAGS += -Wno-missing-prototypes
sdl-widgets.a: $(OBJECTS)
	$(AR) rs $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) sdl-widgets.a
	$(RM) testsw.o


testsw$(EXE_EXT): testsw.o sdl-widgets.a
	$(LINK) testsw.o sdl-widgets.a $(LFLAGS) $(LIBS) -o $@
$(OBJECTS): *.h ../../sys/melder.h

sdl-widgets.o: sdl-widgets.h sw-pixmaps.h config.h shapes.h
testsw.o: sdl-widgets.h shapes.h
