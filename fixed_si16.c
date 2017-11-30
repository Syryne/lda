#include "fixed_si16.h"

const uint8_t si32_mask_cnt = 8;
const si32 si32_mask[]  = {16, \
                            256, \
                            4096, \
                            32768, \
                            524288, \
                            8388608, \
                            134217728, \
                            2147483647};

const uint8_t si32_mask_bit[] = {4, \
                            8, \
                            12, \
                            15, \
                            19, \
                            23, \
                            27, \
                            31};

const uint8_t si16_mask_cnt = 4;
const si16 si16_mask[] = {16384, 4096, 256, 16};
const uint8_t si16_mask_bit[] = {14, 12, 8, 4};

si16 flt_Q_si16(const float a, si8 scale) {

    int8_t i = 0;
    float b = a;
    for (; i < scale; ++i)
        b *= 2;
    return (int16_t) roundf(b);
}

void fl_si16_fs(si16* s, const float a, const si8 scale) {

    int8_t i = 0;
    float b = a;
    for (; i < scale; ++i)
        b *= 2;
    *s = (int16_t) roundf(b);
}

void fl_si16_ds(si16 *s, si8 *scale, float a) {

    si8 i = 1;
    if (fabsf(a) > si16_mask[0])
        *scale = 0;
    else if (fabsf(a) < si16_mask[si16_mask_cnt-1])
        *scale = 15 - si16_mask_bit[si16_mask_cnt-1];
    else {

        for (; i < si16_mask_cnt; ++i) {

            if (fabsf(a) > si16_mask[i]) {

                *scale = 15 - si16_mask_bit[i-1];
                i = si16_mask_cnt;

            }

        }

    }


    float b = a;
    for (i = 0; i < *scale; ++i)
        b *= 2;
    *s = (int16_t) roundf(b);

}


/// \brief converts float to si16 using predefined scale
/// \param s
/// \param scale
/// \param a
void fl_si16_es(si16* s, const si8 scale, const float a) {

    si8 i = 0;
    float b = a;
    for (; i < scale; ++i)
        b *= 2;
    *s = (int16_t) roundf(b);

}

float si16_fl(si16 s, si8 scale) {

    float b = (float) s;
    int8_t i = 0;
    for (; i < scale; ++i)
        b /= 2;
    return b;

}
float si32_fl(si32 s, si8 scale) {

    float b = (float) s;
    int8_t i = 0;
    for (; i < scale; ++i)
        b /= 2;
    return b;

}


void si32_si16_ds(si16 *b16, si8 *b_scale, const si32 a32) {

    uint8_t m = 0;
    uint8_t fl = 0;

    if (a32 < 0)
        fl = 1;

    //TODO 15 softcoding
    for (; m < si32_mask_cnt; ++m) {

        if ((fl ? -a32 : a32) < si32_mask[m]) {

            if (si32_mask_bit[m] < 15) {
                *b16 = (si16) (a32 << (15 - si32_mask_bit[m]));
                *b_scale += 15 - si32_mask_bit[m];
                return;
            } else if (si32_mask_bit[m] > 15) {
                *b16 = (si16) (a32 >> (si32_mask_bit[m] - 15));
                *b_scale -= si32_mask_bit[m] - 15;
                return;
            } else {
                *b16 = (si16) a32;
                return;
            }

        }

    }

}

void si32_si16_es(si16 *b16, const si8 b_scale, const si32 a32) {



}


si16 si16_add_es(const si16 a, const si16 b) {

    if (a > -1 && b > -1) {

        si16 temp = a + b;
        if (temp < 0)
            return SI16_MAX;
        else
            return temp;

    } else if (a < 0 && b < 0) {

        si16 temp = a + b;
        if (temp > 0)
            return SI16_MIN;
        else
            return temp;

    } else {

        return a + b;

    }

}

si16 si16_sub_es(const si16 a, const si16 b) {

    return si16_add_es(a, -b);

}

si16 si16_add_ds(const si16 a, const uint8_t sa, const si16 b, const uint8_t sb) {

    if (sa > sb)
        return si16_add_es(a >> (sa-sb), b);
    else
        return si16_add_es(a, b >> (sb-sa));

}

si16 si16_sub_ds(const si16 a, const uint8_t sa, const si16 b, const uint8_t sb) {

    return si16_add_ds(a, sa, -b, sb);

}


si16 si16_mul_es(const si16 a, const si16 b, const uint8_t s) {

    si32 temp = a * b;
    temp >>= s;
    if (temp > SI16_MAX)
        return SI16_MAX;
    else if (temp < SI16_MIN)
        return SI16_MIN;
    else
        return (si16) temp;

}

si16 si16_mul_ds(const si16 a, const uint8_t sa, const si16 b, const uint8_t sb) {
    //TODO scale!
    si32 temp = a * b;
    temp >>= sa<sb ? sa : sb;
    if (temp > SI16_MAX)
        return SI16_MAX;
    else if (temp < SI16_MIN)
        return SI16_MIN;
    else
        return (si16) temp;

}

si16 si16_mul_rs(const si16 a, const uint8_t sa, const si16 b, const uint8_t sb, uint8_t sr) {
    //TODO scale!
    si32 temp = a * b;
    //temp >>= (sr);
    if (temp > SI16_MAX) {
        printf("si16_mul_rs > max_i16\n");
        return SI16_MAX;
    }
    else if (temp < SI16_MIN) {
        printf("si16_mu_rs < min_i16\n");
        return SI16_MIN;
    }
    else
        return (si16) temp;

}

si16 si16_div_es(const si16 a, const si16 b, const uint8_t s) {

    si32 temp = a << s;
    return (si16) (temp / b);

}

si16 si16_div_ds(const si16 a, const uint8_t sa, const si16 b, const uint8_t sb) {

    si32 temp = a << sb;
    return (si16) (temp / b);

}