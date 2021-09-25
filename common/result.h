#ifndef COMMON_RESULT_H
#define COMMON_RESULT_H

#include "types.h"

#define AOUT_OK       ((aout_res) { 0 })
#define AOUT_ERR(err) ((aout_res) { .code = err })

#define AOUT_IS_OK(res)  ((res).code == 0)
#define AOUT_IS_ERR(res) ((res).code != 0)

typedef struct aout_res {
        int32_t code;
} aout_res;

#endif
