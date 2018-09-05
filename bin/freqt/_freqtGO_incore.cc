#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lib/common.hpp"
//#include "mkl.h"
#include "cblas.h"

inline void freqtGO_incore_single(double *c1, double *d, double *g, const int m1, const int m2, const double a, const double b);

FrameSeq *freqtGO_incore(FrameSeq *c1, const int m1, const float a1, const int m2, const float a2)
{
      const double a = (double)(a2 - a1) / (1 - a1*a2);
      const double b = 1 - a*a;
      
      float *c1_ptr = c1->data;
      int n = c1->len;
      int c1_dim = c1->dim; // == m1 + 1
      int c2_dim = m2 + 1;
      FrameSeq *c2 = FrameSeq_new(c2_dim, n);


      double *c1_s = (double *)malloc(c1_dim*sizeof(double));
      double *g = (double *)malloc(c2_dim*sizeof(double));
      float *gf = (float *)malloc(c2_dim*sizeof(float));
      double *d = (double *)malloc(c2_dim*sizeof(double));

      float dur_set = 0.;
      float dur_cpy = 0.;
      float dur_cal = 0.;
      float dur_pus = 0.;
      clock_t start, end;
      for (int i = 0; i < n; i++) {
            start = clock();
            memset(g, 0x00, c2_dim*sizeof(double));
            end = clock();
            dur_set += (float)(end - start) / CLOCKS_PER_SEC;
            start = clock();
            //memcpy(c1_s, c1_ptr, c1_dim*sizeof(float));
            for (int j = 0; j < c1_dim; j++) c1_s[j] = (double)c1_ptr[j];
            end = clock();
            dur_cpy += (float)(end - start) / CLOCKS_PER_SEC;
            start = clock();
            freqtGO_incore_single(c1_s, d, g, m1, m2, a, b);
            end = clock();
            dur_cal += (float)(end - start) / CLOCKS_PER_SEC;
            start = clock();
            for (int j = 0; j < c2_dim; j++) gf[j] = (float)g[j];
            FrameSeq_pushframe(c2, gf);
            end = clock();
            dur_pus += (float)(end - start) / CLOCKS_PER_SEC;
            c1_ptr += c1_dim;
      }
      fprintf(stderr, "set: %f\n", dur_set);
      fprintf(stderr, "cpy: %f\n", dur_cpy);
      fprintf(stderr, "cal: %f\n", dur_cal);
      fprintf(stderr, "pus: %f\n", dur_pus);
      free(c1_s);
      free(g);
      free(d);
      free(gf);


      //float *g_ptr = c2->data;
      //float *d = (float *)malloc(c2_dim*sizeof(float));
      //clock_t start = clock();
      //for (int i = 0; i < n; i++) {
      //      freqtGO_incore_single(c1_ptr, d, g_ptr, m1, m2, a, b);
      //      c1_ptr += c1_dim;
      //      g_ptr += c2_dim;
      //}
      //clock_t end = clock();
      //fprintf(stderr, "%f s\n", (float)(end - start) / CLOCKS_PER_SEC);
      //free(d);

      return c2;
}

#define LIMIT 1e-37

inline void freqtGO_incore_single(double *c1, double *d, double *g, const int m1, const int m2, const double a, const double b)
{
      int i, j;
      for (i = -m1; i <= 0; i++) {
            if (0 <= m2) {
                  d[0] = g[0];
                  g[0] = c1[-i] + a * d[0];
            }
            if (1 <= m2) {
                  d[1] = g[1];
                  g[1] = b * d[0] + a * d[1];
            }
            for (j = 2; j <= m2; j++) {
                  d[j] = g[j];
                  g[j] = d[j - 1] + a * (d[j] - g[j - 1]);
            }
      }
}
