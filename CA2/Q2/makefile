CC=g++

CFLAGS=-c

INCDIR=-I

all: main

main: main.o 
	$(CC)  main.o -o main

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -msse3

clean:
	rm -f *o main