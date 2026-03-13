#!/bin/sh

set -xe

cc -Wall -Wextra -ggdb -o ./bin/example example.c
cc -Wall -Wextra -ggdb -o ./bin/test test.c
clang -Wall -Wextra --target=wasm32 --no-standard-libraries -Wl,--no-entry -Wl,--export-all -Wl,--allow-undefined -o wasm.wasm ./wasm.c

./bin/example
./bin/test
