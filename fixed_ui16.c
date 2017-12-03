#include "fixed_ui16.h"
#include <math.h>

const uint8_t ui32_mask_cnt = 7;
const ui32 ui32_mask[]  = {0x0000000F, \
                            0x000000FF, \
                            0x00000FFF, \
                            0x0000FFFF, \
                            0x000FFFFF, \
                            0x00FFFFFF, \
                            0x0FFFFFFF};
const uint8_t ui32_mask_bit[] = {4, \
                            8, \
                            12, \
                            16, \
                            20, \
                            24, \
                            28};

const uint8_t ui16_mask_cnt = 4;
const ui32 ui16_mask[] = {0x3FFF, 0x0FFF, 0x00FF, 0x000F};
const uint8_t ui16_mask_bit[] = {14, 12, 8, 4};


void ui32_ui16_ds(ui16 *b16, ui8 *b_scale, const ui32 a32) {

    uint8_t m = 0;

    //TODO 15 softcoding
    for (; m < ui32_mask_cnt; ++m) {

        if (a32 < ui32_mask[m]) {

            if (ui32_mask_bit[m] < 15) {
                *b16 = (ui16) (a32 << (15 - ui32_mask_bit[m]));
                *b_scale += 15 - ui32_mask_bit[m];
                return;
            } else if (ui32_mask_bit[m] > 15) {
                *b16 = (ui16) (a32 >> (ui32_mask_bit[m] - 15));
                *b_scale -= ui32_mask_bit[m] - 15;
                return;
            } else {
                *b16 = (ui16) a32;
                return;
            }

        }

    }

}


ui16 fl_ui16_fs(const float a, ui8* scale) {

    uint8_t i = 0;
    float b = fabsf(a);
    for (; i < *scale; ++i)
        b *= 2;
    if (a < 0)
        *scale |= UI16_SIGN;
    return (ui16) roundf(b);

}

ui16 fl_ui16_ds(ui8 *scale, float a) {

    ui8 i = 1;
    uint8_t sign = (a<0) ? 1 : 0;
    float b = fabsf(a);
    if (b > ui16_mask[0])
        *scale = 0;
    else if (b < ui16_mask[ui16_mask_cnt-1])
        *scale = 15 - ui16_mask_bit[ui16_mask_cnt-1];
    else {

        for (; i < ui16_mask_cnt; ++i) {

            if (b > ui16_mask[i]) {

                *scale = 15 - ui16_mask_bit[i-1];
                i = ui16_mask_cnt;

            }

        }

    }

    if (a < 0)
        *scale |= UI16_SIGN;

    for (i = 0; i < (*scale&UI16_SCALE); ++i)
        b *= 2;
    return (ui16) roundf(b);

}


float ui16_fl(const ui16 a, const ui8 scale) {

    float b = (float) a;
    uint8_t i = 0;
    uint8_t sign = scale & UI16_SIGN;
    uint8_t sc = scale & UI16_SCALE;
    for (; i < sc; ++i)
        b /= 2;
    return (sign ? -b : b);

}


ui16 ui16_add_es(ui8* sc, const ui16 a, const ui8 sa, const ui16 b, const ui8 sb) {

    ui16 res;

    if (a > b) {

        if ((sa & UI16_SIGN) == (sb & UI16_SIGN)) {

            res = a + b;
            *sc = sa;
            return (res < b) ? (ui16) UI16_MAX : res;

        } else {

            res = a - b;
            *sc = sa;
            return res;

        }

    } else {

        if ((sa & UI16_SIGN) == (sb & UI16_SIGN)) {

            res = a + b;
            *sc = sb;
            return (res < a) ? (ui16) UI16_MAX : res;

        } else {

            res = b - a;
            *sc = sb;
            return res;

        }

    }

}

void ui16_add_slf_es(ui16* a, ui8* sa, const ui16 b, const ui8 sb) {

    //TODO check overflows

    if (*a > b) {

        if ((*sa & UI16_SIGN) == (sb & UI16_SIGN))
            *a += b;
        else
            *a -= b;

    } else {

        if ((*sa & UI16_SIGN) == (sb & UI16_SIGN)) {

            *a += b;
            *sa = sb;

        } else {

            *a = b - *a;
            *sa = sb;

        }

    }

}

ui16 ui16_sub_es(ui8* sc, const ui16 a, const ui8 sa, const ui16 b, const ui8 sb) {

    //flip sign bit
    return ui16_add_es(sc, a, sa, b, sb ^ UI16_SIGN);

}