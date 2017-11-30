#include "features_si.h"

void mav_si(T* fv, const T* sample, cry* c) {

    if (*sample > ZERO)
        *fv += *sample;
    else
        *fv -= *sample;

    if (*fv > OF_mav-1) {

        ++(*c);
        *fv -= OF_mav;

    }

}


void rms_si(T* fv, const T* sample, si32* t) {

    si32 temp = (si32) *sample;
    *t += temp*temp;

}


void zc_si(T *fv, const T *sample) {

    if (*(sample-1) > *sample) {

        if (*(sample-1) - *sample > THRESHOLD_ZC) {

            if (*sample == ZERO) ++(*fv);

            else if (*sample > ZERO && *(sample - 1) < ZERO) ++(*fv);

            else if (*sample < ZERO && *(sample - 1) > ZERO) ++(*fv);

        }

    }

    if (*(sample-1) < *sample) {

        if (*sample - *(sample-1) > THRESHOLD_ZC) {

            if (*sample == ZERO) ++(*fv);

            else if (*sample > ZERO && *(sample - 1) < ZERO) ++(*fv);

            else if (*sample < ZERO && *(sample - 1) > ZERO) ++(*fv);

        }

    }

}


void wfl_si(T *fv, const T *sample, cry *c) {

    if (*sample > *(sample-1))
        *fv += *sample - *(sample-1);
    else
        *fv += *(sample-1) - *sample;

    if (*fv > OF_wfl-1) {

        ++(*c);
        *fv -= OF_wfl;

    }

}


void scc_si(T *fv, const T *sample) {

    if (*(sample-1) > *sample) {

        if (*(sample-1) - *sample > THRESHOLD_SCC) {

            if (*(sample-1) > *(sample-2)) ++(*fv);

        }

    }

    if (*(sample-1) < *sample) {

        if (*sample - *(sample-1) > THRESHOLD_SCC) {

            if (*(sample-1) < *(sample-2)) ++(*fv);

        }

    }

}


void wamp_si(T *fv, const T *sample) {

    if (*sample > *(sample-1)) {

        if (*sample - *(sample-1) > THRESHOLD_WAMP) ++(*fv);

    } else {

        if (*(sample-1) - *sample > THRESHOLD_WAMP) ++(*fv);

    }

}


void calc_feat_vec_si(char *buf, T *fv) {

    T sample[3];
    T* sam = sample + 1;
    uint16_t w = 0;
    si32 temp_rms = 0;
    cry carry_mav = 0;
    cry carry_wfl = 0;

    char* token = strtok(buf, ",");
    sample[0] = (si16) strtof(token, NULL);
    *fv = 0;
    *(fv+1) = 0;
    mav_si(fv, sample, &carry_mav);
    rms_si(fv+1, sample, &temp_rms);
    *(fv+2) = 0;
    *(fv+3) = 0;
    *(fv+4) = 0;
    *(fv+5) = 0;

    token = strtok(NULL, ",");
    *sam = (si16) strtof(token, NULL);
    mav_si(fv, sam, &carry_mav);
    rms_si(fv+1, sam, &temp_rms);
    wfl_si(fv + 2, sam, &carry_wfl);
    zc_si(fv + 3, sam);
    wamp_si(fv + 4, sam);
    ++sam;// = sample + 2;

    for (; w < WINDOW_LENGTH-2; ++w) {

        token = strtok(NULL, ",");
        *sam = (si16) strtof(token, NULL);
        mav_si(fv, sam, &carry_mav);
        rms_si(fv+1, sam, &temp_rms);
        wfl_si(fv + 2, sam, &carry_wfl);
        zc_si(fv + 3, sam);
        wamp_si(fv + 4, sam);
        scc_si(fv + 5, sam);

        sample[0] = sample[1];
        sample[1] = sample[2];

    }


    //mav: divide by windowlength
    uint8_t vl;
    si16 temp;
    si16 lsb = *(fv) << SL_mav;                  //add decimals
    lsb /= WINDOW_LENGTH;
    if (carry_mav > 7) {
        temp = ((si16) carry_mav) << SThigh_mav;     //cast to 16bit + add decimals
        vl = 1;
    } else {
        temp = ((si16) carry_mav) << STlow_mav;     //cast to 16bit + add decimals
        vl = 0;
    }
    temp /= WINDOW_LENGTH;
    temp <<= SM_mav-(vl?SThigh_mav:STlow_mav)+SL_mav;              //set to same scale as lsb
    *(fv) = (temp + lsb) << (S_feat_si-SL_mav);

    //rms: divide by windowlength and take root, temp is 32 bit
    temp_rms <<= S_rms;
    temp_rms /= WINDOW_LENGTH;
    if (temp_rms > 32767)
        *(fv+1) = 32767;
    else
        *(fv+1) = (si16) temp_rms;           //TODO: check for overflows!!!! set S_rms to zero

    *(fv+1) = (si16) sqrt(*(fv+1));
    *(fv+1) <<= (S_feat_si-S_rmssq);

    //wlf:
    lsb = *(fv+2) << SL_wfl;
    lsb /= CDN_wfl;
    if (carry_wfl > 7) {
        temp = ((si16) carry_wfl) << SThigh_wfl;
        vl = 1;
    } else {
        temp = ((si16) carry_wfl) << STlow_wfl;
        vl = 0;
    }

    //TODO change SM_wfl according to S_feat_si
    temp /= CDN_wfl;
    temp <<= SM_wfl - (vl?SThigh_wfl:STlow_wfl) + SL_wfl;
    *(fv+2) = temp + lsb;

    *(fv+3) <<= S_feat_si;
    *(fv+4) <<= S_feat_si;
    *(fv+5) <<= S_feat_si;

}


void test_features_si() {

    uint8_t s = 0, seq = 4;
    uint8_t sen = 0;
    uint8_t cl;

    T feature_vector[FEAT_TOT_COUNT];
    float feat_vec_cmp[FEAT_TOT_COUNT];
    float distortion[FEAT_TOT_COUNT];
    float mean_dist[FEAT_TOT_COUNT];
    uint16_t cnt = 0;
    uint8_t f = 0;
    for (; f < FEAT_TOT_COUNT; ++f)
        mean_dist[f] = 0;

    T* it_fv;
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
                calc_feat_vec_si(buf, it_fv);

                it_fv += FEAT_COUNT;
                it_cmp += FEAT_COUNT;

            }

            //arr_print_fl(feat_vec_cmp, FEAT_TOT_COUNT);
            //arr_si16_print(feature_vector, SL_wfl, FEAT_TOT_COUNT);

            uint8_t ft = 0;
            for (; ft < FEAT_TOT_COUNT; ++ft)
                distortion[ft] = fabsf(1 - feat_vec_cmp[ft]/ si16_fl(feature_vector[ft], S_feat_si));

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


