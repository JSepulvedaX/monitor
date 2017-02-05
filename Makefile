CC      := gcc

BFLAGS  := -Wall -I/usr/local/include/libxml2 -I/libxml

AFLAGS  := -lxml2 -lpthread -lm

all: monitor

monitor: main.c main.h
	$(CC) $(BFLAGS) -o $@ main.c $(AFLAGS)

install:

clean:
	rm -f monitor
