#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lib/common.hpp"
//#include "mkl.h"
#include "cblas.h"


FrameSeq *freqtGO(FrameSeq *c1, const int m1, const float a1, const int m2, const float a2)
{
      const float a = (a1 - a2) / (1 - a1*a2);
      const float b = 1 - a*a;
      
      clock_t start, end;
      float dur;

      // c1t: c1 tranposed version
      start = clock();
      FrameSeq *c1t = FrameSeq_transcopy(c1);
      end = clock();
      dur = (float)(end - start) / CLOCKS_PER_SEC;
      fprintf(stderr, "Transpose c1 costs: %f s.\n", dur);
      // g, d: for temp cal
      // init with all 0s.
      FrameSeq *g = FrameSeq_new(c1->len, m2+1);
      g->len = c1->dim;
      FrameSeq *d = FrameSeq_new(c1->len, m2+1);
      d->len = c1->dim;

      start = clock();
      for (int i=-m1; i <= 0; i++) {
            // m = 0
            if (0 <= m2) {
                  cblas_scopy(g->dim, g->data, 1, d->data, 1);
                  cblas_saxpby(g->dim, 1, c1t->data+(-i)*c1t->dim, 1, a, g->data, 1);
            }
            // m = 1
            if (1 <= m2) {
                  cblas_scopy(g->dim, g->data+g->dim, 1, d->data+d->dim, 1); 
                  cblas_saxpby(g->dim, b, d->data, 1, a, g->data+g->dim, 1);
            }
            // m = 2, ..., m2
            for (int j=2; j <= m2; j++) {
                  cblas_scopy(g->dim, g->data+j*g->dim, 1, d->data+j*d->dim, 1);
                  cblas_saxpby(g->dim, -a, g->data+(j-1)*g->dim, 1, a, g->data+j*g->dim, 1);
                  cblas_saxpy(g->dim, 1, d->data+(j-1)*d->dim, 1, g->data+j*g->dim, 1);
            }
      }
      end = clock();
      dur = (float)(end - start) / CLOCKS_PER_SEC;
      fprintf(stderr, "Calculating c2 costs: %f s.\n", dur);

      FrameSeq *c2 = FrameSeq_transcopy(g);

      FrameSeq_del(c1t);
      FrameSeq_del(g);
      FrameSeq_del(d);

      return c2;
}
