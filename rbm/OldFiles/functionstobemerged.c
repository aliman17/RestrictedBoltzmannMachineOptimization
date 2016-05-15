
void image_pp(){

for(int i=0; i< num_img_train; i++){


images_train[i]=  (int)images_train[i] <127 ? 0 : 1;
  
}


}




int Predict(unsigned char *x){

    //W[j][i]=W[n*j+i] nxD
    /*
    calculate the term cj+\sum Wji xi
    */

    double *precomValues =(double *) malloc(sizeof(double)*n); 



    for(int j=0; j<n;j++){
    double sum=0;
        for(int i=0;i<D;i++){
            sum += W[j*n+i]*(double)x[i];
        }
        precomValues[j]=c[j]+sum;
    }

    /*
    calculate the product exp(dy)*\Prod (1+Exp(Ujy+cj+\sum Wji xi))
    */

    double *numerators =(double *) malloc(sizeof(double)*K); 

    for (int y = 0; y < K; y++) {
        double prod = 1;
        for (int j = 0; j < n; j++) {

            prod *= (1 + exp(U[j * n + y] + precomValues[j]));
        }
        prod = exp(d[y]) * prod;

        numerators[y] = prod;

    }

    /*
    calculate denominator
    */

    double denominator=0;
    for(int y=0;y<K;y++){
        denominator+=numerators[y];
    }
    /*
    return most probable class
    */
    //posible optimization, the denominator is not requiered to make the predictions
    int mostProClass=0;
    double max=numerators[0]/denominator;

    for(int y=1;y<K;y++){

        if(max<(numerators[y]/denominator)){
            mostProClass=y;
            max=(numerators[y]/denominator);
        }

    
    }


    free(precomValues);
    free(numerators);

    return mostProClass;

}


void init_param()
{
    W = (double *) malloc(n * D * sizeof(double));
    b = (double *) malloc(D * sizeof(double));
    c = (double *) malloc(n * sizeof(double));
    d = (double *) malloc(K * sizeof(double));
    U = (double *) malloc(n * K * sizeof(double));

    //The biases b ,c ,d are initiallez to zero 

    for(int i=0;i<D;i++){
        b[i]=0;
    }

    for(int i=0;i<n;i++){
        c[i]=0;
    }

    for(int i=0;i<K;i++){
        d[i]=0;
    }

    /*
    The matrices are initialized with a random number 
    in the range [-m^0.5,m^0.5] where m is the maximum between the number of rows and columns of the matrix.
    */

    double maxW = n<D?1.0/sqrt(D):1.0/sqrt(n);
    for(int i=0 ;i<n*D;i++){
        W[i]= (uniform()-0.5)*maxW;
    }

    double maxU = n<K?1.0/sqrt(K):1.0/sqrt(n);
    for(int i=0 ;i<n*K;i++){
        U[i]= (uniform()-0.5)*maxU;
    }
   

}