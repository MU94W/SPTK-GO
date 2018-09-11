#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lib/common.hpp"
#include "mkl.h"
//#include "cblas.h"
// #include <fenv.h>
#include <xmmintrin.h>

void build_poe(const float *b, const float *d_b, const float *dd_b, float *ret_b,
               const float *tau, const float *d_tau, const float *dd_tau, float *ret_prec, const int n);

void pentaband_linearsolve(float *A, float *b, float *x, const int n);

FrameSeq *mlpgGO(FrameSeq *features, FrameSeq *covariance, const int static_dimension)
/*
Args:
    features:
    covariance:
    static_dimension:
*/
{
    // fesetenv(FE_DFL_DISABLE_SSE_DENORMS_ENV); // avoid denormal float point values
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);

    FrameSeq *ft = FrameSeq_transcopy(features);
    FrameSeq *ct = FrameSeq_transcopy(covariance);
    int n = features->len;
    float *b, *d_b, *dd_b, *ret_b = (float *)malloc(n*sizeof(float));
    float *tau, *d_tau, *dd_tau, *ret_prec = (float *)malloc(5*n*sizeof(float));

    FrameSeq *tmp = FrameSeq_new(n, static_dimension);
    tmp->len = static_dimension;
    b = ft->data;       d_b = b + static_dimension*n;       dd_b = d_b + static_dimension*n;
    tau = ct->data;     d_tau = tau + static_dimension*n;   dd_tau = d_tau + static_dimension*n;
    for (int i = 0; i < static_dimension; i++) {
        d_tau[0] = 1e11;
        d_tau[n-1] = 1e11;
        dd_tau[0] = 1e11;
        dd_tau[n-1] = 1e11;

        for (int j = 0; j < n; j++) {
            tau[j] = 1. / tau[j];
            b[j] *= tau[j];
            d_tau[j] = 1. / d_tau[j];
            d_b[j] *= d_tau[j];
            dd_tau[j] = 1. /dd_tau[j];
            dd_b[j] *= dd_tau[j];
        }

        build_poe(b, d_b, dd_b, ret_b, tau, d_tau, dd_tau, ret_prec, n);
        pentaband_linearsolve(ret_prec, ret_b, tmp->data + i*n, n);

        b += n;     d_b += n;       dd_b += n;
        tau += n;   d_tau += n;     dd_tau += n;
    }

    FrameSeq *ret = FrameSeq_transcopy(tmp);

    free(ret_b);
    free(ret_prec);
    FrameSeq_del(ft);
    FrameSeq_del(ct);
    FrameSeq_del(tmp);

    return ret;
}

void build_poe(const float *b, const float *d_b, const float *dd_b, float *ret_b,
               const float *tau, const float *d_tau, const float *dd_tau, float *ret_prec, const int n)
{
    // since we need tranpose bandmat ...
    //static const float d_l = -0.5;
    static const float d_l = 0.5;
    //static const float d_u = 0.5;
    static const float d_u = -0.5;
    static const float dd_l = 1.;
    static const float dd_c = -2.;
    static const float dd_u = 1.;

    // 1st. bm.dot_mv_plus_equals
    ret_b[0] = b[0] + d_u*d_b[1] + dd_c*dd_b[0] + dd_u*dd_b[1];
    for (int i = 1; i < n-1; i++) {
        ret_b[i] = b[i] + d_l*d_b[i-1] + d_u*d_b[i+1] + dd_l*dd_b[i-1] + dd_c*dd_b[i] + dd_u*dd_b[i+1];
    }
    ret_b[n-1] = b[n-1] + d_l*d_b[n-2] + dd_l*dd_b[n-2] + dd_c*dd_b[n-1];

    // 2nd. bm.dot_mm_plus_equals
    static const float d_lu = d_l*d_u;
    static const float dd_lu = dd_l*dd_u;
    static const float dd_lc = dd_l*dd_c;
    static const float dd_cu = dd_c*dd_u;
    static const float d_ll = d_l*d_l;
    static const float d_uu = d_u*d_u;
    static const float dd_ll = dd_l*dd_l;
    static const float dd_cc = dd_c*dd_c;
    static const float dd_uu = dd_u*dd_u;
    for (int i = 0; i < n; i++) {
        if (i >= 2) ret_prec[5*i] = d_lu*d_tau[i-1] + dd_lu*dd_tau[i-1];
        if (i >= 1) ret_prec[5*i+1] = dd_lc*dd_tau[i-1] + dd_cu*dd_tau[i];
        ret_prec[5*i+2] = tau[i] + (d_ll*d_tau[i-1] + d_uu*d_tau[i+1]) + (dd_ll*dd_tau[i-1] + dd_cc*dd_tau[i] + dd_uu*dd_tau[i+1]);
        if (i <= n-2) ret_prec[5*i+3] = dd_lc*dd_tau[i] + dd_cu*dd_tau[i+1];
        if (i <= n-3) ret_prec[5*i+4] = d_lu*d_tau[i+1] + dd_lu*dd_tau[i+1];
    }

}

void pentaband_linearsolve(float *A, float *b, float *x, const int n) {
/*
Args:
    A: [(0, 0, c, u_0, u_1), (0, l_0, c, u_0, u_1), ..., (l_1, l_0, c, 0, 0)]
    b: [b_0, b_1, ..., b_n-1]
    x: [x_0, x_1, ..., x_n-1]
*/
    float coef;
    // 1st. eliminate the lowermost band.
    for (int r = 2; r < n; r++) {
        coef = A[5*r] / A[5*r-4]; // div by 0? i dont know ...
        // A[5*r] = 0.;     // no need to reset
        A[5*r+1] -= coef * A[5*r-3];
        A[5*r+2] -= coef * A[5*r-2];
        if (r <= n-2) A[5*r+3] -= coef * A[5*r-1];
        b[r] -= coef * b[r-1];
    }
    // 2nd. eliminate the uppermost band.
    for (int r = n-3; r >= 0; r--) {
        coef = A[5*r+4] / A[5*r+8]; // div by 0? i dont know ...
        // A[5*r+4] = 0.;   // no need to reset
        A[5*r+3] -= coef * A[5*r+7];
        A[5*r+2] -= coef * A[5*r+6];
        b[r] -= coef * b[r+1];
    }

    // now, it's a tripleband linear system.

    // 1st. eliminate the lower band. the last row needs to be handled carefully.
    for (int r=1; r < n; r++) {
        coef = A[5*r+1] / A[5*r-3]; // div by 0? i dont know ...
        // A[5*r+1] = 0.;   // no need to reset
        A[5*r+2] -= coef * A[5*r-2];
        b[r] -= coef * b[r-1];
    }
    // 2nd. eliminate the upper band.
    // 3rd. get x.
    // we can do both at the same time.
    x[n-1] = b[n-1] / A[(n-1)*5+2];
    for (int r=n-2; r >= 0; r--) {
        coef = A[5*r+3] / A[5*r+7]; // div by 0? i dont know ...
        // A[5*r+3] = 0.;   // no need to reset
        b[r] -= coef * b[r+1];
        x[r] = b[r] / A[5*r+2];
    }

}
