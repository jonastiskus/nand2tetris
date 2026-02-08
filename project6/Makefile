CC=gcc
CFLAGS=-Wall -Wextra

CFILES=main.c symbols.c reader.c utils.c parser.c

BINARY=bin

all: $(BINARY)

$(BINARY): $(CFILES)
	$(CC) $(CFLAGS) $(CFILES) -o $(BINARY)
