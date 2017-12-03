#include "lda.h"
#include "fixed_ui16.h"
#include "mat_ui16.h"

size_n predict_mahalanobis_fl(float *distance, const float *fv_fl, const float *means_fl, const float *covar_inv_fl,
                              const size_n n_feat) {

    float temp_fl[n_feat];
    float t_fl[n_feat];
    float dist_min = 50000;

    //printf("FLOAT fv\n");
    //arr_print_fl(fv_fl, n_feat);

    size_n pr = 0;
    uint8_t i = 0;
    for (; i < CLASS_COUNT; ++i) {

        arr_sub_fl(temp_fl, fv_fl, means_fl + i * n_feat, n_feat);
        arr_trnsf_upp_fl(t_fl, temp_fl, covar_inv_fl, n_feat);
        distance[i] = arr_mult_scal_fl(temp_fl, t_fl, n_feat);

        if (distance[i] < dist_min) {
            pr = i;
            dist_min = distance[i];
        }

    }

    return pr;

}

size_n predict_mahalanobis_si16(si16 *distance, const si8 dist_sc, const si16 *fv, const si8 fv_sc, const si16 *means,
                                const si16 *covar_inv, const si8 *covar_inv_sc, const size_n n_feat) {

    si16 temp[n_feat];
    si8 t_scale[n_feat];
    si16 t[n_feat];

    si16 dist_min = SI16_MAX;
    si16* it_dist = distance;
    size_n pr = 0;

    uint8_t i = 0;
    for (; i < CLASS_COUNT; ++i) {

        arr_sub_si16e(temp, fv, means + i * n_feat, n_feat);
        arr_trnsf_upp_si16d(t, t_scale, temp, fv_sc, covar_inv, covar_inv_sc, n_feat);
        *it_dist = arr_mult_scal_si16de_si16e(dist_sc, t, t_scale, temp, fv_sc, n_feat);

        //*it_dist = (*it_dist < 0) ? -*it_dist : *it_dist;
        if (*it_dist < dist_min) {
            pr = i;
            dist_min = *(it_dist++);
        } else
            ++it_dist;

    }
    printf("\n");

    return pr;

}

size_n predict_mahalanobis_si32(si32 *distance, const si8 dist_sc, const si16 *fv, const si8 fv_sc, const si16 *means,
                                const si16 *covar_inv, const si8 *covar_inv_sc, const size_n n_feat) {

    si16 temp[n_feat];
    si8 t_scale[n_feat];
    si16 t[n_feat];

    si32 dist_min = SI32_MAX;
    si32* it_dist = distance;
    size_n pr = 0;

    uint8_t i = 0;
    for (; i < CLASS_COUNT; ++i) {

        arr_sub_si16e(temp, fv, means + i * n_feat, n_feat);
        arr_trnsf_upp_si16d(t, t_scale, temp, fv_sc, covar_inv, covar_inv_sc, n_feat);

        *it_dist = arr_mult_scal_si16de_si32e(dist_sc, t, t_scale, temp, fv_sc, n_feat);
        //if (*it_dist > SI16_MAX || *it_dist < SI16_MIN) {
        //printf("%f\t%i\n", si32_fl(*it_dist, dist_sc), *it_dist);
        //}

        //*it_dist = (*it_dist < 0) ? -*it_dist : *it_dist;
        if (*it_dist < dist_min) {
            pr = i;
            dist_min = *(it_dist++);
        } else
            ++it_dist;

    }

    return pr;

}

size_n predict_mahalanobis_sns_si32(si32 *distance, const si8 dist_sc, const si16 *fv, const si8 fv_sc, const si16 *means,
                                    const si16 *covar_inv, const si8 *covar_inv_sc, const size_n n_feat) {

    si32 temp[CLASS_COUNT];
    memset(temp, 0, CLASS_COUNT*sizeof(si16));
    si32* it_dist = distance;
    const si16* it_fv = fv;
    const si16* it_means = means;
    const si16* it_ci = covar_inv;
    const si8* it_cis = covar_inv_sc;

    uint8_t j = 0;
    for (; j < SENS_COUNT; ++j) {

        predict_mahalanobis_si32(it_dist, dist_sc, it_fv, fv_sc, it_means, it_ci, it_cis, n_feat);
        arr_add_self_si32e(temp, it_dist, CLASS_COUNT);
        it_dist += CLASS_COUNT;
        it_fv += n_feat;
        it_means += n_feat*CLASS_COUNT;
        it_ci += COVAR_L_S;
        it_cis += COVAR_L_S;

    }

    return arr_argmin_si32(temp, CLASS_COUNT);

}

