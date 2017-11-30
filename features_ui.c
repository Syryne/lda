#include "features_ui.h"
#include <math.h>

void mav_ui(ui16* fv, const ui16* sample, ui8* c) {

    if (*sample >> adc_zero_bits != 0)
        *fv += (*sample-adc_zero);
    else
        *fv += (adc_zero-*sample);

    if (*fv >> OF_mav_ui_bits != 0) {

        ++(*c);
        *fv -= OF_mav_ui;

    }

}


void rms_ui(ui16* fv, const ui16* sample, ui32* t) {

    ui32 temp;
    if (*sample >> adc_zero_bits != 0)
        temp = (ui32) (*sample-adc_zero);
    else
        temp = (ui32) (adc_zero-*sample);

    *t += temp*temp;

}


void zc_ui(ui16 *fv, const ui16 *sample) {

    if (*(sample-1)>>adc_zero_bits != *sample>>adc_zero_bits) {
        if (*(sample-1) > *sample) {
            if (*(sample-1) - *sample > THRES_ZC_ui)
                ++(*fv);
        } else {
            if (*sample - *(sample-1) > THRES_SCC_ui)
                ++(*fv);
        }
    }

}


void wfl_ui(ui16 *fv, const ui16 *sample, ui8 *c) {

    if (*sample > *(sample-1))
        *fv += *sample - *(sample-1);
    else
        *fv += *(sample-1) - *sample;

    if (*fv >> OF_wfl_ui_bits != 0) {

        ++(*c);
        *fv -= OF_wfl_ui;

    }

}


void scc_ui(ui16 *fv, const ui16 *sample) {

    if (*(sample-1) > *sample) {

        if (*(sample-1) - *sample > THRES_SCC_ui) {

            if (*(sample-1) > *(sample-2)) ++(*fv);

        }

    }

    if (*(sample-1) < *sample) {

        if (*sample - *(sample-1) > THRES_SCC_ui) {

            if (*(sample-1) < *(sample-2)) ++(*fv);

        }

    }

}


void wamp_ui(ui16 *fv, const ui16 *sample) {

    if (*sample > *(sample-1)) {

        if (*sample - *(sample-1) > THRES_WAMP_ui) ++(*fv);

    } else {

        if (*(sample-1) - *sample > THRES_WAMP_ui) ++(*fv);

    }

}


void calc_feat_vec_ui(char *buf, ui16* fv, const float* fv_cmp) {

    ui16 sample[3];
    ui16* sam = sample + 1;
    uint16_t w = 0;
    ui32 temp_rms = 0;
    ui8 carry_mav = 0;
    ui8 carry_wfl = 0;

    char* token = strtok(buf, ",");
    sample[0] = (ui16) (strtof(token, NULL) + 512);
    *fv = 0;
    *(fv+1) = 0;
    mav_ui(fv, sample, &carry_mav);
    rms_ui(fv+1, sample, &temp_rms);
    *(fv+2) = 0;
    *(fv+3) = 0;
    *(fv+4) = 0;
    *(fv+5) = 0;

    token = strtok(NULL, ",");
    *sam = (ui16) (strtof(token, NULL) + 512);
    mav_ui(fv, sam, &carry_mav);
    rms_ui(fv+1, sam, &temp_rms);
    wfl_ui(fv + 2, sam, &carry_wfl);
    zc_ui(fv + 3, sam);
    wamp_ui(fv + 4, sam);
    ++sam;// = sample + 2;

    for (; w < WINDOW_LENGTH-2; ++w) {

        token = strtok(NULL, ",");
        *sam = (ui16) (strtof(token, NULL) + 512);
        mav_ui(fv, sam, &carry_mav);
        rms_ui(fv+1, sam, &temp_rms);
        wfl_ui(fv + 2, sam, &carry_wfl);
        zc_ui(fv + 3, sam);
        wamp_ui(fv + 4, sam);
        scc_ui(fv + 5, sam);

        sample[0] = sample[1];
        sample[1] = sample[2];

    }


    //mav: divide by windowlength
    uint8_t vl;
    ui16 temp;
    ui16 lsb = *(fv) << SL_mav_ui_bits;                  //add decimals
    lsb /= WINDOW_LENGTH;
    if (carry_mav > ST_lh_mav_ui) {
        temp = ((ui16) carry_mav) << ST_h_mav_ui_bits;     //cast to 16bit + add decimals
        vl = 1;
    } else {
        temp = ((ui16) carry_mav) << ST_l_mav_ui_bits;     //cast to 16bit + add decimals
        vl = 0;
    }
    temp /= WINDOW_LENGTH;
    temp <<= OF_mav_ui_bits-(vl?ST_h_mav_ui_bits:ST_l_mav_ui_bits)+SL_mav_ui_bits;              //set to same scale as lsb
    temp = (temp + lsb) << (S_feat_ui-SL_mav_ui_bits);
    float dst = fabsf(1 - (*fv_cmp)/ ui16_fl(temp, S_feat_ui));
    //if (dst > 0.06)
    //    printf("why?\n");
    *(fv) = temp;

    //rms: divide by windowlength and take root, temp is 32 bit
    temp_rms <<= S_rms_ui_bits;
    temp_rms /= WINDOW_LENGTH;
    if (temp_rms > 65535)
        *(fv+1) = 65535;
    else
        *(fv+1) = (ui16) temp_rms;           //TODO: check for overflows!!!! set S_rms to zero

    *(fv+1) = (ui16) sqrt(*(fv+1));
    *(fv+1) <<= (S_feat_ui-S_rmssq_ui_bits);

    //wlf:
    lsb = *(fv+2) << SL_wfl_ui_bits;
    lsb /= CDN_wfl_ui;
    if (carry_wfl > ST_lh_wfl_ui) {
        temp = ((ui16) carry_wfl) << ST_h_wfl_ui_bits;
        vl = 1;
    } else {
        temp = ((ui16) carry_wfl) << ST_l_wfl_ui_bits;
        vl = 0;
    }
    temp /= CDN_wfl_ui;
    temp <<= OF_wfl_ui_bits - (vl?ST_h_wfl_ui_bits:ST_l_wfl_ui_bits) + SL_wfl_ui_bits;
    *(fv+2) = (temp + lsb) << (S_feat_ui-SL_wfl_ui_bits);

    *(fv+3) <<= S_feat_ui;
    *(fv+4) <<= S_feat_ui;
    *(fv+5) <<= S_feat_ui;

}


