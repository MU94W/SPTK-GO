#include "common.hpp"
#include <string.h>

FrameSeq *FrameSeq_new(int dim, int capacity) {
    FrameSeq *fs = (FrameSeq *)malloc(sizeof(FrameSeq));
    fs->dim = dim;
    fs->len = 0;
    fs->capacity = capacity;
    //fs->data = (float *)malloc(capacity*dim*sizeof(float));
    // init with all 0s.
    fs->data = (float *)calloc(capacity*dim, sizeof(float));
    return fs;
}

void FrameSeq_del(FrameSeq *fs) {
    free(fs->data);
    free(fs);
}

void FrameSeq_pushframe(FrameSeq *fs, float *frame_data) {
    if (fs->len == fs->capacity) {
        fs->capacity += CAPACITY_INCREAMENT;
        fs->data = (float *)realloc(fs->data, fs->capacity*fs->dim*sizeof(float));
    }
    memcpy((fs->data) + (fs->len)*(fs->dim), frame_data, (fs->dim)*sizeof(float));
    fs->len += 1;
}

FrameSeq *FrameSeq_transcopy(const FrameSeq *fs) {
    FrameSeq *fst = FrameSeq_new(fs->len, fs->dim);
    fst->len = fs->dim;
    // transpose copy
    for (int i = 0; i < fst->len; i++) {
        for (int j = 0; j < fst->dim; j++) {
                (fst->data)[i*fst->dim + j] = (fs->data)[j*fs->dim + i];
        }
    }
    return fst;
}
