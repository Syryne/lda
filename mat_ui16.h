#ifndef UNTITLED_MAT_UI16_H
#define UNTITLED_MAT_UI16_H

#include <stdio.h>
#include <string.h>
#include "project.h"
#include "fixed_ui16.h"

void arr_fl_ui16d(ui16 *ui, ui8 *sui, const float *fl, size_n2 n);
void arr_fl_ui16e(ui16 *ui, ui8 *sui, const float *fl, size_n2 n);
void mat_fl_ui16e(ui16 *mat_ui16, ui8* scale, const float *mat_fl, size_n n_row, size_n n_col);
void arr_print_ui16(const ui16* arr, const ui8* sarr, size_n n);
void arr_print_ui16e(const ui16* arr, ui8 sarr, size_n n);
void mat_print_upp_ui16d(const ui16 *mat, const ui8 *scale, size_n n_rows, size_n n_cols);
void mat_print_ui16e(const ui16 *mat, ui8 scale, size_n n_row, size_n n_col);
void mat_print_ui16d(const ui16 *mat, const ui8* scale, size_n n_row, size_n n_col);

void arr_sub_ui16e(ui16 *c, ui8 *sc, const ui16 *a, ui8 sa, const ui16 *b, ui8 sb, size_n n);
void arr_trnsf_upp_ui16d(ui16* y, ui8* sy, const ui16 *x, const ui8* sx, const ui16 *trans_mat, const ui8* stm, size_n n_row);
ui16 arr_mult_scal_ui16de_ui16e(ui8 *sr, const ui16 *a, const ui8 *sa, const ui16 *b, const ui8 *sb, size_n n_col);


#endif //UNTITLED_MAT_UI16_H