size_n predict_mahalanobis_sns_si16(si16 *distance, const si8 dist_sc, const si16 *fv, const si8 fv_sc, const si16 *means,
                                const si16 *covar_inv, const si8 *covar_inv_sc, const size_n n_feat) {

    si16 temp[CLASS_COUNT];
    memset(temp, 0, CLASS_COUNT*sizeof(si16));
    si16* it_dist = distance;
    const si16* it_fv = fv;
    const si16* it_means = means;
    const si16* it_ci = covar_inv;
    const si8* it_cis = covar_inv_sc;

    uint8_t j = 0;
    for (; j < SENS_COUNT; ++j) {

        predict_mahalanobis_si16(it_dist, dist_sc, it_fv, fv_sc, it_means, it_ci, it_cis, n_feat);
        arr_add_self_si16e(temp, it_dist, CLASS_COUNT);
        //arr_print_si16e(temp, dist_sc, CLASS_COUNT);
        it_dist += CLASS_COUNT;
        it_fv += n_feat;
        it_means += n_feat*CLASS_COUNT;
        it_ci += COVAR_L_S;
        it_cis += COVAR_L_S;

    }

    //printf("%i\n", arr_argmin_si16e(temp, CLASS_COUNT));
    return arr_argmin_si16e(temp, CLASS_COUNT);

}

size_n predict_mahalanobis_sns_fl(float *distance, const float *fv, const float *means,
                                    const float *covar_inv, const size_n n_feat) {

    float temp[CLASS_COUNT];
    memset(temp, 0, CLASS_COUNT*sizeof(float));
    float* it_dist = distance;
    const float* it_fv = fv;
    const float* it_means = means;
    const float* it_ci = covar_inv;

    uint8_t j = 0;
    for (; j < SENS_COUNT; ++j) {

        predict_mahalanobis_fl(it_dist, it_fv, it_means, it_ci, n_feat);
        arr_add_self_fl(temp, it_dist, CLASS_COUNT);

        it_dist += CLASS_COUNT;
        it_fv += n_feat;
        it_means += n_feat*CLASS_COUNT;
        it_ci += COVAR_L_S;

    }

    return arr_argmin_fl(temp, CLASS_COUNT);

}


size_n predict_mahalanobis_ui16(ui16 *distance, ui8 *dist_sc, const ui16 *fv, const ui8 fv_sc, const ui16 *means,
                                const ui16 *covar_inv, const ui8 *covar_inv_sc, const size_n n_feat) {

    ui16 fv_m[n_feat];
    ui8 fv_m_sc[n_feat];
    ui8 t_scale[n_feat];
    ui16 t[n_feat];

    ui16 dist_min = UI16_MAX;
    ui16* it_dist = distance;
    ui8* it_dist_sc = dist_sc;
    size_n pr = 0;

    uint8_t i = 0;
    for (; i < CLASS_COUNT; ++i) {

        arr_sub_ui16e(fv_m, fv_m_sc, fv, fv_sc, means + i * n_feat, fv_sc, n_feat);
        arr_trnsf_upp_ui16d(t, t_scale, fv_m, fv_m_sc, covar_inv, covar_inv_sc, n_feat);
        *it_dist = arr_mult_scal_ui16de_ui16e_(it_dist_sc, t, t_scale, fv_m, fv_m_sc, n_feat);

        //*it_dist = (*it_dist < 0) ? -*it_dist : *it_dist;
        if (*it_dist < dist_min) {
            pr = i;
            dist_min = *(it_dist++);
        } else
            ++it_dist;

        ++it_dist_sc;

    }

    return pr;

}

