#!/usr/bin/env bash

set -xe

cc -Wall -Wextra -ggdb -o ./bin/example example.c
cc -Wall -Wextra -ggdb -o ./bin/test test.c
cc -Wall -Wextra -ggdb -DSDL_PLATFORM -o ./bin/triangle triangle.c -lm -lSDL3
cc -Wall -Wextra -ggdb -DSDL_PLATFORM -o ./bin/ddd 3d.c -lm -lSDL3
clang -Wall -Wextra --target=wasm32 -o wasm.o -c ./wasm.c
wasm-ld -m wasm32 --no-entry --export-all --allow-undefined -o wasm.wasm wasm.o

./bin/example
./bin/test
# ./bin/triangle
./bin/ddd
# feh ./sample_output/alpha_blending.png
