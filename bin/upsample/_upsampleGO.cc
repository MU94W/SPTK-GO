#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lib/common.hpp"
//#include "mkl.h"
#include "cblas.h"


FrameSeq *upsampleGO(FrameSeq *c1, const int ratio) {
    int c_dim = c1->dim;
    int c1_len = c1->len;
    int c2_len = c1_len * ratio;
    FrameSeq *c2 = FrameSeq_new(c_dim, c2_len);


    return c2;
}