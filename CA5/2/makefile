CC=g++-10

CFLAGS=-c
LNKFLAGS=-fopenmp 

all: main

main: main.o 
	$(CC) main.o -o main -lpthread

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

clean:
	rm -f *o main