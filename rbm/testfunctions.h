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

printf("Prob %f, expected percentage  p*n %f, actual percentage %f\n",sigmoid(c[0]), sigmoid(param)*n, count );

 free(h0);
 free(x0);

}
