#include <immintrin.h>

void W_update3(double * W, double * d){
    // Scalar replacement
    // Loop unrolling: by 2

    __m128d w1, w2;
    w1 = _mm_load_sd(W);
    w2 = _mm_load_sd(d);
     
    w1 = _mm_add_sd(w1, w2);

     _mm_store_sd(W, w1);           
}
