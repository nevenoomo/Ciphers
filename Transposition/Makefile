CC=g++
FLAGS=-std=c++11 
DEBUGFLAGS=-g -Wall

all: route

debug: route.h
	$(CC) $(FLAGS) $(DEBUGFLAGS) route.cpp -o route.exe

route: route.h
	$(CC) $(FLAGS) route.cpp -o route.exe

clean:
	rm -rf *.o route.exe