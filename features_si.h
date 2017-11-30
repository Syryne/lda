#ifndef UNTITLED_FEATURES_SI_H
#define UNTITLED_FEATURES_SI_H

#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#import <math.h>
#include "proj_cte.h"
#include "fixed_si16.h"

#define ZERO 0
#define THRESHOLD_ZC 1
#define THRESHOLD_SCC 1
#define THRESHOLD_WAMP 1
#define factor 500

typedef int16_t T;
typedef int8_t cry;
#define OF_mav 4096     //2^12  overflow of mav function
#define OF_wfl 2048     //2^11  overflow of wfl_si function

#define SL_mav 3        //scale lsb in mav calc
#define SM_mav 12       //scale msb (~overflow)
#define STlow_mav 12    //scale for temp, -> precision to msb, if carry is <8 (3bits)
#define SThigh_mav 9    //scale for temp, -> precision to msb, if carry is 4<=c<31 (5bits)
#define S_rms 2         //scale for rms
#define S_rmssq 1       //scale after sqrt

#define SL_wfl 4        //scale lsb in mav calc
#define SM_wfl 11       //scale msb (~overflow)
#define SThigh_wfl 9        //scale for temp, -> precision to msb
#define STlow_wfl 12
#define CDN_wfl 256      //common denominator for wfl_si feature, to scale the feature to a lower number
                        //it's linear, so I guess it's ok?

#define S_feat_si 4
#define S_ci_si 8

void test_features_si();
void calc_feat_vec_si(char *buf, T *fv);

#endif //UNTITLED_FEATURES_H
