CC = gcc
IDIR =./src/include
CFLAGS = -c -I$(IDIR) 
LFLAGS = -lX11

all: example

example: queue.o gfx.o example.o
	$(CC) $(LFLAGS) queue.o gfx.o example.o -o example

queue.o: src/queue/queue.c
	$(CC) $(CFLAGS) src/queue/queue.c

gfx.o: src/gfx/gfx.c
	$(CC) $(CFLAGS) src/gfx/gfx.c

example.o: example.c
	$(CC) -c example.c

clean:
	rm *.o example
