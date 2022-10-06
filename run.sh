#!/bin/bash
if [[ "$1" = "gdb" ]]; then
    qemu-system-x86_64 -k en-us -s -S -hda Image/x64BareBonesImage.qcow2 -m 512 -d int
else
    qemu-system-x86_64 -k en-us -hda Image/x64BareBonesImage.qcow2 -m 512 
fi
