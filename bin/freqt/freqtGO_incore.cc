/************************************************************************
*                                                                       *
*    Frequency Transformation                                           *
*                                                                       *
*                                       1988.2  K.Tokuda                *
*                                       1996.1  K.Koishida              *
*                                                                       *
*       usage:                                                          *
*               freqt [ options ] [ infile ] > stdout                   *
*       options:                                                        *
*               -m m     :  order of minimum phase sequence     [25]    *
*               -M M     :  order of warped sequence            [25]    *
*               -a a     :  all-pass constant of input sequence [0]     *
*               -A A     :  all-pass constant of output sequence[0.35]  *
*       infile:                                                         *
*               minimum phase sequence                                  *
*                   , c(0), c(1), ..., c(M),                            *
*       stdout:                                                         *
*               warped sequence                                         *
*                   , c~(0), c~(1), ..., c~(N),                         *
*       require:                                                        *
*               freqt()                                                 *
*                                                                       *
************************************************************************/

static char *rcs_id = (char*)"$Id$";


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
#include "freqtGO.hpp"


/*  Default Values  */
#define ORDERC1 25
#define ORDERC2 25
#define ALPHA1 0.0
#define ALPHA2 0.35

/*  Command Name  */
char *cmnd;


void usage(int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - frequency transformation\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -m m  : order of minimum phase sequence      [%d]\n",
           ORDERC1);
   fprintf(stderr, "       -M M  : order of warped sequence             [%d]\n",
           ORDERC2);
   fprintf(stderr, "       -a a  : all-pass constant of input sequence  [%g]\n",
           ALPHA1);
   fprintf(stderr, "       -A A  : all-pass constant of output sequence [%g]\n",
           ALPHA2);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       minimum phase sequence (%s)               [stdin]\n",
           FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       warped sequence (%s)\n", FORMAT);
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n", PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}

int main(int argc, char **argv)
{
   int m1 = ORDERC1, m2 = ORDERC2;
   FILE *fp = stdin;
   float *c1, a1 = ALPHA1, a2 = ALPHA2, a;

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
         case 'M':
            m2 = atoi(*++argv);
            --argc;
            break;
         case 'a':
            a1 = atof(*++argv);
            --argc;
            break;
         case 'A':
            a2 = atof(*++argv);
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

   c1 = fgetmem(m1 + 1);

   a = (a2 - a1) / (1 - a1 * a2);

   FrameSeq *c1_seq = FrameSeq_new(m1+1);

   //while (freadf(c1, sizeof(*c1), m1 + 1, fp) == m1 + 1) {
   while (fread(c1, sizeof(*c1), m1 + 1, fp) == m1 + 1) {
      FrameSeq_pushframe(c1_seq, c1);
   }
   FrameSeq *c2_seq = freqtGO_incore(c1_seq, m1, a1, m2, a2);
   for (int i = 0; i < c2_seq->len; i++) {
       //fwritef(c2_seq->data + i*c2_seq->dim, sizeof(double), c2_seq->dim, stdout);
       fwrite(c2_seq->data + i*c2_seq->dim, sizeof(float), c2_seq->dim, stdout);
   }

   FrameSeq_del(c1_seq);
   FrameSeq_del(c2_seq);

   return 0;
}
