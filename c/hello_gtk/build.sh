#!/usr/bin/env bash

# gcc $(pkg-config --cflags gtk4) -o bin/hello_world_gtk hello_world_gtk.c $(pkg-config --libs gtk4)
# bin/hello_world_gtk

gcc $(pkg-config --cflags gtk4) -o bin/drawing_area drawing_area.c $(pkg-config --libs gtk4)
bin/drawing_area
