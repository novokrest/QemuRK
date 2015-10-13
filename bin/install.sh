#!/bin/bash

IMAGE=debian-8.2.0-i386-netinst.iso
DISK=test.qcow2

rm -f $DISK
./qemu-img create -f qcow2 $DISK 3G

i386-softmmu/qemu-system-i386 -m 1024 -enable-kvm \
-drive if=virtio,file=$DISK,cache=none \
-cdrom $IMAGE
