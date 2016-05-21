#include <stdio.h>
#include <math.h>
#include "emmintrin.h"
#include <immintrin.h>
#include <stdlib.h>
#include "tsc_x86.h"


//RANDOM LINE ADDED

//#define D 784      //Dimension
//#define n 6000        //Number of hidden layers
//#define lambda 0.005 //Training Rate
//#define K 10       //Number of classes

const int D = 784;
int n = 150;
const int K = 10;
const double lambda = 0.005;

//#define PERF_U_UPDATE
double mean = 0;
int count = 0;
double m2 = 0;
double delta;
#define FREQ 1.6e09

//Training Data
int *images_train;
unsigned char *images_train_init;
//Starting Pointer for all Images
int *labels_train; //labels in the dataset
int num_img_train; // Number of Images

//Test Data
int *images_test;
unsigned char *images_test_init;
//Starting Pointer for all Images
int *labels_test; //labels in the dataset
int num_img_test; // Number of Images

//Predictions
int *predictions;

// Training parameters
double *W; // Number of col= D, rows = n
double *b;
double *c;
double *d;
double *U; // Number of col= K, rows = n
double *h0_cap;



/*
 Definition of useful structure to check gibbs_Y and the prediction routines
 */
struct miniarr{
    double mr[K];
};
typedef struct miniarr Parr;



double energy_for_all();
void gibbs_H(int * h0, int y0, int *x0);
int gibbs_Y_(int* h0, double * U, double * d, int K, int n);
void gibbs_X_(int * x, int * h0, double * W, int D, int n);


double uniform()
{
    return (double)rand() / (double)(RAND_MAX);
}


