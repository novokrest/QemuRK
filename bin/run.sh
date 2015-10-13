#!/bin/bash

DISK=test.qcow2

i386-softmmu/qemu-system-i386 -m 1024 -enable-kvm \
-hda $DISK -net nic -net user
