#ifndef UNTITLED_FIXED_UI16_H
#define UNTITLED_FIXED_UI16_H

#include <stdint.h>

#define UI16_MAX 0xFFFF
#define UI16_MIN 0x0000
#define UI16_SIGN 0b10000000
#define UI16_SCALE 0b01111111
#define UI16_MID 0x00FF

typedef uint8_t ui8;
typedef uint16_t ui16;
typedef uint32_t ui32;

void ui32_ui16_ds(ui16 *b16, ui8 *b_scale, ui32 a32);

ui16 fl_ui16_fs(float a, ui8* scale);
ui16 fl_ui16_ds(ui8 *scale, float a);
float ui16_fl(ui16 a, ui8 scale);

ui16 ui16_add_es(ui8* sc, ui16 a, ui8 sa, ui16 b, ui8 sb);
ui16 ui16_sub_es(ui8* sc, ui16 a, ui8 sa, ui16 b, ui8 sb);
ui16 ui16_add_ds(ui16 a, uint8_t sa, ui16 b, uint8_t sb);
ui16 ui16_sub_ds(ui16 a, uint8_t sa, ui16 b, uint8_t sb);

ui16 ui16_mul_es(ui16 a, ui16 b, uint8_t s);
ui16 ui16_mul_ds(ui16 a, uint8_t sa, ui16 b, uint8_t sb);
ui16 ui16_div_es(ui16 a, ui16 b, uint8_t s);
ui16 ui16_div_ds(ui16 a, uint8_t sa, ui16 b, uint8_t sb);

#endif //UNTITLED_FIXED_UI16_H
