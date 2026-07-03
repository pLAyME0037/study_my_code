#!/usr/bin/env bash

set -xe

# cc -Wall -Wextra -ggdb -o ./bin/text text.c
# ./bin/text
# feh ./sample_output/text.png
# feh ./sample_output/alpha_blending.png

# cc -Wall -Wextra -ggdb -o ./bin/test test.c
# ./bin/test

clang -O2 -Wall -Wextra -ggdb -DPLATFORM=SDL_PLATFORM -I. -o ./bin/triangle.sdl triangle.c -lm -lSDL3
# ./bin/triangle.sdl

clang -Os -Wall -Wextra --target=wasm32 -DPLATFORM=WASM_PLATFORM -o bin/wasm.o -c wasm.c
wasm-ld -m wasm32 --no-entry --export-all --allow-undefined -o bin/wasm.wasm bin/wasm.o

clang -O2 -Wall -Wextra -ggdb -DPLATFORM=TERM_PLATFORM -I. -o ./bin/triangle.term triangle.c -lm -lSDL3
./bin/triangle.term

# cc -Wall -Wextra -ggdb -DSDL_PLATFORM -o bin/ddd 3d.c -lm -lSDL3
# clang -Wall -Wextra --target=wasm32 -o bin/ddd.o -c 3d.c
# wasm-ld -m wasm32 --no-entry --export-all --allow-undefined -o bin/ddd.wasm bin/ddd.o
# ./bin/ddd

# python -m http.server 3333
