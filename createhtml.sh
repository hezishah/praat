emcc praat.bc --closure 0 -O2 -s USE_PTHREADS=2 -s DISABLE_EXCEPTION_CATCHING=0 -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2 -s TOTAL_MEMORY=268435456 -o index.html --embed-file fonts
