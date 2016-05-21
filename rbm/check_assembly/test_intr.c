#include <immintrin.h>

void W_update3(double * W){
    // Scalar replacement
    // Loop unrolling: by 2

    __m128d w1;
    w1 = _mm_load_sd(W);
            

     _mm_store_sd(W, w1);
     _mm_store_sd(W, w1);
     _mm_store_sd(W, w1);
     _mm_store_sd(W, w1);
     _mm_store_sd(W, w1);
           
}
