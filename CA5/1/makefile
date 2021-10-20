CC=g++

CFLAGS=-c
INCDIR=-I.

all: out

out: 1.o 
	$(CC) 1.o -o out -lpthread

1.o: 1.cpp
	$(CC) $(CFLAGS) $(INCDIR) 1.cpp

clean:
	rm -f *o out