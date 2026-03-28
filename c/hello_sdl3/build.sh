#!/usr/bin/bash
set -xe

export PKG_CONFIG_PATH=$HOME/opt/SDL3/lib/pkgconfig/
cc $(pkg-config --cflags sdl3) -o main main.c $(pkg-config --libs sdl3)

./main