int ReverseInt (int i)
{
    unsigned char ch1, ch2, ch3, ch4;
    ch1 = i & 255;
    ch2 = (i >> 8) & 255;
    ch3 = (i >> 16) & 255;
    ch4 = (i >> 24) & 255;
    return ((int) ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
}

void image_pp() {
    
    images_train = (int *)malloc(sizeof(int) * D * num_img_train);
    images_test = (int *)malloc(sizeof(int) * D * num_img_test);
    
    for (int i = 0; i < num_img_train * D; i++)
    images_train[i] =  (int)images_train_init[i] < 127 ? 0 : 1;
    
    
    for (int i = 0; i < num_img_test * D; i++)
    images_test[i] =  (int)images_test_init[i] < 127 ? 0 : 1;
    
}


void init_param()
{
    W = (double *) malloc(n * D * sizeof(double));
    b = (double *) malloc(D * sizeof(double));
    c = (double *) malloc(n * sizeof(double));
    d = (double *) malloc(K * sizeof(double));
    U = (double *) malloc(n * K * sizeof(double));
    h0_cap = (double *) malloc(sizeof(double) * n);
    
    //The biases b ,c ,d are initiallez to zero
    
    for (int i = 0; i < D; i++) {
        b[i] = 0;
    }
    
    for (int i = 0; i < n; i++) {
        c[i] = 0;
    }
    
    for (int i = 0; i < K; i++) {
        d[i] = 0;
    }
    
    /*
     The matrices are initialized with a random number
     in the range [-m^0.5,m^0.5] where m is the maximum between the number of rows and columns of the matrix.
     */
    
    double maxW = n < D ? 1.0 / sqrt(D) : 1.0 / sqrt(n);
    for (int i = 0 ; i < n * D; i++) {
        W[i] = (uniform() - 0.5) * maxW;
    }
    
    double maxU = n < K ? 1.0 / sqrt(K) : 1.0 / sqrt(n);
    for (int i = 0 ; i < n * K; i++) {
        U[i] = (uniform() - 0.5) * maxU;
    }
    
}


/*
 Load all images into memory
 Returns pointer to the first image
 */
void get_images(int *_num_img, unsigned char **_images,
                int **_labels, char *imagesFileName, char *labelsFileName)
{
    int num_img;
    unsigned char *images;
    int *labels;
    
    FILE *image_file = fopen(imagesFileName, "rb");
    FILE *label_file = fopen(labelsFileName, "rb");
    if (!image_file) {
        perror("Image file opening failed");
        exit (1);
    }
    if (!label_file) {
        perror("Label file opening failed");
        exit (1);
    }
    
    int magicNumber;
    int numberOfRows;
    int numberOfColumns;
    int labelNumberOfImages;
    int labelMagicNumber;
    int code;
    
    fseek(image_file, 0, SEEK_SET);
    fread(&magicNumber, sizeof(int), 1, image_file);
    fread(&num_img, sizeof(int), 1, image_file);
    fread(&numberOfRows, sizeof(int), 1, image_file);
    fread(&numberOfColumns, sizeof(int), 1, image_file);
    
    fseek(label_file, 0, SEEK_SET);
    fread(&labelMagicNumber, sizeof(int), 1, label_file);
    fread(&labelNumberOfImages, sizeof(int), 1, label_file);
    
    magicNumber = ReverseInt(magicNumber);
    num_img = ReverseInt(num_img);
    numberOfRows = ReverseInt(numberOfRows);
    numberOfColumns = ReverseInt(numberOfColumns);
    labelNumberOfImages = ReverseInt(labelNumberOfImages);
    labelMagicNumber = ReverseInt(labelMagicNumber);
    
    *_num_img = num_img;
    
    images = (unsigned char *) malloc(sizeof(unsigned char) * num_img * numberOfRows * numberOfColumns);
    unsigned char * temp_labels = (unsigned char *) malloc(sizeof(int) * num_img);
    labels = (int *) malloc(sizeof(int) * num_img);
    
    code = fread(images, sizeof(unsigned char), num_img * numberOfRows * numberOfColumns, image_file);
    
    if (code != num_img * numberOfRows * numberOfColumns) {
        perror("Image transfer failed");
        exit (1);
    }
    
    code = fread(temp_labels, sizeof(unsigned char), num_img, label_file);
    if (code != labelNumberOfImages) {
        perror("Label transfer failed");
        exit (1);
    }
    
    for (int i = 0; i < labelNumberOfImages; i++)
    {
        *(labels + i) = *(temp_labels + i);
        //printf("%d ", *(labels+i));
    }
    
    *_labels = labels;
    *_images = images;
    
    fclose(image_file);
    fclose(label_file);
    
}


/*
 Get one image from the loaded set of images
 Output is a pointer to that image
 */
int * get_image(int num, int *start)
{
    return start + num * D;
}

/*
 Gives us lable of the image n
 */
int get_label(int num, int * labels)
{
    return *(labels + num);
}


#include "helper.h" //DO NOT MOVE to be improved


/*
 *Sigmoid Function
 */
double sigmoid(double val)
{
    return 1.0 / (1.0 + exp(-1.0 * val));
}



/*
 * This function if more general then the previous one for h0_cap
 * h <- sigmoid( c + Wx + Uy )
 */
void h_update(double * h, int y0, int * x0, double * c,
              double * W, double * U, int n, int D, int K){
    
    for (int i = 0; i < n ; i++)
    {
        double sum = c[i];
        for (int j = 0; j < D; j++)
        {
            sum = sum + W[i * D + j] * x0[j];
        }
        
        
        //Can be optimized
        for (int j = 0; j < K; j++)
        {
            if (j == y0) sum = sum + U[i * K + j];
        }
        
        h[i] = sigmoid(sum);
    }
    
}

// TODO
double * img_tmp_mem0;
double * img_tmp_mem1;

void W_update(double * W, double * h0_cap, double * h1_cap,
              double * x0, double * x1, double lambda, int n, int D){
    // Scalar replacement
    // Loop unrolling: by 2

    double m1, m2, m3, m4, m5, m6, m7, m8;
    double n1, n2, n3, n4, n5, n6, n7, n8;
    double s1, s2, s3, s4, s5, s6, s7, s8;
    double t1, t2, t3, t4, t5, t6, t7, t8;
    double l1, l2, l3, l4, l5, l6, l7, l8;

    __m128d w1, w2, w3, w4, w5, w6, w7, w8;
    __m128d tmp11, tmp12, tmp13, tmp14, tmp21, tmp22, tmp23, tmp24;
    __m128d tmp31, tmp32, tmp33, tmp34, tmp41, tmp42, tmp43, tmp44;
    __m128d x01d, x02d, x03d, x04d, x11d, x12d, x13d, x14d;
    __m128d x05d, x06d, x07d, x08d, x15d, x16d, x17d, x18d;
    double tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;
    double h0scalar_lambda, h1scalar_lambda;
    double h1scalar_lambda1, h1scalar_lambda2, h1scalar_lambda3, h1scalar_lambda4, h1scalar_lambda5, h1scalar_lambda6, h1scalar_lambda7, h1scalar_lambda8;
    double h0scalar_lambda1, h0scalar_lambda2, h0scalar_lambda3, h0scalar_lambda4, h0scalar_lambda5, h0scalar_lambda6, h0scalar_lambda7, h0scalar_lambda8;

    int iD;
    for (int i = 0; i < n; i++)
    {
        iD = i * D;
        // Store h0 and h1 into variable, and multiply by lambda
        // so that we don't multiply inside of the inner loop
        h0scalar_lambda = lambda * h0_cap[i];
        h1scalar_lambda = lambda * h1_cap[i]; 


        h1scalar_lambda1 = h1scalar_lambda;
        h1scalar_lambda2 = h1scalar_lambda;
        h1scalar_lambda3 = h1scalar_lambda;
        h1scalar_lambda4 = h1scalar_lambda;
        h1scalar_lambda5 = h1scalar_lambda;
        h1scalar_lambda6 = h1scalar_lambda;
        h1scalar_lambda7 = h1scalar_lambda;
        h1scalar_lambda8 = h1scalar_lambda;

        h0scalar_lambda1 = h0scalar_lambda;
        h0scalar_lambda2 = h0scalar_lambda;
        h0scalar_lambda3 = h0scalar_lambda;
        h0scalar_lambda4 = h0scalar_lambda;
        h0scalar_lambda5 = h0scalar_lambda;
        h0scalar_lambda6 = h0scalar_lambda;
        h0scalar_lambda7 = h0scalar_lambda;
        h0scalar_lambda8 = h0scalar_lambda;

        __m128d h0sc = _mm_load_sd(&h0scalar_lambda);
        __m128d h1sc = _mm_load_sd(&h1scalar_lambda);
        
        for (int j = 0; j < D; j+=12)
        {

            l1 = W[iD+j+8];
            l2 = W[iD+j+9];
            l3 = W[iD+j+10];
            l4 = W[iD+j+11];

            w1 = _mm_load_sd(W + iD + j);
            w2 = _mm_load_sd(W + iD + j + 1);
            w3 = _mm_load_sd(W + iD + j + 2);
            w4 = _mm_load_sd(W + iD + j + 3);
            w5 = _mm_load_sd(W + iD + j + 4);
            w6 = _mm_load_sd(W + iD + j + 5);
            w7 = _mm_load_sd(W + iD + j + 6);
            w8 = _mm_load_sd(W + iD + j + 7);

            x01d = _mm_load_sd(x0 + j);
            x11d = _mm_load_sd(x1 + j);
            x02d = _mm_load_sd(x0 + j + 1);
            x12d = _mm_load_sd(x1 + j + 1);
            x03d = _mm_load_sd(x0 + j + 2);
            x13d = _mm_load_sd(x1 + j + 2);
            x04d = _mm_load_sd(x0 + j + 3);
            x14d = _mm_load_sd(x1 + j + 3);

            x05d = _mm_load_sd(x0 + j + 4);
            x15d = _mm_load_sd(x1 + j + 4);
            x06d = _mm_load_sd(x0 + j + 5);
            x16d = _mm_load_sd(x1 + j + 5);
            x07d = _mm_load_sd(x0 + j + 6);
            x17d = _mm_load_sd(x1 + j + 6);
            x08d = _mm_load_sd(x0 + j + 7);
            x18d = _mm_load_sd(x1 + j + 7);

            m1 = h1scalar_lambda1 * x1[j+8];
            tmp11 = _mm_fmsub_pd(h1sc, x11d, w1);
            tmp12 = _mm_fmsub_pd(h1sc, x12d, w2);
            m2 = h1scalar_lambda2 * x1[j+9]; 
            s1 = m1 - l1;              
            tmp13 = _mm_fmsub_pd(h1sc, x13d, w3);
            tmp14 = _mm_fmsub_pd(h1sc, x14d, w4);
            m3 = h1scalar_lambda3 * x1[j+10];
            s2 = m2 - l2;  
            tmp31 = _mm_fmsub_pd(h1sc, x15d, w5);
            tmp32 = _mm_fmsub_pd(h1sc, x16d, w6);
            m4 = h1scalar_lambda4 * x1[j+11];
            s3 = m3 - l3; 
            tmp33 = _mm_fmsub_pd(h1sc, x17d, w7);
            tmp34 = _mm_fmsub_pd(h1sc, x18d, w8);
            n1 = h0scalar_lambda1 * x0[j+8];
            s4 = m4 - l4;   
            tmp21 = _mm_fmsub_pd(h0sc, x01d, tmp11);
            tmp22 = _mm_fmsub_pd(h0sc, x02d, tmp12);
            n2 = h0scalar_lambda2 * x0[j+9];
            s1 = n1 - s1; 
            tmp23 = _mm_fmsub_pd(h0sc, x03d, tmp13);
            tmp24 = _mm_fmsub_pd(h0sc, x04d, tmp14);
            n3 = h0scalar_lambda3 * x0[j+10];
            s2 = n2 - s2; 
            tmp41 = _mm_fmsub_pd(h0sc, x05d, tmp31);
            tmp42 = _mm_fmsub_pd(h0sc, x06d, tmp32);
            n4 = h0scalar_lambda4 * x0[j+11];
            s3 = n3 - s3;              
            tmp43 = _mm_fmsub_pd(h0sc, x07d, tmp33);
            tmp44 = _mm_fmsub_pd(h0sc, x08d, tmp34);             
            s3 = n3 - s3;    
            s4 = n4 - s4;    

            
            // Note: The computation is done by the following:
            // Original: W[iD+j] = W[iD+j] + (h0scalar_lambda * x0 - h1scalar_lambda * x1);
            // Rearranged: W[iD+j] = h0scalar_lambda * x0 - (h1scalar_lambda * x1 - W[iD+j]);

            _mm_store_sd(W + iD + j, tmp21);
            _mm_store_sd(W + iD + j + 1, tmp22);
            _mm_store_sd(W + iD + j + 2, tmp23);
            _mm_store_sd(W + iD + j + 3, tmp24);

            _mm_store_sd(W + iD + j + 4, tmp41);
            _mm_store_sd(W + iD + j + 5, tmp42);
            _mm_store_sd(W + iD + j + 6, tmp43);
            _mm_store_sd(W + iD + j + 7, tmp44);


            //W[i * D + j] = W[i * D + j] + lambda * (h0_cap[i] * x0[j] - h1_cap[i] * x1[j])             
                 

            W[iD+j+8] = s1;
            W[iD+j+9] = s2;
            W[iD+j+10] = s3;
            W[iD+j+11] = s4;


        }
        // We don't need additional patch, because D mod 8 = 0
    }
}

void b_update(double * b, int * x0, int * x1, double lambda, int D){
    for (int i = 0; i < D; i++)
    {
        b[i] = b[i] + lambda * (x0[i] - x1[i]);
    }
}

void c_update(double * c, double * h0_cap, double * h1_cap, double lambda, int n){
    for (int i = 0; i < n; i++)
    {
        c[i] = c[i] + lambda * (h0_cap[i] - h1_cap[i]);
    }
}

void d_update(double * d, int y0, int y1, double lambda, int K){
    for (int i = 0; i < K; i++)
    {
        if (i == y0)
        d[i] = d[i] + lambda;
        
        if (i == y1)
        d[i] = d[i] - lambda;
    }
}

void U_update(double * U, double * h0_cap, double * h1_cap,
              int y0, int y1, double lambda, int n, int K){
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < K; j++)
        {
            if (j == y0)
            U[i * K + j] = U[i * K + j] + lambda * (h0_cap[i]);
            
            if (j == y1)
            U[i * K + j] = U[i * K + j] + lambda * (-h1_cap[i]);
        }
    }
}


