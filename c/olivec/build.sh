#!/usr/bin/env bash

set -xe

cc -Wall -Wextra -ggdb -o ./bin/text text.c
cc -Wall -Wextra -ggdb -o ./bin/test test.c
cc -Wall -Wextra -ggdb -DSDL_PLATFORM -o ./bin/triangle triangle.c -lm -lSDL3

clang -Wall -Wextra --target=wasm32 -o bin/wasm.o -c wasm.c
wasm-ld -m wasm32 --no-entry --export-all --allow-undefined -o bin/wasm.wasm bin/wasm.o

cc -Wall -Wextra -ggdb -DSDL_PLATFORM -o bin/ddd 3d.c -lm -lSDL3
clang -Wall -Wextra --target=wasm32 -o bin/ddd.o -c 3d.c
wasm-ld -m wasm32 --no-entry --export-all --allow-undefined -o bin/ddd.wasm bin/ddd.o

./bin/test
./bin/text
feh ./sample_output/text.png
# feh ./sample_output/alpha_blending.png
# ./bin/triangle
# ./bin/ddd
# python -m http.server 3333
