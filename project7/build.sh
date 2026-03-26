#!/bin/bash
echo "build running"

FLAGS="-g"
CLASSES="main.c line.c"
OUTPUT="vm_translator"

gcc $FLAGS $CLASSES -o $OUTPUT