size_n predict_mahalanobis_sns_ui16(ui16 *distance, ui8 *dist_sc, const ui16 *fv, const ui8 fv_sc, const ui16 *means,
                                    const ui16 *covar_inv, const ui8 *covar_inv_sc, const size_n n_feat) {

    ui16 temp[CLASS_COUNT];
    ui8 temp_sc[CLASS_COUNT];
    memset(temp, 0, CLASS_COUNT*sizeof(ui16));
    ui16* it_dist = distance;
    ui8* it_dist_sc = dist_sc;
    const ui16* it_fv = fv;
    const ui16* it_means = means;
    const ui16* it_ci = covar_inv;
    const ui8* it_cis = covar_inv_sc;

    uint8_t j = 0;
    for (; j < SENS_COUNT; ++j) {

        predict_mahalanobis_ui16(it_dist, it_dist_sc, it_fv, fv_sc, it_means, it_ci, it_cis, n_feat);
        arr_add_self_ui16e(temp, temp_sc, it_dist, it_dist_sc, CLASS_COUNT);

        it_dist += CLASS_COUNT;
        it_dist_sc += CLASS_COUNT;
        it_fv += n_feat;
        it_means += n_feat*CLASS_COUNT;
        it_ci += COVAR_L_S;
        it_cis += COVAR_L_S;

    }

    return arr_argmin_ui16e(temp, CLASS_COUNT);

}

