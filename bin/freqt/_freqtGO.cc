#include <stdio.h>
#include <stdlib.h>
//#include <time.h>

#include "lib/common.hpp"
//#include "mkl.h"
#include "cblas.h"


FrameSeq *freqtGO(FrameSeq *c1, const int m1, const float a1, const int m2, const float a2)
{
      const float a = (a2 - a1) / (1 - a1*a2);
      const float b = 1 - a*a;
      
      // c1t: c1 tranposed version
      FrameSeq *c1t = FrameSeq_transcopy(c1);
      // g, d: for temp cal
      // init with all 0s.
      FrameSeq *g = FrameSeq_new(c1t->dim, m2+1);
      g->len = m2+1;
      FrameSeq *d = FrameSeq_new(c1t->dim, m2+1);
      d->len = m2+1;

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

      FrameSeq *c2 = FrameSeq_transcopy(g);

      FrameSeq_del(c1t);
      FrameSeq_del(g);
      FrameSeq_del(d);

      return c2;
}
