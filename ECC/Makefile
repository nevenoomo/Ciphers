CC=g++
CFLAGS=$(CFLAGS) -g -std=c++11 -Wall
KEYGEN_SOURCE=key_generation.cpp
KEYGEN_HEADERS=key_generation.h
ECC_SOURCE=sign.cpp
ECC_HEADER=ECC.h
COMMON_HEADERS=common.h BigInteger.h
RAND_SOURCE=rand.cpp
RAND_HEADER=rand.h
HASH_HEADER=hash.h

all: $(KEYGEN_SOURCE) $(ECC_SOURCE) $(RAND_SOURCE) key_gen signature

key_gen: $(KEYGEN_HEADERS) $(KEYGEN_SOURCE) $(RAND_SOURCE) $(RAND_HEADER)
	$(CC) $(CFLAGS) -o genpkey -D_GEN_PRIV_ $(KEYGEN_SOURCE) $(RAND_SOURCE); $(CC) $(CFLAGS) -o convpkey $(KEYGEN_SOURCE) $(RAND_SOURCE)

signature: $(ECC_SOURCE) $(ECC_HEADER) $(RAND_SOURCE) $(RAND_HEADER) $(HASH_HEADER)
	$(CC) $(CFLAGS) -o sign -D__SIGN__ $(ECC_SOURCE) $(RAND_SOURCE); $(CC) $(CFLAGS) -o verify $(ECC_SOURCE) $(RAND_SOURCE)