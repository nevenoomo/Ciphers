HEADERS=hash.h hash_data.h common.h

all: g256sum g512sum

g256sum: main256.cpp $(HEADERS)
	g++ -g -Wall -std=c++11 main256.cpp -o g256sum

g512sum: main512.cpp $(HEADERS)
	g++ -g -Wall -std=c++11 main512.cpp -o g512sum

clean:
	rm g256sum g512sum  