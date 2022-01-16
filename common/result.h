#ifndef COMMON_RESULT_H
#define COMMON_RESULT_H

#include "types.h"

#define AOUT_OK       ((aout_res) { 0 })
#define AOUT_ERR      ((aout_res) { .code = AOUT_RES_ERR })
#define AOUT_RES(err) ((aout_res) { .code = err })

#define AOUT_IS_OK(res)  ((res).code == 0)
#define AOUT_IS_ERR(res) ((res).code != 0)

// TODO: Add common result enum with members
// AOUT_RES_OK
// AOUT_RES_ERR

typedef enum aout_res_common {
        AOUT_RES_OK,
        AOUT_RES_ERR,
} aout_res_common;

typedef struct aout_res {
        int32_t code;
} aout_res;

#endif
