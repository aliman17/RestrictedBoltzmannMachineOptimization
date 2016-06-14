#include <immintrin.h>
void c_update(double * c, double * h0_cap, double * h1_cap, double lambda, int n) {

  // //ORIGINAL CODE

        for (int i = 0; i < n; i++)
        {
        c[i] = c[i] + lambda * (h0_cap[i] - h1_cap[i]);
        }



    // __m256d lambdaVec = {lambda,lambda,lambda,lambda};

    // for (int i = 0; i < n; i+=4)
    // {
    //     //loads
    //     __m256d cVec , h0Vec,  h1Vec, temp1,res;
    //     cVec  = _mm256_loadu_pd(c + i );
    //     h0Vec = _mm256_loadu_pd(h0_cap + i );
    //     h1Vec = _mm256_loadu_pd(h1_cap + i );
    //     //substraction

    //     temp1 = _mm256_sub_pd(h0Vec, h1Vec);

    //     //FMA

    //     res = _mm256_fmadd_pd(lambdaVec, temp1, cVec);

    //     // store

    //     _mm256_storeu_pd(c + i, res);
    // }

}