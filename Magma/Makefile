CC=g++
CFLAGS=-c -std=c++11 -Wall
DEBUGFLAGS=-g
HEADERS=basic_block_cypher.cpp modes.h utils.h
SOURCES=main.cpp modes.cpp utils.cpp basic_block_cypher.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=magma.exe 

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(DEBUGFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)