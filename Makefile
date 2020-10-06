CC = gcc
IDIR = src/include
CFLAGS = -g -c -I$(IDIR) -Wall
LFLAGS = -lX11 -lm

all: example

example: queue.o gfx.o example.o geometry.o
	$(CC) queue.o gfx.o example.o geometry.o -o example $(LFLAGS) 

queue.o: src/queue/queue.c
	$(CC) $(CFLAGS) src/queue/queue.c

gfx.o: src/gfx/gfx.c
	$(CC) $(CFLAGS) src/gfx/gfx.c

geometry.o: src/geometry/geometry.c
	$(CC) $(CFLAGS) src/geometry/geometry.c

example.o: example.c
	$(CC) $(CFLAGS) example.c

clean:
	rm *.o example
