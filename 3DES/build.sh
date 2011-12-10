#!/bin/bash -x

yasm -f macho32 -o "3desasm.obj" 3des.asm
gcc main.c 3des.c keys.c 3desasm.obj -o 3des -m32

