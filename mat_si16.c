#include "mat_si16.h"


void arr_add_self_si16e(si16 *a, const si16 *b, size_n size) {

    si16* it_a = a;
    const si16* it_b = b;
    size_n i = 0;

    for (; i < size; ++i)
        *(it_a++) += *(it_b++);

}

void arr_add_self_si32e(si32 *a, const si32 *b, size_n size) {

    si32* it_a = a;
    const si32* it_b = b;
    size_n i = 0;

    for (; i < size; ++i)
        *(it_a++) += *(it_b++);

}

size_n arr_argmax_si16(const si16 *a, const size_n n) {

    size_n i = 1;
    si16 max = *a;
    size_n p = 0;
    const si16* it_a = a;

    for (; i < n; ++i) {

        if (*(++it_a) > max) {
            max = *it_a;
            p = i;
        } else
            ++it_a;

    }

    return p;

}

size_n arr_argmin_si16e(const si16 *a, const size_n n) {

    size_n i = 1;
    si16 min = *a;
    size_n p = 0;
    const si16* it_a = a;

    for (; i < n; ++i) {

        if (*(++it_a) < min) {
            min = *it_a;
            p = i;
        }

    }

    return p;

}

size_n arr_argmin_si32(const si32 *a, const size_n n) {

    size_n i = 1;
    si32 min = *a;
    size_n p = 0;
    const si32* it_a = a;

    for (; i < n; ++i) {

        if (*(++it_a) < min) {
            min = *it_a;
            p = i;
        }

    }

    return p;

}


void mat_cmp_fl_si16e(float *cmp, const float *mat_fl, const si16 *mat_si16, const si8 scale, size_n rows, size_n cols) {

    si16 it = 0;
    for (; it < rows*cols; ++it)
        cmp[it] = 1 - mat_fl[it]/ si16_fl(mat_si16[it], scale);

}

void mat_cmp_fl_si16d(float* cmp, const float* mat_fl, const si16* mat_si16, const si8* scale, const size_n rows, const size_n cols) {

    si16 it = 0;
    for (; it < rows*cols; ++it)
        cmp[it] = 1 - mat_fl[it]/ si16_fl(mat_si16[it], scale[it]);

}

void arr_cmp_fl_si16e(float* cmp, const float* arr_fl, const si16* arr_si16, const si8 scale, const size_n2 n) {

    size_n2 it = 0;
    for (; it < n; ++it)
        cmp[it] = 1 - arr_fl[it]/ si16_fl(arr_si16[it], scale);

}

void arr_cmp_fl_si16d(float* cmp, const float* arr_fl, const si16* arr_si16, const si8* scale, const size_n2 n) {

    size_n2 it = 0;
    for (; it < n; ++it)
        cmp[it] = 1 - arr_fl[it]/ si16_fl(arr_si16[it], scale[it]);

}

void mat_print_si16e(const si16 *mat, const si8 scale, const size_n n_row, const size_n n_col) {

    const si16* it_col = mat;
    const si16* it_last_col = mat + n_col;
    const si16* it_end = mat + n_row * n_col;

    while (it_col != it_end) {

        while (it_col != it_last_col) {
            printf("%f\t", si16_fl(*it_col, scale));
            ++it_col;
        }

        printf("\n");

        it_last_col += n_col;

    }

    printf("\n");

}

void mat_print_si32e(const si32 *mat, const si8 scale, const size_n n_row, const size_n n_col) {

    const si32* it_col = mat;
    const si32* it_last_col = mat + n_col;
    const si32* it_end = mat + n_row * n_col;

    while (it_col != it_end) {

        while (it_col != it_last_col) {
            printf("%f\t", si32_fl(*it_col, scale));
            ++it_col;
        }

        printf("\n");

        it_last_col += n_col;

    }

    printf("\n");

}

void mat_print_series_si16e(const si16 *mat, const si8 scale, const size_n n_row, const size_n n_col, const size_n n) {

    size_n i = 0;
    const si16* it_mat = mat;
    for (; i < n; ++i) {
        mat_print_si16e(it_mat, scale, n_row, n_col);
        it_mat += n_row*n_col;
    }

}

void mat_fl_si16e(si16 *mat_si16, size_n scale, const float *mat_fl, size_n n_row, size_n n_col) {

    const float* it_fl = mat_fl;
    si16* it_si = mat_si16;
    size_n2 i = 0;
    size_n2 ii = n_row*n_col;

    for (; i < ii; ++i) {

        *(it_si++) = flt_Q_si16(*it_fl, scale);
        ++it_fl;

    }

}

void arr_sub_si16e(si16 *c, const si16 *a, const si16 *b, size_n n) {

    size_n i = 0;
    for (; i < n; ++i)
        c[i] = si16_sub_es(a[i], b[i]);

}


void arr_trnsf_upp_si16d(si16* y, si8* sy, const si16 *x, const si8 sx, const si16 *trans_mat, const si8* stm, const size_n n_row) {

    si16* it_y = y;
    si8* it_sy = sy;
    const si16* it_x;
    const si16* it_trans;
    const si8* it_stm;
    size_n jmp;
    si8 min;
    si32 sum;
    size_n i=0, j;
    size_n tmp;

    for (; i < n_row; ++i) {

        sum = 0;
        it_x = x;
        it_trans = trans_mat + i;
        it_stm = stm + i;
        jmp = n_row - 1;

        min = 15;
        tmp = 0;
        for (; tmp < n_row; ++tmp) {
            if (*it_stm < min)
                min = *it_stm;
            it_stm += (tmp < i) ? jmp-- : 1;
        }

        it_stm = stm + i;
        jmp = n_row - 1;

        for (j = 0; j < n_row; ++j) {

            sum += ((si32) *it_x * *it_trans) >> (*it_stm - min);
            if (j < i) {
                it_trans += jmp;
                it_stm += jmp--;
            } else {
                ++it_trans;
                ++it_stm;
            }
            ++it_x;

        }

        *it_sy = min + sx;
        si32_si16_ds(it_y, it_sy, sum);

        ++it_y;
        ++it_sy;

    }

}



