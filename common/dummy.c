#include "dummy.h"

aout_res aout_dummy(bool test) {
        if (!test) {
                return AOUT_RES(AOUT_DUMMY_ERR);
        }

        return AOUT_OK;
}
