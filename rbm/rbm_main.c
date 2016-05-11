//
//  rbm_main.c
//  
//
//  Created by Prabhakaran on 11/05/16.
//
//

#include "rbm.h"


long double my_log(double imput){

if (imput > 99)
    {
        return imput; //+ log(1+exp(-imput));
    }
    else
    {
         return log(1+exp(imput));
    }

}

int Predict_one_image(int *x) {
    
    //W[j][i]=W[n*j+i] nxD
    /*
     calculate the term cj+\sum Wji xi
     */
    
    double *precomValues = (double *) malloc(sizeof(double) * n);
    
    
    
    for (int j = 0; j < n; j++) {
        double sum = 0;
        for (int i = 0; i < D; i++) {
            sum += W[j * D + i] * (double)x[i];
        }
        precomValues[j] = c[j] + sum;
    }
    
    /*
     calculate the product exp(dy)*\Prod (1+Exp(Ujy+cj+\sum Wji xi))
     */
    
    double *numerators = (double *) malloc(sizeof(double) * K);
    
    for (int y = 0; y < K; y++) {
        double prod = 0;
        for (int j = 0; j < n; j++) {
            
            //prod += log((1 + exp(U[j * K + y] + precomValues[j])));
            prod += my_log(U[j * K + y] + precomValues[j]);
            //prod += U[j * K + y] + precomValues[j];
            
            //printf("NUM: %f,  %f,  %f\n", U[j * K + y], precomValues[j], exp(U[j * K + y] + precomValues[j])   );
        }
        //exit(0); // REMOVE
        prod = d[y] + prod;
        
        numerators[y] = prod;
        
    }
    
    /*
     calculate denominator
     */
    
    // double denominator=0;
    // for(int y=0;y<K;y++){
    //     denominator+=numerators[y];
    // }
    /*
     return most probable class
     */
    //posible optimization, the denominator is not requiered to make the predictions
    int mostProClass = 0;
    double max = numerators[0];
    
    for (int y = 1; y < K; y++) {
        
        if (max < (numerators[y])) {
            mostProClass = y;
            max = (numerators[y]);
        }
        
        
    }
    
    
    // for (int i = 0; i<K; i++){
    //     printf("%f  ", numerators[i]);
    // }
    // printf("\nmax: %d\n", mostProClass);
    
    free(precomValues);
    free(numerators);
    
    return mostProClass;
    
}

void predict_images () {
    
    predictions = (int *)malloc(sizeof(int) * num_img_test);
    for (int i = 0; i < num_img_test; i++) {
        //printf("predicting image %d\n", i + 1);
        predictions[i] = Predict_one_image(get_image(i, images_test));
        
    }
    
}


double score_function() {
    
    double score = 0;
    for (int i = 0; i < num_img_test; i++) {
        if (predictions[i] != labels_test[i])
        {
            score++;
        }
    }
    
    return 100 * score / ((double) num_img_test);
    
}


void save_parameters ()
{
    
    
    // char *name = "./Wparam_v_";
    // int num =11221230;
    // char *numstr = (char *) malloc ((int)((ceil(log10(num))+1)*sizeof(char))) ;
    // sprintf(numstr, "%d", num);
    // char *extension = ".bin";
    // char *fullname = (char *) malloc(sizeof(char)*(15+(int)(ceil(log10(num))+1)));
    
    // strcpy(fullname,name);
    // strcat(fullname,numstr);
    // strcat(fullname,extension);
    
    // printf("my string %s ",fullname);
    
    FILE *writeW;
    writeW = fopen("./Wparam2.bin","wb");
    fwrite(W,sizeof(double),n*D,writeW);
    fclose(writeW);
    
    FILE *writeb;
    writeb = fopen("./bparam2.bin","wb");
    fwrite(b,sizeof(double),D,writeb);
    fclose(writeb);
    
    FILE *writec;
    writec = fopen("./cparam2.bin","wb");
    fwrite(c,sizeof(double),n,writec);
    fclose(writec);
    
    FILE *writed;
    writed = fopen("./dparam2.bin","wb");
    fwrite(d,sizeof(double),K,writed);
    fclose(writed);
    
    FILE *writeU;
    writeU = fopen("./Uparam2.bin","wb");
    fwrite(U,sizeof(double),n*K,writeU);
    fclose(writeU);
}

void read_parameters(){
    
    FILE *readW;
    readW = fopen("./Wparam.bin","rb");
    fread(W,sizeof(double),n*D,readW);
    fclose(readW);
    
    FILE *readb;
    readb = fopen("./bparam.bin","rb");
    fread(b,sizeof(double),D,readb);
    fclose(readb);
    
    FILE *readc;
    readc = fopen("./cparam.bin","rb");
    fread(c,sizeof(double),n,readc);
    fclose(readc);
    
    FILE *readd;
    readd = fopen("./dparam.bin","rb");
    fread(d,sizeof(double),K,readd);
    fclose(readd);
    
    FILE *readU;
    readU = fopen("./Uparam.bin","rb");
    fread(U,sizeof(double),n*K,readU);
    fclose(readU);
    
    
}

#ifndef UNITTEST_MODE

int main(int argc, char * argv[])
{
    srand(1);
    
    if(argc > 1) n = atoi(argv[1]);
    
    get_images(&num_img_train, &images_train_init, &labels_train,
               "../MNISTDataSet/train-images-idx3-ubyte", "../MNISTDataSet/train-labels-idx1-ubyte");
    
    num_img_train = 2500;  // REMOVE
    get_images(&num_img_test, &images_test_init, &labels_test,
               "../MNISTDataSet/t10k-images-idx3-ubyte", "../MNISTDataSet/t10k-labels-idx1-ubyte");
    num_img_test = 200; // REMOVE
    image_pp();
    
    //check_images_labels(300, 222); // PROBLEM, doesn't work
    
    init_param();
    
    //exit(0);
    //num_img_train=1000;
    //num_img_test=200;
    //read_parameters();
    //printf(" DONE Initializing W b c d and U\n");
    fflush(stdout);
    int T=0;
    
    while(T<4)
    {
        //printf("Traing time %d \n",T++);
        
        
#ifdef PERF_COD_TRAIN
        myInt64 start;
        start = start_tsc();
#endif
        COD_train();
#ifdef PERF_COD_TRAIN
        myInt64 cycles = stop_tsc(start);
        count = count + 1;
        delta = cycles - mean;
        mean += delta/(double)count;
        m2 += delta*(cycles - mean);
#endif
        predict_images ();
        double score = score_function();
        dprintf(2,"Errors %d %lf \n", T, score);
        //save_parameters ();
        T++;
    }
    printf("%d %lf %lf %lf %lf\n", n, mean, sqrt(m2/(double)(count-1)), mean/FREQ, sqrt(m2/(double)(count-1))/FREQ);
    
    return 0;
}

#endif
