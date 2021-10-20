CC=g++-10

CFLAGS=-c -fopenmp
LNKFLAGS=-fopenmp 

all: Q3_1 Q3_2

Q3_1: Q3_1.o 
	$(CC) $(LNKFLAGS) Q3_1.o -o Q3_1

Q3_1.o: question3_1.cpp
	$(CC) $(CFLAGS) question3_1.cpp -o Q3_1.o

Q3_2: Q3_2.o 
	$(CC) $(LNKFLAGS) Q3_2.o -o Q3_2

Q3_2.o: question3_2.cpp
	$(CC) $(CFLAGS) question3_2.cpp -o Q3_2.o

clean:
	rm -f *o main