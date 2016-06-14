#define UNITTEST_MODE
#include "rbmV2.c"


void test_h_update(){

  double precision = 0.0001;

  // Variables
  int n = 2;
  int D = 2;
  int K = 10;

  // Allocations
  int * x0 = (int *) malloc(sizeof(int) * D);
  double * W = (double *) malloc(n * D * sizeof(double));
  double * c = (double *) malloc(n * sizeof(double));
  double * U = (double *) malloc(n * K * sizeof(double));
  double * h0_cap = (double *) malloc(sizeof(double) * n);
  double * result = (double *) malloc(sizeof(double) * n);

  // Initialization

  /*
   * c = [0 1 2 3 4 5 ... n]
   */
  for (int i = 0; i < n; i++){
    c[i] = i;
  }

  /* W =
   * 000
   * 111
   * 222
   */
  for (int i = 0; i < n; i++){
    for (int j = 0; j < D; j++){
      W[i * D + j] = i;
    }
  }

  /* U =
   * 012
   * 012
   * 012
   */
  for (int i = 0; i < n; i++){
    for (int j = 0; j < K; j++){
      U[i * K + j] = j;
    }
  }

  /* 
   * x = 111
   */
  for (int i = 0; i < D; i++){
    x0[i] = 1;
  }


  /*
   * y0 e [0, 9]
   */
  int isOK = 1; // true
  for (int y0 = 0; y0 < K; y0++){
    // EXPECTED
    for (int i = 0; i < n; i++){
      result[i] = sigmoid(i + i * D + y0);
    }

    // OUTPUT
    h_update(h0_cap, y0, x0, c, W, U, n, D, K);
    
    // COMPARE
    double diff = 0;
    for (int i = 0; i < n; i++){
      diff += (result[i] - h0_cap[i])/(y0+1);
    }

    if (fabs(diff) > precision){
      isOK = 0; // not OK, set to false
      printf("FAILED: Difference is %f \n", diff);
      break;
    }
  }

  if (isOK)
    printf("OK: h_update\n");

  free(x0);
  free(W);
  free(c);
  free(U);
  free(h0_cap);
  free(result);
}

void test_d_update(){
  const int d_size = 10;

  // Initialize d to zeros
  double * d = (double *) calloc(d_size, sizeof(double));  

  int y0 = 3;
  int y1 = 4;
  double lambda = 0.5;
  int K = 10; // num of classes
  double d_expected[d_size] = {0, 0, 0, lambda, -lambda, 0, 0, 0, 0, 0};

  // Compute
  d_update(d, y0, y1, lambda, K);

  // Compare
  for (int i = 0; i < K; i++){
    if (d[i] != d_expected[i]){
      printf("FAILURE: d_update\n");
      return;
    }
  }

  printf("OK: d_update\n");
}

void test_W_update(){

  const int n = 3;
  const int D = 4;
  double lambda = 1;


  double * W = (double *) calloc(n * D, sizeof(double));
  double h0_cap[n] = {1, 2, 3};
  double h1_cap[n] = {-2, 0, -1};
  int x0[D] = {1, 2, 3, 4};
  int x1[D] = {-4, 3, -2, 1};

  double W_expected[n*D] = {-7, 8, -1, 6, 
                             2, 4, 6, 8, 
                            -1, 9, 7, 13};

  // COMPUTE
  W_update(W, h0_cap, h1_cap, x0, x1, lambda, n, D);

  // COMPARE
  for (int i = 0; i < n*D; i++){
    if (W[i] != W_expected[i]){
      printf("FAILURE: W_update\n");
      printf("Expected\n");
      for (int k = 0; k < n; k++){
        for (int j = 0; j<D; j++){
          printf("%f   ", W_expected[k*D + j]);
        }
        printf("\n");
      }

      printf("Computed\n");
      for (int k = 0; k < n; k++){
        for (int j = 0; j<D; j++){
          printf("%f   ", W[k*D + j]);
        }
        printf("\n");
      }
      return;
    }
  }

  printf("OK: W_update\n");
}

