/****************************************************************************************
*                                                                                       *
*    Fast ML-based Parameter Generation from PDFs                                       *
*                                                                                       *
*                                    2018.9  Mu Wang                                    *
*                                                                                       *
*       usage:                                                                          *
*               mlpgGO [ options ] [infile] > stdout                                    *
*       options:                                                                        *
*               -m m              : order of vector                              [25]   *
*               -h                : print this message                                  *
*       infile:                                                                         *
*              PDF sequence                                                             *
*              ex.)   , m(0), m(1), ..., m((m+1)*d-1),                                  *
*                     U(0), U(1), ..., U((m+1)*d-1),                                    *
*       stdout:                                                                         *
*              parameter sequence                                                       *
*                     , c(0), c(1), ..., c_1(m),                                        *
*                                                                                       *
****************************************************************************************/



/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>


#ifdef HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#ifndef HAVE_STRRCHR
#define strrchr rindex
#endif
#endif

#include "lib/common.hpp"
#include "mlpgGO.hpp"


/*  Default Values  */
#define ORDERC1 25

/*  Command Name  */
char *cmnd;


void usage(int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - obtain parameter sequence from PDF sequence\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -m m  : order of vector      [%d]\n",
           ORDERC1);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr,
           "       PDF sequence                                                     [stdin]\n");
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       parameter sequence\n");
   fprintf(stderr, "\n");
   exit(status);
}

int main(int argc, char **argv)
{
   int m1 = ORDERC1;
   FILE *fp = stdin;
   float *c1;

   if ((cmnd = strrchr(argv[0], '/')) == NULL)
      cmnd = argv[0];
   else
      cmnd++;

   while (--argc)
      if (**++argv == '-') {
         switch (*(*argv + 1)) {
         case 'm':
            m1 = atoi(*++argv);
            --argc;
            break;
         case 'h':
            usage(0);
         default:
            fprintf(stderr, "%s : Invalid option '%c'!\n", cmnd, *(*argv + 1));
            usage(1);
         }
      } else
         fp = getfp(*argv, (char *)"rb");

   int static_dimension = m1 + 1;
   c1 = fgetmem(static_dimension*6);

   FrameSeq *features = FrameSeq_new(static_dimension*3);
   FrameSeq *covariance = FrameSeq_new(static_dimension*3);

   //while (freadf(c1, sizeof(*c1), m1 + 1, fp) == m1 + 1) {
   while (fread(c1, sizeof(*c1), static_dimension*6, fp) == static_dimension*6) {
      FrameSeq_pushframe(features, c1);
      FrameSeq_pushframe(covariance, c1+static_dimension*3);
   }

   FrameSeq *gen_params = mlpgGO(features, covariance, static_dimension);
   for (int i = 0; i < gen_params->len; i++) {
       //fwritef(c2_seq->data + i*c2_seq->dim, sizeof(double), c2_seq->dim, stdout);
       fwrite(gen_params->data + i*gen_params->dim, sizeof(float), gen_params->dim, stdout);
   }

   FrameSeq_del(features);
   FrameSeq_del(covariance);
   FrameSeq_del(gen_params);

   return 0;
}
