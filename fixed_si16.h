#ifndef UNTITLED_FIXED_SI16_H
#define UNTITLED_FIXED_SI16_H

#include <stdint.h>
#include <math.h>
#include <stdio.h>

#define SI16_MAX 32767
#define SI16_MIN (-32767)
#define SI32_MAX 2147483647
#define SI32_MIN (-2147483647)

typedef int8_t si8;
typedef int16_t si16;
typedef int32_t si32;

void fl_si16_ds(si16 *s, si8 *scale, float a);
void fl_si16_es(si16* s, si8 scale, float a);
float si32_fl(si32 s, si8 scale);
si16 flt_Q_si16(float a, si8 scale);
float si16_fl(si16 a, si8 scale);

si16 si16_add_es(si16 a, si16 b);
si16 si16_sub_es(si16 a, si16 b);
si16 si16_add_ds(si16 a, uint8_t sa, si16 b, uint8_t sb);
si16 si16_sub_ds(si16 a, uint8_t sa, si16 b, uint8_t sb);

si16 si16_mul_es(si16 a, si16 b, uint8_t s);
si16 si16_mul_ds(si16 a, uint8_t sa, si16 b, uint8_t sb);
si16 si16_mul_rs(si16 a, uint8_t sa, si16 b, uint8_t sb, uint8_t sr);
si16 si16_div_es(si16 a, si16 b, uint8_t s);
si16 si16_div_ds(si16 a, uint8_t sa, si16 b, uint8_t sb);

void si32_si16_ds(si16 *b16, si8 *b_scale, si32 a32);


#endif //UNTITLED_FIXED_H
