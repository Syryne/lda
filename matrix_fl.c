#include "matrix_fl.h"


size_n arr_argmax_fl(const float *a, const size_n n) {

    size_n i = 1;
    float max = *a;
    size_n p = 0;
    const float* it_a = a+1;

    for (; i < n; ++i) {

        if (*it_a > max) {
            max = *(it_a++);
            p = i;
        } else
            ++it_a;

    }

    return p;

}

size_n arr_argmin_fl(const float *a, const size_n n) {

    size_n i = 1;
    float min = *a;
    size_n p = 0;
    const float* it_a = a+1;

    for (; i < n; ++i) {

        if (*it_a < min) {
            min = *(it_a++);
            p = i;
        } else
            ++it_a;

    }

    return p;

}

void mat_read_fl(float *mat, const char *filename, size_n n_row, size_n n_col) {

    FILE *csv_file = fopen(filename, "r");

    if (!csv_file) {

        printf("\nfile opening failed:\t%s\n", filename);
        return;

    }

    char buf[10000];
    char *token;
    float* it_mat = mat;

    int rw=0, cl;

    for (; rw < n_row; ++rw) {

        fgets(buf, sizeof buf, csv_file);
        token = strtok(buf, ",");
        *(it_mat++) = strtof(token, NULL);

        for (cl = 1; cl < n_col; ++cl) {

            token = strtok(NULL, ",");
            *(it_mat++) = strtof(token, NULL);

        }

    }

    fclose(csv_file);

}


void mat_read_series_fl(char *filename, float *mat, size_n n_row, size_n n_col, size_n n) {

    FILE *csv_file = fopen(filename, "r");

    if (!csv_file) {

        printf("\nfile opening failed:\t%s\n", filename);
        return;

    }

    char buf[10000];
    char *token;
    float* it_mat = mat;

    int i = 0, rw, cl;

    for (; i < n; ++i) {

        for (rw = 0; rw < n_row; ++rw) {

            fgets(buf, sizeof buf, csv_file);
            token = strtok(buf, ",");
            *(it_mat++) = strtof(token, NULL);

            for (cl = 1; cl < n_col; ++cl) {

                token = strtok(NULL, ",");
                *(it_mat++) = strtof(token, NULL);

            }

        }

    }

    fclose(csv_file);

}


//prints matrix
void mat_print_fl(const float *mat, const size_n n_row, const size_n n_col) {

    size_n rw = 0, cl;
    const float* it_mat = mat;

    for (rw = 0; rw < n_row; ++rw) {

        for (cl = 0; cl < n_col; ++cl)
            printf("%f\t", *(it_mat++));

        printf("\n");

    }

}


void mat_print_series_fl(const float *mat, const size_n n_row, const size_n n_col, const size_n n) {

    size_n i=0, rw, cl;
    const float* it_mat = mat;

    for (; i < n; ++i) {

        printf("\n");

        for (rw = 0; rw < n_row; ++rw) {

            for (cl = 0; cl < n_col; ++cl)
                printf("%f\t", *(it_mat++));

            printf("\n");

        }

    }

}


void arr_read_fl(char *buf, float *arr, size_n l) {

    float* it = arr;
    float* it_end = arr+l;

    char* token = strtok(buf, ",");
    *(it++) = strtof(token, NULL);

    while (it != it_end) {

        token = strtok(NULL, ",");
        *(it++) = strtof(token, NULL);

    }

}

void arr_print_fl(const float *arr, const size_n l) {

    size_n i = 1;
    printf("%f", arr[0]);
    for (; i<l; ++i)
        printf("\t%f", arr[i]);
    printf("\n");

}

void arr_add_self_fl(float *a, const float *b, size_n size) {

    float* it_a = a;
    const float* it_b = b;
    const float* it_end = a + size;

    while (it_a != it_end)
        *(it_a++) += *(it_b++);

}

void arr_div_self_scal_fl(float *a, int16_t scal, size_n n) {

    size_n i = 0;

    for (; i < n; ++i) {

        a[i] /= scal;

    }

}


void arr_sub_fl(float *c, const float *a, const float *b, const size_n n) {

    size_n i = 0;
    for (; i < n; ++i)
        c[i] = a[i] - b[i];
}


void arr_transform_fl(const float *x, const float *trans_mat, const size_n n_row, float *y) {

    float* it_y = y;
    const float* it_x;
    const float* it_trans;
    float sum;
    size_n i=0, j;

    for (; i < n_row; ++i) {

        sum = 0;
        it_x = x;
        it_trans = trans_mat + i;

        for (j = 0; j < n_row; ++j) {

            sum += *it_x * *it_trans;
            //printf("%f,%f\n", (float) *it_x**it_trans/4096, (float) sum/4096);
            ++it_x;
            it_trans = (j < i) ? it_trans + n_row: it_trans + 1;

        }
        //printf("\n");

        ++it_y;

    }

}


void mat_symm_upp_fl(float *mat_up, const float *mat_sym, const size_n n_rows, const size_n n_cols) {

    size_n rws = 0;
    size_n cls;
    const float* it_ci_full = mat_sym;
    float* it_ci = mat_up;
    for (; rws < n_rows; ++rws) {

        for (cls = rws; cls < n_cols; ++cls)
            *(it_ci++) = *(it_ci_full++);

        it_ci_full += rws + 1;

    }
}

void mat_print_upp_fl(const float *mat, const size_n n_rows, const size_n n_cols) {
    const float* it_col;
    size_n rws = 0;
    size_n cls;
    size_n jmp;

    printf("\n");

    for (; rws < n_rows; ++rws) {

        it_col = mat + rws;
        jmp  = n_cols - 1;

        for (cls = 0; cls < n_cols; ++cls) {

            printf("%f\t", *it_col);
            it_col += (cls < rws) ? jmp-- : 1;

        }

        printf("\n");

    }

}

void arr_trnsf_upp_fl(float *y, const float *x, const float *trans_mat, size_n n_row) {

    float* it_y = y;
    const float* it_x;
    const float* it_trans;
    size_n jmp;
    size_n i=0, j;

    for (; i < n_row; ++i) {

        *it_y = 0;
        it_x = x;
        it_trans = trans_mat + i;
        jmp = n_row - 1;

        for (j = 0; j < n_row; ++j) {

            *it_y += *it_x * *it_trans;
            it_trans += (j < i) ? jmp-- : 1;
            ++it_x;

        }

        ++it_y;

    }

}

float arr_mult_scal_fl(const float* a, const float* b, const size_n n) {

    const float* it_a = a;
    const float* it_b = b;
    size_n i = 0;
    float res = 0;

    for (i; i < n; ++i)
        res += *(it_a++) * *(it_b++);

    return res;

}