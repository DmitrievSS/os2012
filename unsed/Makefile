CC=gcc
CFLAGS=-c -O2 -Wall
EXECUTABLE=readlines-main

all: readlines-main

readlines-main: readlines-main.o readlines.o
	$(CC) readlines-main.o readlines.o -o $(EXECUTABLE)

readlines-main.o:
	$(CC) $(CFLAGS) readlines-main.c

readlines.o:
	$(CC) $(CFLAGS) readlines.c

clean:
	rm -f *.o $(EXECUTABLE)