#include "modes.h"

int main(int argc, char const *argv[])
{
    uint32_t K[8] = {
        0xffeeddcc,
        0xbbaa9988,
        0x77665544,
        0x33221100,
        0xf0f1f2f3,
        0xf4f5f6f7,
        0xf8f9fafb,
        0xfcfdfeff
    };
    ifstream i("input.txt", ios::binary);
    ofstream o("output.txt", ios::binary);
    ECB m(K, i, o);
    m.encrypt();

    return 0;
}