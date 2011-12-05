#!/bin/bash -x

yasm -f macho32 -o "3desasm.obj" 3des.asm
cc main.c 3des.c evil.c keys.c 3desasm.obj -o 3des -m32

