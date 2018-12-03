#include "common.h"
#include "hash.h"

int main(){
    FILE * f = fopen("test.txt", "rb"), *out = fopen("test2.txt", "wb");
    Streebog s(f);
    uint8_t * ret = s.H();
    fwrite(ret, 1, BLOCK_SIZE, out);
    return 0;
}