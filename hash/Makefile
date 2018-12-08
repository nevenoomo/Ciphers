HEADERS=hash.h hash_data.h coommon.h

all: g256sum g512sum

g256sum: $(HEADERS) main256.cpp
	g++ -g -Wall -std=c++11 main256.cpp -o g256sum

g512sum: $(HEADERS) main256.cpp
	g++ -g -Wall -std=c++11 main512.cpp -o g512sum

clean:
	rm g256sum g512sum  