void test_features_ui() {

    uint8_t s = 0, seq = 4;
    uint8_t sen = 0;
    uint8_t cl;

    ui16 feature_vector[FEAT_TOT_COUNT];
    float feat_vec_cmp[FEAT_TOT_COUNT];
    float distortion[FEAT_TOT_COUNT];
    float mean_dist[FEAT_TOT_COUNT];
    uint16_t cnt = 0;
    uint8_t f = 0;
    for (; f < FEAT_TOT_COUNT; ++f)
        mean_dist[f] = 0;

    ui16 * it_fv;
    float* it_cmp;


    char* data_path = "/home/speklap/CLionProjects/untitled/database/raw_win_data/0";
    char file[100];
    for (; s < seq; ++s) {

        snprintf(file, sizeof(file), "%s/seq_%i_train.csv\0", data_path, s);
        FILE *train_data = fopen(file, "r");

        char buf[11500];
        char *token;

        while (fgets(buf, sizeof(buf), train_data)) {
            token = strtok(buf, ",");
            cl = (uint8_t) strtof(token, NULL);

            fgets(buf, sizeof(buf), train_data);
            arr_read_fl(buf, feat_vec_cmp, FEAT_TOT_COUNT);

            it_fv = feature_vector;
            it_cmp = feat_vec_cmp;

            for (sen = 0; sen < SENS_COUNT; ++sen) {

                fgets(buf, sizeof(buf), train_data);
                calc_feat_vec_ui(buf, it_fv, it_cmp);

                it_fv += FEAT_COUNT;
                it_cmp += FEAT_COUNT;

            }

            //arr_print_fl(feat_vec_cmp, FEAT_TOT_COUNT);
            //arr_si16_print(feature_vector, SL_wfl, FEAT_TOT_COUNT);

            uint8_t ft = 0;
            for (; ft < FEAT_TOT_COUNT; ++ft)
                distortion[ft] = fabsf(1 - feat_vec_cmp[ft]/ ui16_fl(feature_vector[ft], S_feat_ui));

            arr_add_self_fl(mean_dist, distortion, FEAT_TOT_COUNT);
            //arr_print_fl(distortion, FEAT_TOT_COUNT);
            ++cnt;

        }

    }

    arr_div_self_scal_fl(mean_dist, cnt, FEAT_TOT_COUNT);
    uint8_t cls = 1;
    printf("1");
    for (; cls < FEAT_TOT_COUNT; ++cls)
        printf("\t\t\t%i", cls%FEAT_COUNT+1);
    printf("\n");
    arr_print_fl(mean_dist, FEAT_TOT_COUNT);


}

