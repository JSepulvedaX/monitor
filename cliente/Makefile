CC      := gcc

BFLAGS  := -Wall

AFLAGS  := -lpthread

all: cliente

cliente: main.c main.h
	$(CC) $(BFLAGS) -o $@ main.c $(AFLAGS)

install:

clean:
	rm -f cliente
