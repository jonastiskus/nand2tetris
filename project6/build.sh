#!/bin/bash
echo "build running"

FLAGS="-g"
CLASSES="main.c instruction.c line.c symbol.c utils.c parser.c reader.c"
OUTPUT="hack_assembler"

gcc $FLAGS $CLASSES -o $OUTPUT