/*
 Performs next step of training with a new image and it's class
 */
void COD_training_update(int yi, int * xi, double * h0_cap,
                         double * b, double * c, double * d,
                         double * U, double * W, int n, int D, int K) //DONE
{
    img_tmp_mem0 = (double*)malloc(sizeof(double) * D);
    img_tmp_mem1 = (double*)malloc(sizeof(double) * D);



    //Positive Phase
    int y0 = yi;
    int * x0 = xi;
    
    //double * h0_cap = (double *) malloc(sizeof(double) * n);
    
    // Here we update h0_cap <- sigmoid( c + Wx0 + Uy0 )
#ifdef PERF_H0_UPDATE
    myInt64 start;
    start = start_tsc();
#endif
    h_update(h0_cap, y0, x0, c, W, U, n, D, K);
#ifdef PERF_H0_UPDATE
    myInt64 cycles = stop_tsc(start);
    count = count + 1;
    delta = cycles - mean;
    mean += delta/(double)count;
    m2 += delta*(cycles - mean);
#endif
    
    
    //Negative Phase
    int * h0 = (int *) malloc(sizeof(int) * n);
    int * x1 = (int *) malloc(sizeof(int) * D);
    double * h1_cap = (double *) malloc(sizeof(double) * n);
    
    // Compute Gibbs samplings for h0, y1 and x1
#ifdef PERF_GIBBS_H
    myInt64 start;
    start = start_tsc();
#endif
    gibbs_H(h0, y0, x0);
#ifdef PERF_GIBBS_H
    myInt64 cycles = stop_tsc(start);
    count = count + 1;
    delta = cycles - mean;
    mean += delta/(double)count;
    m2 += delta*(cycles - mean);
#endif
    
    
    
#ifdef PERF_GIBBS_Y
    myInt64 start;
    start = start_tsc();
#endif
    int y1 = gibbs_Y_(h0, U, d, K, n);
#ifdef PERF_GIBBS_Y
    myInt64 cycles = stop_tsc(start);
    count = count + 1;
    delta = cycles - mean;
    mean += delta/(double)count;
    m2 += delta*(cycles - mean);
#endif
    
    
    
#ifdef PERF_GIBBS_X
    myInt64 start;
    start = start_tsc();
#endif
    gibbs_X_(x1, h0, W, D, n);
#ifdef PERF_GIBBS_X
    myInt64 cycles = stop_tsc(start);
    count = count + 1;
    delta = cycles - mean;
    mean += delta/(double)count;
    m2 += delta*(cycles - mean);
#endif
    
    
    
    
    // Here we update h1_cap <- sigmoid( c + Wx1 + Uy1 )
#ifdef PERF_H1_UPDATE
    myInt64 start;
    start = start_tsc();
#endif
    h_update(h1_cap, y1, x1, c, W, U, n, D, K);
#ifdef PERF_H1_UPDATE
    myInt64 cycles = stop_tsc(start);
    count = count + 1;
    delta = cycles - mean;
    mean += delta/(double)count;
    m2 += delta*(cycles - mean);
#endif
    
        // TODO
    for (int i = 0; i < D; i++){
        img_tmp_mem0[i] = (double)x0[i];
        img_tmp_mem1[i] = (double)x1[i];
    }
    
    //Update Phase
#ifdef PERF_W_UPDATE
    myInt64 start;
    start = start_tsc();
#endif
    W_update(W, h0_cap, h1_cap, img_tmp_mem0, img_tmp_mem1, lambda, n, D);
#ifdef PERF_W_UPDATE
    myInt64 cycles = stop_tsc(start);
    count = count + 1;
    delta = cycles - mean;
    mean += delta/(double)count;
    m2 += delta*(cycles - mean);
#endif
    
    
    
#ifdef PERF_B_UPDATE
    myInt64 start;
    start = start_tsc();
#endif
    b_update(b, x0, x1, lambda, n);
#ifdef PERF_B_UPDATE
    myInt64 cycles = stop_tsc(start);
    count = count + 1;
    delta = cycles - mean;
    mean += delta/(double)count;
    m2 += delta*(cycles - mean);
#endif
    
    
#ifdef PERF_C_UPDATE
    myInt64 start;
    start = start_tsc();
#endif
    c_update(c, h0_cap, h1_cap, lambda, n);
#ifdef PERF_C_UPDATE
    myInt64 cycles = stop_tsc(start);
    count = count + 1;
    delta = cycles - mean;
    mean += delta/(double)count;
    m2 += delta*(cycles - mean);
#endif
    
    
    
#ifdef PERF_D_UPDATE
    myInt64 start;
    start = start_tsc();
#endif
    d_update(d, y0, y1, lambda, K);
#ifdef PERF_D_UPDATE
    myInt64 cycles = stop_tsc(start);
    count = count + 1;
    delta = cycles - mean;
    mean += delta/(double)count;
    m2 += delta*(cycles - mean);
#endif
    
    
#ifdef PERF_U_UPDATE
    myInt64 start;
    start = start_tsc();
#endif
    U_update(U, h0_cap, h1_cap, y0, y1, lambda, n, K);
#ifdef PERF_U_UPDATE
    myInt64 cycles = stop_tsc(start);
    count = count + 1;
    delta = cycles - mean;
    mean += delta/(double)count;
    m2 += delta*(cycles - mean);
#endif
    
    //free(h0_cap);
    free(h1_cap);
    free(x1);
    free(h0);
    
}