void test_b_update(){
  // GIVED
  const int D = 4;
  double * b = (double *) calloc (D, sizeof(double));
  int x0[D] = {1,1,1,1};
  int x1[D] = {2,2,2,2};
  double lambda = 1;

  // EXPECTED
  double b_expected[D] = {-1, -1, -1, -1};

  // COMPUTED
  b_update(b, x0, x1, lambda, D);

  // COMPARE
  for (int i = 0; i < D; i++){
    if (b[i] != b_expected[i]){
      printf("FAILURE: b_update\n");
      return;
    }
  }

  printf("OK: b_update\n");
}


void test_c_update(){
  // GIVED
  const int D = 4;
  double * c = (double *) calloc (D, sizeof(double));
  double h0[D] = {1,1,1,1};
  double h1[D] = {2,2,2,2};
  double lambda = 1;

  // EXPECTED
  double c_expected[D] = {-1, -1, -1, -1};

  // COMPUTED
  c_update(c, h0, h1, lambda, D);

  // COMPARE
  for (int i = 0; i < D; i++){
    if (c[i] != c_expected[i]){
      printf("FAILURE: c_update\n");
      return;
    }
  }

  printf("OK: c_update\n");
}


void test_U_update(){
  // GIVEN
  const int K = 10;
  const int n = 4;
  double * U = (double *) calloc (n*K, sizeof(double));
  double h0_cap[n] = {1, 2, 3, 4};
  double h1_cap[n] = {2, 2, 2, 2};
  int y0 = 3;
  int y1 = 7;
  double lambda = 1;

  // EXPECTED
  double U_expected[n*K] = {0, 0, 0, 1, 0, 0, 0, -2, 0, 0,
                            0, 0, 0, 2, 0, 0, 0, -2, 0, 0,
                            0, 0, 0, 3, 0, 0, 0, -2, 0, 0,
                            0, 0, 0, 4, 0, 0, 0, -2, 0, 0};

  // COMPUTED
  U_update(U, h0_cap, h1_cap, y0, y1, lambda, n, K);

  // COMPARE
  for (int i = 0; i < n*K; i++){
    if (U[i] != U_expected[i]){
      printf("FAILURE: U_update\n");
      printf("Expected\n");
      for (int k = 0; k < n; k++){
        for (int j = 0; j<K; j++){
          printf("%f   ", U_expected[k*K + j]);
        }
        printf("\n");
      }

      printf("Computed\n");
      for (int k = 0; k < n; k++){
        for (int j = 0; j<K; j++){
          printf("%f   ", U[k*K + j]);
        }
        printf("\n");
      }
      return;
    }
  }

  printf("OK: U_update\n");
}


void test_uniform(){
    printf("TEST UNIFORM FUNCTION\n");
    for(int i=0; i<100; i++) printf("%f \n",uniform());
}


void test_energy(){
  // Given
  const int n = 2;
  const int D = 4;
  const int K = 3;

  double h[n] = {1, 2};
  double W[n*D] = {1,2,3,4,0,1,2,3};
  int x[D] = {0,1,1,0};
  double b[D] = {0,1,1,1};
  double c[n] = {2,3};
  double d[K] = {0,0,1};
  int y[K] = {0,0,1};
  double U[n*K] = {1,1,1,2,2,2};

  // Expected
  double expected = -27;

  // Compute
  double result = energy(2, x, h, W, b, c, d, U, n, D, K);

  // Compare
  if (expected == result)
    printf("OK: energy\n");
  else
    printf("FAILED: energy: (expected, computed) %f  %f\n", expected, result);
}

void test_init_params(){

  init_param();   


    for (int i = 0; i < D; i++) {
        if(b[i] != 0) {printf ("test failed b\n"); exit(0);}
    }

    for (int i = 0; i < n; i++) {
        if(c[i] != 0) {printf ("test failed c\n"); exit(0);}
    }

    for (int i = 0; i < K; i++) {
        if(d[i] != 0) {printf ("test failed d\n"); exit(0);}
    }



    double maxW = n < D ? 1.0 / sqrt(D*1.0) : 1.0 / sqrt(n*1.0);
    for (int i = 0 ; i < n * D; i++) {
       if(fabs(W[i])>maxW) {printf ("test failed W\n"); exit(0);}
    }

    double maxU = n < K ? 1.0 / sqrt(K*1.0) : 1.0 / sqrt(n*1.0);

     for (int i = 0 ; i < n * K; i++) {
        if(fabs(U[i])>maxU) {printf ("test failed U \n"); exit(0);}
    }
  printf ("Init params test succeeded \n");
}


