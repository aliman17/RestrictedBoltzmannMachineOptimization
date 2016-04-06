#include "rbmV2.c"


void test_h0_cap_update(){

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
      result[i] = (i + i * D + y0);
    }

    // OUTPUT
    h0_cap_update(y0, x0, c, W, U, n, D, K, h0_cap);
    
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
    printf("OK\n");

  free(x0);
  free(W);
  free(c);
  free(U);
  free(h0_cap);
  free(result);
}


void test_uniform(){
    printf("TEST UNIFORM FUNCTION\n");
    for(int i=0; i<100; i++) printf("%f \n",uniform());
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



void test_gibbs_H(double param)
{

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


gibbs_X(x,h0);

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
counts.mr[gibbs_Y(h0)]+=1;
}

printf("gibbs_Y testing: \n");

printf("Probs-  expected number- actual number  \n");
for(int i=0;i<K;i++){
  printf("%f  %f  %f \n",Probs.mr[i],Probs.mr[i]*numsamples,counts.mr[i]);
}


}







int main(){
  
  test_h0_cap_update();

  // THE REST IS HERE


  init_param();

  /*
    Test uniform() function
    */
  //  test_uniform();

    /*
    Test init params
    */
    //    test_init_params(); 

    /*
    test gibbs sampling of h

    test if given p, the outcome has the right distribution
    does not test yet the correctness of the calculation of p

    from the first result the probabilities seem to be inverted in the original implementation.

 
    */

   test_gibbs_H(0);

  
    /*
    test gibbs sampling of X

    test if given p, the outcome has the right distribution
    does not test yet the correctness of the calculation of p

   from the first result the probabilities seem to be inverted in the original implementation.

   
    */
    test_gibbs_x(0.34);
  

    /*
    test gibbs sampling of X

    test if given the weigths/bias, the outcome has the right distribution
    does not test yet the correctness of the calculation of the weights/bias

   from the first result the probabilities seem to be OK

    */

     test_gibbs_Y(1,-100,-100,-100,-100,-100,-100,-100,-100,-100);

     test_gibbs_Y(-100,-100,-100,-100,1,-100,-100,-100,-100,-100);

     test_gibbs_Y(-100,-100,-100,-100,-100,-100,-100,-100,-100,1);

     test_gibbs_Y(1,2,1,2,1,2,1,2,1,2);

  return 0;
}
