#include <stdio.h>

#include "lda.h"
#include "fixed_ui16.h"
#include "mat_ui16.h"


typedef struct Histogram {

    si32 histogram[8];
    size_n n_mask;

} si32_histogram;

void histogram_add(si32_histogram *hist, const si32 value) {

    si32 t = (value > 0) ? value : -value;
    size_n i = 0;
    for (; i < hist->n_mask; ++i) {

        //if (t < si32_mask[i]) {
        //    ++hist->histogram[i];
        //    return;
        //}
    }

}

void histogram_print(const si32_histogram hist) {

    size_n i = 0;
    for (; i < hist.n_mask; ++i)
        printf("%i\t", hist.histogram[i]);
    printf("\n");

}

int main() {

    //char* dp_cov_inv = "/home/speklap/CLionProjects/untitled/database/raw_win_data/0/covar_inv_train.csv";
    //char* dp_cov_inv_sns = "/home/speklap/CLionProjects/untitled/database/raw_win_data/0/covar_inv_sens_train.csv";

    test_mahalanobis();



    return 0;

}