void test_gibbs_H_helper(double param)
{
    init_param();
  /*
  test if given p, the outcome has the right distribution
  does not test yet the correctness of the calculation of p
  */
  srand(10);
  // double param=100000;

  for (int i = 0 ; i < n * K; i++) {
          U[i]=0;
      }
  for (int i = 0 ; i < n * D; i++) {
        W[i]=0;
    }

  for (int i = 0 ; i < n ; i++) {
        c[i]=param;
    }



  int *x0 = (int *) malloc(sizeof(int)*D);
    for(int i=0; i<D; i++){

      x0[i]=0;
    }

  int * h0 = (int *) malloc(sizeof(int) * n);

  gibbs_H(h0,0,x0);

  double count=0;

  for(int i=0; i<n; i++)
  {
  if(h0[i]==1) count++;
  }

  printf("gibbs_H: Prob %f, expected percentage  p*n %f, actual percentage %f\n",sigmoid(c[0]), sigmoid(param)*n, count );

 free(h0);
 free(x0);

}




void test_gibbs_x(double param){

/*
test if given p, the outcome has the right distribution
does not test yet the correctness of the calculation of p
*/
srand(10);
int *h0 = (int*) malloc(sizeof(int)*n);
int *x  = (int*) malloc(sizeof(int)*D);

for(int i=0; i<n; i++){
h0[i]=0;
}


for (int i = 0 ; i < n * D; i++) {
      W[i]=0;
  }


for(int i=0; i<n; i++){
b[i]=param;
}


gibbs_X_(x,h0, W, D, n);

double count=0;

for(int i=0; i<D; i++)
{
if(x[i]==1) count++;
}

printf("gibbs_X: Prob %f, expected percentage  p*n %f, actual percentage %f\n",sigmoid(b[0]), sigmoid(param)*D, count );

free(x);
free(h0);

}


void test_gibbs_Y(double d0, double d1, double d2, double d3, double d4, double d5, double d6, double d7, double d8, double d9){

int numsamples=1000;

for (int i = 0 ; i < n * K; i++) {
        U[i]=0;
    }
int *h0 = (int*) malloc(sizeof(int)*n);

for(int i=0; i<n; i++){
h0[i]=0;
}

d[0]=d0;
d[1]=d1;
d[2]=d2;
d[3]=d3;
d[4]=d4;
d[5]=d5;
d[6]=d6;
d[7]=d7;
d[8]=d8;
d[9]=d9;


Parr counts;
Parr Probs;

for(int i=0;i<K;i++){
    counts.mr[i]=0;
    }

double sum=0;
for(int i=0;i<K;i++){
    Probs.mr[i]=exp(d[i]);
    sum+=exp(d[i]);  
  }

for(int i=0;i<K;i++){
    Probs.mr[i]=Probs.mr[i]/sum; 
  }


for(int i=0;i<numsamples;i++){
counts.mr[gibbs_Y_(h0, U, d, K, n)]+=1;
}

printf("gibbs_Y testing: \n");

printf("Probs-  expected number- actual number  \n");
for(int i=0;i<K;i++){
  printf("%f  %f  %f \n",Probs.mr[i],Probs.mr[i]*numsamples,counts.mr[i]);
}


}



int main(){
  
  test_h_update();
  test_d_update();
  test_W_update();
  test_b_update();
  test_c_update();
  test_U_update();

  test_energy();


  test_gibbs_H_helper(-1000);

  // THE REST IS HERE
  return 0;

  init_param();

  

     test_gibbs_Y(1,-100,-100,-100,-100,-100,-100,-100,-100,-100);

     test_gibbs_Y(-100,-100,-100,-100,1,-100,-100,-100,-100,-100);

     test_gibbs_Y(-100,-100,-100,-100,-100,-100,-100,-100,-100,1);

     test_gibbs_Y(1,2,1,2,1,2,1,2,1,2);

  return 0;
}
