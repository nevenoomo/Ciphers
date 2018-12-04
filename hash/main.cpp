#include "common.h"
#include "hash.h"

int main(){
    FILE * f = fopen("example2", "rb"), *out = fopen("output.txt", "wb");
    Streebog s(f, true);
    uint8_t * ret = s.H();
    fwrite(ret, 1, BLOCK_SIZE, out);
    return 0;
}