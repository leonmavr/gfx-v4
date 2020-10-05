CC = gcc
IDIR = src/include
CFLAGS = -g -c -I$(IDIR) -Wall
LFLAGS = -lX11 -lm

all: example

example: queue.o gfx.o example.o rotations.o
	$(CC) $(LFLAGS) queue.o gfx.o example.o rotations.o -o example

queue.o: src/queue/queue.c
	$(CC) $(CFLAGS) src/queue/queue.c

gfx.o: src/gfx/gfx.c
	$(CC) $(CFLAGS) src/gfx/gfx.c

rotations.o: src/rotations/rotations.c
	$(CC) $(CFLAGS) src/rotations/rotations.c

example.o: example.c
	$(CC) $(CFLAGS) example.c

clean:
	rm *.o example
