CC = gcc
IDIR = include
SRCDIR = src
CFLAGS = -g -c -I$(IDIR) -Wall
LFLAGS = -lX11 -lm

all: example

example: queue.o gfx.o example.o geometry.o
	$(CC) queue.o gfx.o example.o geometry.o -o example $(LFLAGS) 

queue.o: $(SRCDIR)/queue/queue.c
	$(CC) $(CFLAGS) $(SRCDIR)/queue/queue.c

gfx.o: $(SRCDIR)/gfx/gfx.c
	$(CC) $(CFLAGS) $(SRCDIR)/gfx/gfx.c

geometry.o: $(SRCDIR)/geometry/geometry.c
	$(CC) $(CFLAGS) $(SRCDIR)/geometry/geometry.c

example.o: example.c
	$(CC) $(CFLAGS) example.c

clean:
	rm *.o example
