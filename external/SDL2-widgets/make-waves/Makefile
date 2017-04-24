include ../../../makefile.defs
OBJ := make-waves.o dump-wave.o ../sdl-widgets.a

.SUFFIXES=

make-waves: $(OBJ)
all: make-waves
make-waves.o: ../sdl-widgets.h dump-wave.h filter-test.cpp 
dump-wave.o: dump-wave.h

clean:
	$(RM) *.o
