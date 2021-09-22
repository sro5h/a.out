#ifndef COMMON_DUMMY_H
#define COMMON_DUMMY_H

#if   defined(COMMON_EXPORTS)
#       define COMMON_API __attribute__((visibility("default")))
#elif defined(COMMON_IMPORTS)
#       define COMMON_API __attribute__((visibility("default")))
#else
#       define COMMON_API
#endif

typedef enum aout_dummy_res_t {
        AOUT_DUMMY_OK,
        AOUT_DUMMY_ERR,
} aout_dummy_res_t;

aout_dummy_res_t COMMON_API aout_dummy(void);

#endif
