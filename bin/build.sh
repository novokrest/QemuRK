#!/bin/bash

QEMU_ROOT=..

$QEMU_ROOT/configure --target-list=i386-softmmu --enable-debug
make -j4

