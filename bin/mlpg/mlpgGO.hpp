FrameSeq *mlpgGO(FrameSeq *features, FrameSeq *covariance, const int static_dimension);

void build_poe(const float *b, const float *d_b, const float *dd_b, float *ret_b,
               const float *tau, const float *d_tau, const float *dd_tau, float *ret_prec, const int n);

void pentaband_linearsolve(float *A, float *b, float *x, const int n);