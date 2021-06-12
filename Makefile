CC = gcc
IDIR = include
SRCDIR = src
CFLAGS = -g -c -I$(IDIR) -Wall
LFLAGS = -lX11 -lm

all: example

example: queue.o gfx.o example.o geometry.o algos.o
	$(CC) queue.o gfx.o example.o geometry.o algos.o -o example $(LFLAGS) 

queue.o: $(SRCDIR)/queue.c
	$(CC) $(CFLAGS) $(SRCDIR)/queue.c

gfx.o: $(SRCDIR)/gfx.c
	$(CC) $(CFLAGS) $(SRCDIR)/gfx.c

geometry.o: $(SRCDIR)/geometry.c
	$(CC) $(CFLAGS) $(SRCDIR)/geometry.c

geometry.o: $(SRCDIR)/algos.c
	$(CC) $(CFLAGS) $(SRCDIR)/algos.c

example.o: example.c
	$(CC) $(CFLAGS) example.c

clean:
	rm *.o example
