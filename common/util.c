#include "util.h"
#include <stdio.h>

void aout_print_bytes(
                uint8_t* bytes, size_t size) {
        assert(bytes);

        for (size_t i = 0; i < size; ++i) {
                printf("%02x", bytes[i]);
        }
}
