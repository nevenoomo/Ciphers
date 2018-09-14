CC=g++
FLAGS=-std=c++11 -Wall
DEBUGFLAGS=-g

all: polybius

debug: polybius.h
	$(CC) $(FLAGS) $(DEBUGFLAGS) polybius.cpp -o polybius.exe

polybius: polybius.h
	$(CC) $(FLAGS) polybius.cpp -o polybius.exe

clean:
	rm -rf *.o polybius.exe