void test_mahalanobis() {

    //path to database
    char* data_path = "/home/speklap/CLionProjects/untitled/database/raw_win_data";
    char* dp_mns = "/home/speklap/CLionProjects/untitled/database/raw_win_data/0/means_train.csv";
    char* dp_mns_sns = "/home/speklap/CLionProjects/untitled/database/raw_win_data/0/means_sens_train.csv";
    char* dp_cov_inv = "/home/speklap/CLionProjects/untitled/database/raw_win_data/0/covar_inv_train.csv";
    char* dp_cov_inv_sns = "/home/speklap/CLionProjects/untitled/database/raw_win_data/0/covar_inv_sens_train.csv";

    //init feature vector
    si16 fv_si16[FEAT_TOT_COUNT];
    ui16 fv_ui16[FEAT_TOT_COUNT];
    ui8 fv_ui16_sc[FEAT_TOT_COUNT];
    memset(fv_ui16_sc, S_feat_si, FEAT_TOT_COUNT * sizeof(ui8));
    float fv_fl[FEAT_TOT_COUNT];

    //init mean matrix
    float means_fl[CLASS_COUNT*FEAT_TOT_COUNT];
    si16 means_si[CLASS_COUNT*FEAT_TOT_COUNT];
    ui16 means_ui[CLASS_COUNT*FEAT_TOT_COUNT];
    ui8 means_ui_sc[CLASS_COUNT*FEAT_TOT_COUNT];
    memset(means_ui_sc, S_feat_si,CLASS_COUNT*FEAT_TOT_COUNT*sizeof(ui8));
    float means_sns_fl[SENS_COUNT*CLASS_COUNT*FEAT_COUNT];
    si16 means_sns_si[SENS_COUNT*CLASS_COUNT*FEAT_COUNT];
    ui16 means_sns_ui[SENS_COUNT*CLASS_COUNT*FEAT_COUNT];
    ui8 means_sns_ui_sc[SENS_COUNT*CLASS_COUNT*FEAT_COUNT];
    memset(means_sns_ui_sc, S_feat_si, SENS_COUNT*CLASS_COUNT*FEAT_COUNT*sizeof(ui8));

    mat_read_fl(means_fl, dp_mns, CLASS_COUNT, FEAT_TOT_COUNT);
    mat_fl_si16e(means_si, S_feat_si, means_fl, CLASS_COUNT, FEAT_TOT_COUNT);
    mat_fl_ui16e(means_ui, means_ui_sc, means_fl, CLASS_COUNT, FEAT_TOT_COUNT);
    mat_read_series_fl(dp_mns_sns, means_sns_fl, CLASS_COUNT, FEAT_COUNT, SENS_COUNT);

    arr_fl_si16e(means_sns_si, S_feat_si, means_sns_fl, CLASS_COUNT*FEAT_COUNT*SENS_COUNT);
    arr_fl_ui16e(means_sns_ui, means_sns_ui_sc, means_sns_fl, CLASS_COUNT*FEAT_COUNT*SENS_COUNT);

    //init inverse covariance matrix
    float ci_full[FEAT_TOT_COUNT*FEAT_TOT_COUNT];
    mat_read_fl(ci_full, dp_cov_inv, FEAT_TOT_COUNT, FEAT_TOT_COUNT);
    float ci_sns_full[FEAT_COUNT*FEAT_COUNT*SENS_COUNT];
    mat_read_series_fl(dp_cov_inv_sns, ci_sns_full, FEAT_COUNT, FEAT_COUNT, SENS_COUNT);

    float ci_fl[COVAR_L];
    float ci_sns_fl[COVAR_L_S*SENS_COUNT];

    si16 ci_si16d[COVAR_L];
    si8 ci_si16d_sc[COVAR_L];
    si16 ci_sns_si16d[COVAR_L_S*SENS_COUNT];
    si8 ci_sns_si16d_sc[COVAR_L_S*SENS_COUNT];

    ui16 ci_ui16d[COVAR_L];
    ui8 ci_ui16d_sc[COVAR_L];
    ui16 ci_sns_ui16d[COVAR_L_S*SENS_COUNT];
    ui8 ci_sns_ui16d_sc[COVAR_L_S*SENS_COUNT];

    mat_symm_upp_fl(ci_fl, ci_full, FEAT_TOT_COUNT, FEAT_TOT_COUNT);
    arr_fl_si16d(ci_si16d, ci_si16d_sc, ci_fl, COVAR_L);
    arr_fl_ui16d(ci_ui16d, ci_ui16d_sc, ci_fl, COVAR_L);

    uint8_t sen = 0;
    float* it_cis_full = ci_sns_full;
    float* it_cis_fl = ci_sns_fl;
    si16* it_cis_sd = ci_sns_si16d;
    si8* it_cis_sd_s = ci_sns_si16d_sc;
    ui16* it_cis_ud = ci_sns_ui16d;
    ui8* it_cis_ud_s = ci_sns_ui16d_sc;
    for (; sen < SENS_COUNT; ++sen) {

        mat_symm_upp_fl(it_cis_fl, it_cis_full, FEAT_COUNT, FEAT_COUNT);
        arr_fl_si16d(it_cis_sd, it_cis_sd_s, it_cis_fl, COVAR_L_S);
        arr_fl_ui16d(it_cis_ud, it_cis_ud_s, it_cis_fl, COVAR_L_S);
        it_cis_fl += COVAR_L_S;
        it_cis_sd += COVAR_L_S;
        it_cis_sd_s += COVAR_L_S;
        it_cis_ud += COVAR_L_S;
        it_cis_ud_s += COVAR_L_S;
        it_cis_full += FEAT_COUNT*FEAT_COUNT;

    }

    uint8_t cl_correct;
    uint8_t cl_tmp;
    uint16_t cl_pr_py = 0;
    uint16_t cl_pr_py_sns = 0;
    uint16_t cl_pr_fl = 0;
    uint16_t cl_pr_fl_sns = 0;
    uint16_t cl_pr_si16 = 0;
    uint16_t cl_pr_si16_sns = 0;
    uint16_t cl_pr_si32 = 0;
    uint16_t cl_pr_si32_sns = 0;
    uint16_t cl_pr_ui16 = 0;
    uint16_t cl_pr_ui16_sns = 0;
    uint16_t cnt = 0;

    //mahalanobis distances
    float md_fl[CLASS_COUNT];
    float mds_fl[CLASS_COUNT*SENS_COUNT];

    si16 md_si16[CLASS_COUNT];
    si16 mds_si16[CLASS_COUNT*SENS_COUNT];
    si8 md_si16_sc = 4;

    si32 md_si32[CLASS_COUNT];
    si32 mds_si32[CLASS_COUNT*SENS_COUNT];
    si8 md_si32_sc = 8;

    ui16 md_ui16[CLASS_COUNT];
    ui16 mds_ui16[CLASS_COUNT*SENS_COUNT];
    ui8 md_ui16_sc = 6;


    //open datafile
    char file[100];
    snprintf(file, sizeof(file), "%s/test_data.csv\0", data_path);
    FILE *test_data = fopen(file, "r");
    char buf[11500];
    char *token;

    si16* it_fv_si16;
    ui16* it_fv_ui16;
    ui8* it_fv_ui16_sc;

    while (fgets(buf, sizeof(buf), test_data)) {

        //get correct class
        token = strtok(buf, ",");
        cl_correct = (uint8_t) strtof(token, NULL);

        //get feature vector calculated in python
        fgets(buf, sizeof(buf), test_data);
        arr_read_fl(buf, fv_fl, FEAT_TOT_COUNT);

        //calculate si16 feature vector from samples
        it_fv_si16 = fv_si16;
        it_fv_ui16 = fv_ui16;
        for (sen = 0; sen < SENS_COUNT; ++sen) {
            fgets(buf, sizeof(buf), test_data);
            calc_feat_vec_si(buf, it_fv_si16);
            size_n j = 0;
            for (; j < FEAT_COUNT; ++j) {
                it_fv_ui16[j] = (ui16) it_fv_si16[j];
            }
            it_fv_si16 += FEAT_COUNT;
            it_fv_ui16 += FEAT_COUNT;
        }

        //get class predicted using python
        fgets(buf, sizeof(buf), test_data);
        cl_tmp = (uint8_t) strtof(token, NULL);
        if (cl_tmp == cl_correct)
            ++cl_pr_py;

        //get class distances using python
        fgets(buf, sizeof(buf), test_data);
        arr_read_fl(buf, md_fl, CLASS_COUNT);

        //get class predicted by using python per sensor
        fgets(buf, sizeof(buf), test_data);
        cl_tmp = (uint8_t) strtof(token, NULL);
        if (cl_tmp == cl_correct)
            ++cl_pr_py_sns;

        //get class distances by using python per sensor
        //it_mdsf = mds_fl;
        for (sen = 0; sen < SENS_COUNT; ++sen) {
            fgets(buf, sizeof(buf), test_data);
            //arr_read_fl(buf, it_mdsf, CLASS_COUNT);
            //it_mdsf += CLASS_COUNT;
        }

        //predict class using floats
        cl_tmp = predict_mahalanobis_fl(md_fl, fv_fl, means_fl, ci_fl, FEAT_TOT_COUNT);
        if (cl_tmp == cl_correct)
            ++cl_pr_fl;

        //printf("Distances float\n");
        //arr_print_fl(md_fl, CLASS_COUNT);

        //predict class using si16
        cl_tmp = predict_mahalanobis_si16(md_si16, md_si16_sc, fv_si16, S_feat_si, means_si, ci_si16d,
                                          ci_si16d_sc, FEAT_TOT_COUNT);
        if (cl_tmp == cl_correct)
            ++cl_pr_si16;

        //printf("\nDistances si16\n");
        //arr_print_si16e(md_si16, md_si16_sc, CLASS_COUNT);

        cl_tmp = predict_mahalanobis_si32(md_si32, md_si32_sc, fv_si16, S_feat_si, means_si, ci_si16d,
                                          ci_si16d_sc, FEAT_TOT_COUNT);
        if (cl_tmp == cl_correct)
            ++cl_pr_si32;

        //predict class using ui16
        cl_tmp = predict_mahalanobis_ui16(md_ui16, md_ui16_sc, fv_ui16, S_feat_si, means_ui, ci_ui16d, ci_ui16d_sc, FEAT_TOT_COUNT);
        if (cl_tmp == cl_correct)
            ++cl_pr_ui16;

        //printf("\nDistances ui16\n");
        //arr_print_ui16e(md_ui16, md_ui16_sc, CLASS_COUNT);


        //predict class using floats and per sensor
        cl_tmp = predict_mahalanobis_sns_fl(mds_fl, fv_fl, means_sns_fl, ci_sns_fl, FEAT_COUNT);
        if (cl_tmp == cl_correct)
            ++cl_pr_fl_sns;

        //predict class using si16 and per sensor
        cl_tmp = predict_mahalanobis_sns_si16(mds_si16, md_si16_sc, fv_si16, S_feat_si, means_sns_si, ci_sns_si16d, ci_sns_si16d_sc, FEAT_COUNT);
        if (cl_tmp == cl_correct)
            ++cl_pr_si16_sns;

        cl_tmp = predict_mahalanobis_sns_si32(mds_si32, md_si32_sc, fv_si16, S_feat_si, means_sns_si, ci_sns_si16d, ci_sns_si16d_sc, FEAT_COUNT);
        if (cl_tmp == cl_correct)
            ++cl_pr_si32_sns;

        ++cnt;

    }

    printf("SCORE python:\t\t%f\n", (float) (cl_pr_py)/cnt);
    printf("SCORE python sns:\t%f\n", (float) (cl_pr_py_sns)/cnt);
    printf("\n");
    printf("SCORE float:\t\t%f\n", (float) (cl_pr_fl)/cnt);
    printf("SCORE float sns:\t%f\n", (float) (cl_pr_fl_sns)/cnt);
    printf("\n");
    printf("SCORE si16:\t\t\t%f\n", (float) (cl_pr_si16)/cnt);
    printf("SCORE si16 sns:\t\t%f\n", (float) (cl_pr_si16_sns)/cnt);
    printf("\n");
    printf("SCORE si32:\t\t\t%f\n", (float) (cl_pr_si32)/cnt);
    printf("SCORE si32 sns:\t\t%f\n", (float) (cl_pr_si32_sns)/cnt);
    printf("\n");
    printf("SCORE ui16:\t\t\t%f\n", (float) (cl_pr_ui16)/cnt);
    printf("SCORE ui16 sns:\t\t%f\n", (float) (cl_pr_ui16_sns)/cnt);


}


