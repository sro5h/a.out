#include "types.h"

#include <math.h>

// TODO: THIS SHOULD BE CORRECTED
// https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
static const float32_t aout_f32_epsilon = 1.0e-5;

bool aout_f32_eql(
                float32_t a,
                float32_t b) {
        return fabs(a - b) < aout_f32_epsilon;
}
