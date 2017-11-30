#ifndef UNTITLED_LDA_H
#define UNTITLED_LDA_H

#define COVAR_L ((FEAT_TOT_COUNT+1)*FEAT_TOT_COUNT/2)
#define COVAR_L_S ((FEAT_COUNT+1)*FEAT_COUNT/2)

#include "mat_si16.h"
#include "matrix_fl.h"
#include "features_si.h"
#include "fixed_si16.h"
#include "project.h"

void test_mahalanobis();

#endif //UNTITLED_LDA_H
