#ifndef UNTITLED_MAT_SI16_H
#define UNTITLED_MAT_SI16_H

#include <stdio.h>
#include <string.h>
#include "fixed_si16.h"
#include "project.h"


void mat_cmp_fl_si16e(float *cmp, const float *mat_fl, const si16 *mat_si16, si8 scale, size_n rows, size_n cols);
void mat_cmp_fl_si16d(float* cmp, const float* mat_fl, const si16* mat_si16, const si8* scale, size_n rows, size_n cols);
void arr_cmp_fl_si16e(float* cmp, const float* arr_fl, const si16* arr_si16, si8 scale, size_n2 n);
void arr_cmp_fl_si16d(float* cmp, const float* arr_fl, const si16* arr_si16, const si8* scale, size_n2 n);

void mat_print_si16e(const si16 *mat, si8 scale, size_n n_row, size_n n_col);
void mat_print_si32e(const si32*mat, si8 scale, size_n n_row, size_n n_col);
void mat_print_series_si16e(const si16 *mat, si8 scale, size_n n_row, size_n n_col, size_n n);
void mat_print_upp_si16d(const si16 *mat, const si8 *scale, size_n n_rows, size_n n_cols);
void mat_print_upp_series_si16d(const si16 *mat, const si8 *scale, size_n n_rows, size_n n_cols, size_n n);

void mat_fl_si16e(si16 *mat_si16, size_n scale, const float *mat_fl, size_n n_row, size_n n_col);

void arr_sub_si16e(si16 *c, const si16 *a, const si16 *b, size_n n);
void arr_add_self_si16e(si16 *a, const si16 *b, size_n size);
void arr_add_self_si32e(si32 *a, const si32 *b, size_n size);
size_n arr_argmax_si16(const si16 *a, size_n n);
size_n arr_argmin_si16(const si16 *a, size_n n);
size_n arr_argmin_si32(const si32 *a, size_n n);

void arr_trnsf_upp_si16d(si16* y, si8* sy, const si16 *x, si8 sx, const si16 *trans_mat, const si8* stm, size_n n_row);


void arr_print_si16d(const si16 *a, const si8 *sa, size_n n);
void arr_print_si16e(const si16 *a, si8 sa, size_n n);
void arr_print_si32e(const si32 *a, si8 sa, size_n n);

si32 arr_mult_scal_si16de_si32d(si8 *sr, const si16 *a, const si8 *sa, const si16 *b, si8 sb, size_n n_col);
si32 arr_mult_scal_si16de_si32e(si8 sr, const si16 *a, const si8 *sa, const si16 *b, si8 sb, size_n n_col);
si16 arr_mult_scal_si16de_si16e(si8 sr, const si16 *a, const si8 *sa, const si16 *b, si8 sb, size_n n_col);

void arr_fl_si16d(si16* arr_si16, si8* scale, const float* arr_fl, size_n2 n);
void arr_fl_si16e(si16* arr_si16, si8 scale, const float* arr_fl, size_n2 n);

#endif //UNTITLED_MAT_SI16_H