size_n predict_mahalanobis_si16_old(si32 *distance, si8 *dist_sc, const si16 *fv, const si8 fv_sc, const si16 *means,
                                    const si16 *covar_inv, const si8 *covar_inv_sc, const size_n n_feat) {

    si16 temp[n_feat];
    si8 t_scale[n_feat];
    si16 t[n_feat];

    si32 dist = 2147483647;
    *dist_sc = 0;
    si32 temp_dist;
    si8 temp_dist_scale;
    size_n pr = 0;

    uint8_t i = 0;
    for (; i < CLASS_COUNT; ++i) {

        arr_sub_si16e(temp, fv, means + i * n_feat, n_feat);
        arr_trnsf_upp_si16d(t, t_scale, temp, fv_sc, covar_inv, covar_inv_sc, n_feat);

        temp_dist = arr_mult_scal_si16de_si32d(&temp_dist_scale, t, t_scale, temp, fv_sc, n_feat);
        temp_dist >>= 1;

        distance[i] = temp_dist;

        if (temp_dist_scale > *dist_sc) {

            if (temp_dist > 0) {
                if (temp_dist >> (temp_dist_scale -  *dist_sc) < dist) {
                    pr = i;
                    dist = temp_dist;
                    *dist_sc = temp_dist_scale;
                }
            } else {
                if (-temp_dist >> (temp_dist_scale -  *dist_sc) < dist) {
                    pr = i;
                    dist = -temp_dist;
                    *dist_sc = temp_dist_scale;
                }
            }

        } else {

            if (temp_dist > 0) {
                if (temp_dist << (*dist_sc - temp_dist_scale) < dist) {
                    pr = i;
                    dist = temp_dist;
                    *dist_sc = temp_dist_scale;
                }
            } else {
                if (-temp_dist << (*dist_sc - temp_dist_scale) < dist) {
                    pr = i;
                    dist = -temp_dist;
                    *dist_sc = temp_dist_scale;
                }
            }

        }

    }

    return pr;

}
