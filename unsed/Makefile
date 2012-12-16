CC=gcc
CFLAGS=-c -O2 -Wall
LIBFLAGS=
EXECUTABLE=readlines-main
LIBRARY=libreadlines.so

all: readlines-main.o readlines.o
	$(CC) readlines-main.o readlines.o -o $(EXECUTABLE)

all-shared: readlines-main.o libreadlines
	$(CC) -L . readlines-main.o -lreadlines -o $(EXECUTABLE)

readlines-main.o:
	$(CC) $(CFLAGS) readlines-main.c

readlines.o:
	$(CC) $(CFLAGS) readlines.c

libreadlines: readlines-shared.o
	$(CC) -shared readlines-shared.o -o $(LIBRARY)

readlines-shared.o:
	$(CC) -fPIC -c readlines.c -o readlines-shared.o

clean:
	rm -f *.o $(EXECUTABLE) $(LIBRARY)