/*
 Runs CODTrainingUpdate over all images and trains the model
 */
void COD_train()
{
    int i;
    for (i = 0; i < num_img_train; i++)
    {
        //printf("training image %d \n ", i + 1);
#ifdef PERF_COD_TRAIN_UPDATE
        myInt64 start;
        start = start_tsc();
#endif
        COD_training_update(get_label(i, labels_train), get_image(i, images_train),
                            h0_cap,
                            b, c, d,
                            U, W, n, D, K);
#ifdef PERF_COD_TRAIN_UPDATE
        myInt64 cycles = stop_tsc(start);
        count = count + 1;
        delta = cycles - mean;
        mean += delta/(double)count;
        m2 += delta*(cycles - mean);
#endif
        
        // if(i%500==0)printf("training image %d \n ", i + 1);
        
        if(i==0)
        {
            //printf(" Energy:%lf\n", energy_for_all());
            //fflush(stdout);
        }
    }
}

double energy(int y, int *x, double * h, double * W,
              double * b, double * c, double * d, double * U,
              int n, int D, int K)
{
    double sum = 0;
    double tmp;
    
    
    // sum += h'Wx
    for(int i=0;i<n;i++)
    {
        // Matrix line with vector multiplication
        tmp = 0;
        for(int j=0;j<D;j++)
        {
            tmp += W[i*D+j] * x[j];
        }
        
        sum += tmp * h[i];
    }
    
    // sum += b'x
    for(int i=0; i<D ;i++)
    {
        sum += b[i] * x[i];
    }
    
    // sum += c'h
    for(int i=0; i<n;i++)
    {
        sum += c[i] * h[i];
    }
    
    // sum += d'y
    sum += d[y];
    
    // sum += h'Uy
    for(int i=0;i<n;i++)
    {
        sum += U[i*K+y] * h[i];
    }
    
    return -sum;
}


