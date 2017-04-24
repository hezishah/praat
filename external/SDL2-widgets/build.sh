emmake make
emcc --separate-asm -O3 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 make-waves/make-waves.bc -s ASYNCIFY=1 -o build/index.html --embed-file fonts
nw build/
