#include "mat_ui16.h"

void arr_fl_ui16d(ui16 *ui, ui8 *sui, const float *fl, const size_n2 n) {

    size_n2 i = 0;
    for (; i < n; ++i)
        ui[i] = fl_ui16_ds(&sui[i], fl[i]);

}

void arr_fl_ui16e(ui16 *ui, ui8 *sui, const float *fl, const size_n2 n) {

    size_n2 i = 0;
    for (; i < n; ++i)
        ui[i] = fl_ui16_fs(fl[i], &sui[i]);

}

void mat_fl_ui16e(ui16 *mat_ui16, ui8* scale, const float *mat_fl, const size_n n_row, const size_n n_col) {

    const float* it_fl = mat_fl;
    ui16* it_ui = mat_ui16;
    ui8* it_sc = scale;
    size_n2 i = 0;
    size_n2 ii = n_row*n_col;

    for (; i < ii; ++i) {

        *(it_ui++) = fl_ui16_fs(*it_fl, it_sc);
        ++it_fl;
        ++it_sc;

    }

}

void arr_print_ui16(const ui16* arr, const ui8* sarr, const size_n n) {

    size_n i = 0;
    for (; i < n; ++i)
        printf("%f\t", ui16_fl(arr[i], sarr[i]));
    printf("\n");

}
void arr_print_ui16e(const ui16* arr, const ui8 sarr, const size_n n) {

    size_n i = 0;
    for (; i < n; ++i)
        printf("%f\t", ui16_fl(arr[i], sarr));
    printf("\n");

}



