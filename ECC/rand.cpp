#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
#define EXTERN_C extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C
#define EXTERN_C_END
#endif

EXTERN_C
static union {
    uint64_t v;
    uint8_t a[8];
} st;
static int pos = 0;

static uint64_t next_value(uint64_t val) { return val + 1; }

int rand_bytes(uint8_t *buf, int num) {
    while (num) {
        int l = 8 - pos;
        if (l > num)
            l = num;

        memcpy(buf, &st.a[pos], l);

        buf += l;
        num -= l;
        pos += l;

        if (pos == 8) {
            st.v = next_value(st.v);
            pos = 0;
        }
    }
    return 1;
}
EXTERN_C_END
