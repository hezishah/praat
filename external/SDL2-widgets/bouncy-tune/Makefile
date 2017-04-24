include ../../../makefile.defs

OBJ := bouncy-tune.o ../make-waves/dump-wave.o ../sdl-widgets.a

.SUFFIXES=

bouncy-tune.bc: $(OBJ)

bouncy-tune.o: ../sdl-widgets.h ../make-waves/dump-wave.h templates.h
../make-waves/dump-wave.o: ../make-waves/dump-wave.h