void mat_print_upp_ui16d(const ui16 *mat, const ui8 *scale, const size_n n_rows, const size_n n_cols) {

    const ui16* it_col;
    const ui8* it_sc;
    size_n rws = 0;
    size_n cls;
    size_n jmp;

    printf("\n");

    for (; rws < n_rows; ++rws) {

        it_col = mat + rws;
        it_sc = scale + rws;
        jmp  = n_rows - 1;

        for (cls = 0; cls < n_rows; ++cls) {

            printf("%f\t", ui16_fl(*it_col, *it_sc));
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

void mat_print_ui16e(const ui16 *mat, const ui8 scale, const size_n n_row, const size_n n_col) {

    const ui16* it_col = mat;
    const ui16* it_last_col = mat + n_col;
    const ui16* it_end = mat + n_row * n_col;

    while (it_col != it_end) {

        while (it_col != it_last_col) {
            printf("%f\t", ui16_fl(*it_col, scale));
            ++it_col;
        }

        printf("\n");

        it_last_col += n_col;

    }

    printf("\n");

}

void mat_print_ui16d(const ui16 *mat, const ui8* scale, const size_n n_row, const size_n n_col) {

    const ui16* it_col = mat;
    const ui8* it_sc = scale;
    const ui16* it_last_col = mat + n_col;
    const ui16* it_end = mat + n_row * n_col;

    while (it_col != it_end) {

        while (it_col != it_last_col) {
            printf("%f\t", ui16_fl(*it_col, *it_sc));
            ++it_col;
            ++it_sc;
        }

        printf("\n");

        it_last_col += n_col;

    }

    printf("\n");

}

void arr_sub_ui16d(ui16 *c, ui8 *sc, const ui16 *a, const ui8 *sa, const ui16 *b, const ui8 *sb, const size_n n) {


    size_n i = 0;
    for (; i < n; ++i)
        c[i] = ui16_sub_es(&sc[i], a[i], sa[i], b[i], sb[i]);

}

void arr_sub_ui16e(ui16 *c, ui8 *sc, const ui16 *a, const ui8 sa, const ui16 *b, const ui8 sb, const size_n n) {


    size_n i = 0;
    for (; i < n; ++i)
        c[i] = ui16_sub_es(&sc[i], a[i], sa, b[i], sb);

}

void arr_trnsf_upp_ui16d(ui16* y, ui8* sy, const ui16 *x, const ui8* sx, const ui16 *trans_mat, const ui8* stm, const size_n n_row) {

    ui16* it_y = y;
    ui8* it_sy = sy;
    const ui16* it_x;
    const ui8* it_sx;
    const ui16* it_trans;
    const ui8* it_stm;

    size_n jmp;
    ui8 min;
    ui32 sum;
    ui32 temp;
    size_n i=0, j;
    size_n tmp;

    for (; i < n_row; ++i) {

        sum = 0;
        *it_sy = 0;
        it_x = x;
        it_sx = sx;
        it_trans = trans_mat + i;
        it_stm = stm + i;
        jmp = n_row - 1;

        min = 15;
        tmp = 0;
        for (; tmp < n_row; ++tmp) {
            if ((*it_stm & UI16_SCALE) < min)
                min = (*it_stm & UI16_SCALE);
            it_stm += (tmp < i) ? jmp-- : 1;
        }

        it_stm = stm + i;
        jmp = n_row - 1;

        for (j = 0; j < n_row; ++j) {

            if ((*it_sx&UI16_SIGN) == (*it_stm&UI16_SIGN)) {

                //product is always positive
                sum += (ui32) (*it_x * *it_trans) >> ((*it_stm&UI16_SCALE) - min);

            } else {

                //product is always negative
                temp = (ui32) (*it_x * *it_trans) >> ((*it_stm&UI16_SCALE) - min);
                if (*it_sy&UI16_SIGN)
                    sum += temp;
                else {
                    if (temp > sum) {
                        sum = temp - sum;
                        *it_sy |= UI16_SIGN;
                    } else
                        sum -= temp;
                }

            }

            if (j < i) {
                it_trans += jmp;
                it_stm += jmp--;
            } else {
                ++it_trans;
                ++it_stm;
            }
            ++it_x;
            ++it_sx;

        }

        *it_sy += min + *sx;
        ui32_ui16_ds(it_y, it_sy, sum);

        ++it_y;
        ++it_sy;

    }

}

ui16 arr_mult_scal_ui16de_ui16e(ui8 *sr, const ui16 *a, const ui8 *sa, const ui16 *b, const ui8* sb, const size_n n_col) {

    const ui16* it_a = a;
    const ui8* it_sa = sa;
    const ui16* it_b = b;
    const ui8* it_sb = sb;
    size_n i = 0;
    ui16 res = 0;
    ui32 tmp;

    //TODO set res to ui32 and cast to ui16 at the end

    for (; i < n_col; ++i) {

        tmp = (ui32) *(it_a++) * *(it_b++);

        if ((*it_sa&UI16_SIGN) == (*it_sb&UI16_SIGN)) {

            //tmp is always positive
            if (*it_sa+*it_sb > *sr)
                tmp >>= ((*it_sa&UI16_SCALE) + (*it_sb&UI16_SCALE) - *sr);
            else
                tmp <<= (*sr&UI16_SCALE - (*it_sa&UI16_SCALE) + (*it_sb&UI16_SCALE));

            tmp = ((tmp > UI16_MAX) ? UI16_MAX : tmp);

            //if res is negative
            if (*sr&UI16_SIGN) {
                if (res > tmp)
                    res -= (ui16) tmp;
                else {
                    res = (ui16) tmp - res;
                    *sr ^= UI16_SIGN;
                }
            } else         //res is positive
                res += tmp;

        } else {

            //tmp is always negative
            if (*it_sa+*it_sb > *sr)
                tmp >>= ((*it_sa&UI16_SCALE) + (*it_sb&UI16_SCALE) - *sr);
            else
                tmp <<= (*sr - (*it_sa&UI16_SCALE) + (*it_sb&UI16_SCALE));

            tmp = ((tmp > UI16_MAX) ? UI16_MAX : tmp);

            //if res is negative
            if (*sr&UI16_SIGN)
                res += (ui16) tmp;
            else {
                if (res > tmp)
                    res -= tmp;
                else {
                    res = (ui16) tmp - res;
                    *sr ^= UI16_SIGN;
                }
            }

        }

        ++it_sa;
        ++it_sb;

    }

    return res;

}