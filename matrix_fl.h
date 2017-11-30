#ifndef UNTITLED_MATRIX_FL_H
#define UNTITLED_MATRIX_FL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "project.h"


void arr_read_fl(char *buf, float *arr, size_n l);
void arr_print_fl(float *arr, size_n l);
void arr_add_self_fl(float *a, const float *b, size_n size);
void arr_div_self_scal_fl(float *a, int16_t scal, size_n n);

size_n arr_argmin_fl(const float *a, size_n n);
size_n arr_argmax_fl(const float *a, size_n n);



void mat_read_fl(float *mat, const char *filename, size_n n_row, size_n n_col);
void mat_print_fl(const float *mat, size_n n_row, size_n n_col);
void mat_read_series_fl(char *filename, float *mat, size_n n_row, size_n n_col, size_n n);
void mat_print_series_fl(const float *mat, size_n n_row, size_n n_col, size_n n);

void arr_sub_fl(float *c, const float *a, const float *b, size_n n);


void arr_transform_fl(const float *x, const float *trans_mat, size_n n_row, float *y);
void mat_symm_upp_fl(float *mat_up, const float *mat_sym, size_n n_rows, size_n n_cols);
void mat_print_upp_fl(const float *mat, size_n n_rows, size_n n_cols);

void arr_trnsf_upp_fl(float *y, const float *x, const float *trans_mat, size_n n_row);
float arr_mult_scal_fl(const float* a, const float* b, size_n n);

#endif //UNTITLED_MATRIX_FL_H