double energy_for_all()
{
    double sum = 0;
    
    for(int k=0; k<num_img_train ; k++)
    {
        //printf("energy image %d \n ", k);
        
        double e = energy(get_label(k, labels_train), get_image(k, images_train),
                          h0_cap, W, b, c, d, U, n, D, K);
        
        sum += e;
    }
    
    return sum;
}


/*
 Gibbs samplings for H (hidden nodes)
 Modifies h0 gibbs_H(h0, y0, x0);
 */
void gibbs_H(int * h0, int y0, int *x0)
{
    for (int j = 0; j < n; j++)
    {
        double sum = c[j] + U[j * K + y0];
        for (int i = 0; i < D; i++)
        {
            sum = sum + W[j * D + i] * x0[i];
        }
        
        double p = sigmoid(sum);
        double rand = uniform();
        if (rand < p)
        {
            h0[j] = 1;
        }
        else
        {
            h0[j] = 0;
        }
    }
}

/*
 Gibbs samplings for Y (label)
 Returns value for Y
 */
int gibbs_Y_(int* h0, double * U, double * d, int K, int n)
{
    double sum = 0;
    double *y_temp = (double *) malloc(sizeof(double) * K);
    for (int i = 0; i < K; i++)
    {
        double val = d[i];
        for (int j = 0; j < n; j++)
        {
            val = val + U[j * K + i] * h0[j];
        }
        y_temp[i] = exp(val);
        sum = sum + y_temp[i];
    }
    
    for (int i = 0; i < K; i++)
    {
        y_temp[i] = y_temp[i] / sum;
    }
    
    double rand_n = uniform();
    sum = 0;
    
    
    for (int i = 0; i < K; i++)
    {
        sum = sum + y_temp[i];
        if (sum >= rand_n)
        {
            free(y_temp);
            return i;
        }
    }
    
    
    free(y_temp);
    return K - 1;
    
    
}


/*
 Gibbs samplings for X (image)
 Modifies x
 */
void gibbs_X_(int * x, int * h0, double * W, int D, int n)
{
    for (int i = 0; i < D; i++)
    {
        double sum = b[i];
        for (int j = 0; j < n ; j++)
        {
            sum = sum + W[j * D + i] * h0[j];
        }
        
        double p = sigmoid(sum);
        double rand_n = uniform();
        if (rand_n < p)
        {
            x[i] = 1;
        }
        else
        {
            x[i] = 0;
        }
    }
}

