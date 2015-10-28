#!/bin/bash

QEMU_ROOT=..

CFLAGS="-g3 -O0" $QEMU_ROOT/configure --target-list=i386-softmmu --enable-debug --disable-pie --enable-debug-info --enable-debug-tcg
make -j4

