include ../../../makefile.defs

.SUFFIXES=

OBJ = hello.o ../sdl-widgets.a
hello$(EXE_EXT) : $(OBJ)
	$(LINK) $(OBJ) $(LFLAGS) $(LIBS) -o $@
all: hello$(EXE_EXT)
hello.o: ../sdl-widgets.h

clean:
	$(RM) *.o
