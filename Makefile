CC=gcc
CFLAGS=`pkg-config --cflags raylib`
LIBS=`pkg-config --libs raylib` -lm

all: pathfinding

pathfinding: main.c
	$(CC) main.c -o pathfinding $(CFLAGS) $(LIBS)

clean:
	rm -f pathfinding