void arr_print_si16d(const si16 *a, const si8 *sa, const size_n n) {

    size_n j = 0;
    for (; j < n; ++j) {
        printf("%f\t", si16_fl(a[j], sa[j]));
    }
    printf("\n");

}
void arr_print_si16e(const si16 *a, const si8 sa, const size_n n) {

    size_n j = 0;
    for (; j < n; ++j) {
        printf("%f\t", si16_fl(a[j], sa));
    }
    printf("\n");

}
void arr_print_si32e(const si32 *a, const si8 sa, const size_n n) {

    size_n j = 0;
    for (; j < n; ++j) {
        printf("%f\t", si32_fl(a[j], sa));
    }
    printf("\n");

}
void mat_print_upp_si16d(const si16 *mat, const si8 *scale, size_n n_rows, size_n n_cols) {

    const si16* it_col;
    const si8* it_sc;
    size_n rws = 0;
    size_n cls;
    size_n jmp;

    printf("\n");

    for (; rws < n_rows; ++rws) {

        it_col = mat + rws;
        it_sc = scale + rws;
        jmp  = n_rows - 1;

        for (cls = 0; cls < n_rows; ++cls) {

            printf("%f\t", si16_fl(*it_col, *it_sc));
            if (cls < rws) {
                it_col += jmp;
                it_sc += jmp--;
            } else {
                ++it_col;
                ++it_sc;
            }

        }

        printf("\n");

    }

}
void mat_print_upp_series_si16d(const si16 *mat, const si8 *scale, size_n n_rows, size_n n_cols, size_n n) {

    const si16* it_mat = mat;
    const si8* it_sc = scale;
    size_n i = 0;
    size_n2 jmp = n_rows*(n_rows+1)/2;

    for (; i < n; ++i) {
        mat_print_upp_si16d(it_mat, it_sc, n_rows, n_rows);
        it_mat += jmp;
        it_sc += jmp;
    }

}
void mat_print_upp_si16e(const si16 *mat, const si8 scale, const size_n n_rows, const size_n n_cols) {

    const si16* it_col;
    size_n rws = 0;
    size_n cls;
    size_n jmp;

    printf("\n");

    for (; rws < n_rows; ++rws) {

        it_col = mat + rws;
        jmp  = n_cols - 1;

        for (cls = 0; cls < n_cols; ++cls) {

            printf("%f\t", si16_fl(*it_col, scale));
            it_col += (cls < rws) ? jmp-- : 1;

        }

        printf("\n");

    }

}

si32 arr_mult_scal_si16de_si32d(si8 *sr, const si16 *a, const si8 *sa, const si16 *b, si8 sb, size_n n_col) {

    const si16* it_a = a;
    const si16* it_b = b;
    size_n i = 0;
    si32 res = 0;

    //TODO macro
    si8 s = 11;

    //find lowest scale
    for (; i < n_col; ++i) {

        if (sa[i] < s)
            s = sa[i];

    }

    for (i = 0; i < n_col; ++i)
        res += ((si32) *(it_a++) * *(it_b++)) >> (sa[i] - s);

    *sr = s;
    return res;
}

si32 arr_mult_scal_si16de_si32e(const si8 sr, const si16 *a, const si8 *sa, const si16 *b, const si8 sb, const size_n n_col) {

    const si16* it_a = a;
    const si16* it_b = b;
    size_n i = 0;
    si32 res = 0;
    si32 tmp;

    for (i = 0; i < n_col; ++i) {
        tmp = (si32) *(it_a++) * *(it_b++);
        if (sa[i]+sb > sr)
            res += tmp >> (sa[i]+sb - sr);
        else
            res += tmp << (sr - sa[i]+sb);
    }

    return res;

}

si16 arr_mult_scal_si16de_si16e(const si8 sr, const si16 *a, const si8 *sa, const si16 *b, const si8 sb, const size_n n_col) {

    const si16* it_a = a;
    const si8* it_sa = sa;
    const si16* it_b = b;
    size_n i = 0;
    si16 res = 0;
    si32 tmp;

    for (; i < n_col; ++i) {

        tmp = (si32) *(it_a++) * *(it_b++);

        if (sa[i]+sb > sr) {
            tmp >>= (*(it_sa++)+sb - sr);
            res += (si16) ((tmp > SI16_MAX) ? SI16_MAX : (tmp < SI16_MIN) ? SI16_MIN : tmp);
        }

        else {
            tmp <<= (sr - *(it_sa++)+sb);
            res += (si16) ((tmp > SI16_MAX) ? SI16_MAX : (tmp < SI16_MIN) ? SI16_MIN : tmp);
        }

    }

    return res;

}

void arr_fl_si16d(si16* arr_si16, si8* scale, const float* arr_fl, const size_n2 n) {

    size_n2 i = 0;

    for (; i < n; ++i)
        fl_si16_ds(&arr_si16[i], &scale[i], arr_fl[i]);

}

void arr_fl_si16e(si16* arr_si16, const si8 scale, const float* arr_fl, const size_n2 n) {

    size_n2 i = 0;

    for (; i < n; ++i)
        fl_si16_es(&arr_si16[i], scale, arr_fl[i]);

}