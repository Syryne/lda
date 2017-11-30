#ifndef UNTITLED_FEATURES_UI_H
#define UNTITLED_FEATURES_UI_H

#include <stdint.h>
#include "fixed_ui16.h"
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include "proj_cte.h"
#include "matrix_fl.h"

#define res_adc 10
#define adc_zero_bits 9
#define adc_zero 512

#define OF_mav_ui_bits 12
#define OF_mav_ui 4096      //2^12
#define SL_mav_ui_bits 3
#define ST_h_mav_ui_bits 9
#define ST_l_mav_ui_bits 13
#define ST_lh_mav_ui 7     //2^3-1

#define S_rms_ui_bits 2
#define S_rmssq_ui_bits 1


#define THRES_ZC_ui 1
#define THRES_WAMP_ui 1
#define THRES_SCC_ui 1

#define OF_wfl_ui_bits 12
#define OF_wfl_ui 4096      //2^12
#define SL_wfl_ui_bits 4
#define ST_h_wfl_ui_bits 9
#define ST_l_wfl_ui_bits 13
#define ST_lh_wfl_ui 7
#define CDN_wfl_ui 256

#define S_feat_ui 4

void test_features_ui();

#endif //UNTITLED_FEATURES_UI_H
