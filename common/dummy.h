#ifndef COMMON_DUMMY_H
#define COMMON_DUMMY_H

#include "result.h"

typedef enum aout_dummy_res {
        AOUT_DUMMY_OK,
        AOUT_DUMMY_ERR,
} aout_dummy_res;

aout_res COMMON_API aout_dummy(bool test);

#endif
