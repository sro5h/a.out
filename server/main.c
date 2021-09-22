#include <stdio.h>
#include <common/dummy.h>

int main(void) {
        if (aout_dummy() == AOUT_DUMMY_OK) {
                printf("server\n");
        }

        return 0;
}
