#include <stdio.h>
#include <stdint.h>

union {
    float f;
    uint32_t d; // Assumes float is 32 bits wide
} u;


int main(void) {
    u.d = 3.14f;
    printf("Hexadecimal representation of 3.14f: %x\n", u.d);
    printf("u.f: %f\n", u.f);
    printf("u.d: %d\n", u.d);
}
