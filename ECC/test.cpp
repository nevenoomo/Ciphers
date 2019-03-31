#include "BigInteger.h"
#include "common.h"
#include "params.h"
#include <cstdio>

using namespace ECP;

int main(){
    BigInteger d = setA.d;
    BigInteger b = setA.p;
    BigInteger c(2, 0x0);
    
    
    c = (d*d)/b;

    for (int i = c.size() - 1; i>=0; --i){
        printf("%02x", *(c.get_data() + i));
    }
    
    return 0;
}