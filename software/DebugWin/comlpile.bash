#!/bin/bash -v

g++ -Wall `pkg-config gtk+-2.0 --cflags --libs` \
      bitmap/[a-z]*.c \
      raster/[a-z]*.c \
      DebugWin.cpp    \
      -o show_bmp     \
      $*

exit $?;

