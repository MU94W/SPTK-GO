#include <stdlib.h>
#include <stdio.h>

#define FORMAT "double"
#define CAPACITY_INCREAMENT 100

struct FrameSeq {
    int dim;
    int len;
    int capacity;
    float *data;
};
FrameSeq *FrameSeq_new(int dim, int capacity=500);
void FrameSeq_del(FrameSeq *fs);
void FrameSeq_pushframe(FrameSeq *fs, float *frame_data);
FrameSeq *FrameSeq_transcopy(const FrameSeq *fs);


int freada(double *p, const int bl, FILE * fp);
int fwritex(void *ptr, const size_t size, const int nitems, FILE * fp);
int freadx(void *ptr, const size_t size, const int nitems, FILE * fp);
int fwritef(double *ptr, const size_t size, const int nitems, FILE * fp);
int freadf(double *ptr, const size_t size, const int nitems, FILE * fp);
int fwrite_little_endian(void *ptr, const size_t size,
                         const size_t n, FILE * fp);

void fillz(void *ptr, const size_t size, const int nitem);

short *sgetmem(const int leng);
long *lgetmem(const int leng);
double *dgetmem(const int leng);
float *fgetmem(const int leng);
/* real *rgetmem (const int leng); */
float **ffgetmem(const int leng);
double **ddgetmem(const int leng1, const int leng2);
char *getmem(const size_t leng, const size_t size);

void movem(void *a, void *b, const size_t size, const int nitem);

FILE *getfp(char *name, char *opt);
