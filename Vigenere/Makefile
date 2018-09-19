CC=g++
FLAGS=-std=c++11 -Wall
DEBUGFLAGS=-g

all: vigenere

debug: vigenere.h
	$(CC) $(FLAGS) $(DEBUGFLAGS) vigenere.cpp -o vigenere.exe

vigenere: vigenere.h
	$(CC) $(FLAGS) vigenere.cpp -o vigenere.exe

clean:
	rm -rf *.o vigenere.exe