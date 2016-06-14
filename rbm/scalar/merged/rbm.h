#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "emmintrin.h"
#include <immintrin.h>
#include "tsc_x86.h"

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

//Starting Pointer for all Test
int *labels_test; //labels in the dataset
int num_img_test; // Number of Test

//Predictions
int *predictions;

// Training parameters
double *W; // Number of col= D, rows = n
double *b;
double *c;
double *d;
double *U; // Number of col= n, rows = K
double *h0_cap;
int * h0;
int * x1;
double * h1_cap;
int * x0;

double energy_for_all();
void gibbs_H(int y0);
int gibbs_Y_();
void gibbs_X_(int * x);

double uniform() {
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


/* Translate image to binary representation */
void image_pp() {

    images_train = (int *)malloc(sizeof(int) * D * num_img_train);
    images_test = (int *)malloc(sizeof(int) * D * num_img_test);

    for (int i = 0; i < num_img_train * D; i++)
        images_train[i] =  (int)images_train_init[i] < 127 ? 0 : 1;

    for (int i = 0; i < num_img_test * D; i++)
        images_test[i] =  (int)images_test_init[i] < 127 ? 0 : 1;
}


/* Initialize parameters */
void init_param() {

    W = (double *) malloc(n * D * sizeof(double));
    b = (double *) malloc(D * sizeof(double));
    c = (double *) malloc(n * sizeof(double));
    d = (double *) malloc(K * sizeof(double));
    U = (double *) malloc(n * K * sizeof(double));
    h0_cap = (double *) malloc(sizeof(double) * n);
    h0 = (int *) malloc(sizeof(int) * n);
    x1 = (int *) malloc(sizeof(int) * D);
    h1_cap = (double *) malloc(sizeof(double) * n);

    //The biases b ,c ,d are initiallez to zero
    for (int i = 0; i < D; i++) b[i] = 0;
    for (int i = 0; i < n; i++) c[i] = 0;
    for (int i = 0; i < K; i++) d[i] = 0;

    /*
     The matrices are initialized with a random number
     in the range [-m^0.5,m^0.5] where m is the maximum between the number of rows and columns of the matrix.
     */

    double maxW = n < D ? 1.0 / sqrt(D) : 1.0 / sqrt(n);
    for (int i = 0 ; i < n * D; i++) {
        W[i] = (uniform() - 0.5) * maxW;
    }

    double maxU = n < K ? 1.0 / sqrt(K) : 1.0 / sqrt(n);
    double *U_temp = (double *) malloc(n * K * sizeof(double));;
    for (int i = 0 ; i < n * K; i++) {
        U_temp[i] = (uniform() - 0.5) * maxU;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < K; j++) {
            U[j * n + i] = U_temp[i * K + j];
        }
    }
}


/*  Load all images into memory. Returns pointer to the first image */
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
        *(labels + i) = *(temp_labels + i);

    *_labels = labels;
    *_images = images;

    fclose(image_file);
    fclose(label_file);
}


/* Get one image from the loaded set of images. Output is a pointer to that image */
int * get_image(int num, int *start) {
    return start + num * D;
}

/* Gives us lable of the image n  */
int get_label(int num, int * labels)
{
    return *(labels + num);
}

#include "helper.h" //DO NOT MOVE


double sigmoid(double val) {
    return 1.0 / (1.0 + exp(-1.0 * val));
}


/*
 * This function if more general then the previous one for h0_cap
 * h <- sigmoid( c + Wx + Uy )
 */
void h_update(double * h, int y0, double * x0) {

    double sum;
    __m128d s1, s2, s3, s4;
    __m128d w1, w2, w3, w4;
    __m128d w5, w6, w7, w8;
    __m128d x1, x2, x3, x4;
    __m128d x5, x6, x7, x8;

    double sum2;

    for (int i = 0; i < n ; i++)
    {
        __m128d s1 = {0};
        __m128d s2 = {0};
        __m128d s3 = {0};
        __m128d s4 = {0};
        __m128d s5 = {0};
        __m128d s6 = {0};
        __m128d s7 = {0};
        __m128d s8 = {0};
        int iD = i * D;
        for (int j = 0; j < D; j += 8)
        {

            w1 = _mm_load_sd(W + iD + j);
            w2 = _mm_load_sd(W + iD + j + 1);
            w3 = _mm_load_sd(W + iD + j + 2);
            w4 = _mm_load_sd(W + iD + j + 3);
            w5 = _mm_load_sd(W + iD + j + 4);
            w6 = _mm_load_sd(W + iD + j + 5);
            w7 = _mm_load_sd(W + iD + j + 6);
            w8 = _mm_load_sd(W + iD + j + 7);

            x1 = _mm_load_sd(x0 + j);
            x2 = _mm_load_sd(x0 + j + 1);
            x3 = _mm_load_sd(x0 + j + 2);
            x4 = _mm_load_sd(x0 + j + 3);
            x5 = _mm_load_sd(x0 + j + 4);
            x6 = _mm_load_sd(x0 + j + 5);
            x7 = _mm_load_sd(x0 + j + 6);
            x8 = _mm_load_sd(x0 + j + 7);


            s1 = _mm_fmadd_sd(w1, x1, s1);
            s2 = _mm_fmadd_sd(w2, x2, s2);
            s3 = _mm_fmadd_sd(w3, x3, s3);
            s4 = _mm_fmadd_sd(w4, x4, s4);
            s5 = _mm_fmadd_sd(w5, x5, s5);
            s6 = _mm_fmadd_sd(w6, x6, s6);
            s7 = _mm_fmadd_sd(w7, x7, s7);
            s8 = _mm_fmadd_sd(w8, x7, s8);

        }
        s1 = _mm_add_sd(s1, s2);
        s2 = _mm_add_sd(s3, s4);
        s1 = _mm_add_sd(s1, s3);
        s5 = _mm_add_sd(s5, s6);
        s7 = _mm_add_sd(s7, s8);
        s5 = _mm_add_sd(s5, s7);
        s1 = _mm_add_sd(s1, s5);
        _mm_store_sd(&sum2, s1);
        sum = c[i] + sum2;

        sum = sum + U[y0 * n + i];

        h[i] = sigmoid(sum);
    }
}

double * img_tmp_mem0;
double * img_tmp_mem1;

void W_update(double * x0, double * x1)
{
    __m128d w1, w2, w3, w4;
    __m128d tmp11, tmp12, tmp13, tmp14, tmp21, tmp22, tmp23, tmp24;
    __m128d x01d, x02d, x03d, x04d, x11d, x12d, x13d, x14d;
    double h0scalar_lambda, h1scalar_lambda;
    int iD;
    for (int i = 0; i < n; i++)
    {
        iD = i * D;
        // Store h0 and h1 into variable, and multiply by lambda
        // so that we don't multiply inside of the inner loop
        h0scalar_lambda = lambda * h0_cap[i];
        h1scalar_lambda = lambda * h1_cap[i];
        __m128d h0sc = _mm_load_sd(&h0scalar_lambda);
        __m128d h1sc = _mm_load_sd(&h1scalar_lambda);

        for (int j = 0; j < D; j += 4)
        {

            w1 = _mm_load_sd(W + iD + j);
            w2 = _mm_load_sd(W + iD + j + 1);
            w3 = _mm_load_sd(W + iD + j + 2);
            w4 = _mm_load_sd(W + iD + j + 3);

            x01d = _mm_load_sd(x0 + j);
            x11d = _mm_load_sd(x1 + j);
            x02d = _mm_load_sd(x0 + j + 1);
            x12d = _mm_load_sd(x1 + j + 1);
            x03d = _mm_load_sd(x0 + j + 2);
            x13d = _mm_load_sd(x1 + j + 2);
            x04d = _mm_load_sd(x0 + j + 3);
            x14d = _mm_load_sd(x1 + j + 3);


            tmp11 = _mm_fmsub_pd(h1sc, x11d, w1);
            tmp12 = _mm_fmsub_pd(h1sc, x12d, w2);
            tmp13 = _mm_fmsub_pd(h1sc, x13d, w3);
            tmp14 = _mm_fmsub_pd(h1sc, x14d, w4);

            tmp21 = _mm_fmsub_pd(h0sc, x01d, tmp11);
            tmp22 = _mm_fmsub_pd(h0sc, x02d, tmp12);
            tmp23 = _mm_fmsub_pd(h0sc, x03d, tmp13);
            tmp24 = _mm_fmsub_pd(h0sc, x04d, tmp14);

            // Note: The computation is done by the following:
            // Original: W[iD+j] = W[iD+j] + (h0scalar_lambda * x0 - h1scalar_lambda * x1);
            // Rearranged: W[iD+j] = h0scalar_lambda * x0 - (h1scalar_lambda * x1 - W[iD+j]);

            _mm_store_sd(W + iD + j, tmp21);
            _mm_store_sd(W + iD + j + 1, tmp22);
            _mm_store_sd(W + iD + j + 2, tmp23);
            _mm_store_sd(W + iD + j + 3, tmp24);

        }
        // We don't need additional patch, because D mod 8 = 0
    }
}

void b_update()
{
    for (int i = 0; i < D; i++)
        b[i] = b[i] + lambda * (x0[i] - x1[i]);
}

void c_update()
{
    for (int i = 0; i < n; i++)
        c[i] = c[i] + lambda * (h0_cap[i] - h1_cap[i]);
}

void d_update(int y0, int y1)
{
    if (y0 != y1) 
    {
        d[y0] = d[y0] + lambda;
        d[y1] = d[y1] - lambda;
    }
}

void U_update(int y0, int y1)
{
    if (y0 != y1)
    {
        for (int i = 0; i < n; i += 2)
        {
            double u00 = U[y0 * n + i];
            double u01 = U[y0 * n + i + 1];
            double u10 = U[y1 * n + i];
            double u11 = U[y1 * n + i + 1];

            u00 = u00 + lambda * (h0_cap[i]);
            u10 = u10 + lambda * (-h1_cap[i]);
            u01 = u01 + lambda * (h0_cap[i + 1]);
            u11 = u11 + lambda * (-h1_cap[i + 1]);

            U[y0 * n + i] = u00;
            U[y1 * n + i] = u10;
            U[y0 * n + i + 1] = u01;
            U[y1 * n + i + 1] = u11;
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            U[y0 * n + i] = U[y0 * n + i] + lambda * (h0_cap[i])
                            + lambda * (-h1_cap[i]);
        }
    }
}


/*
 Performs next step of training with a new image and it's class
 */
void COD_training_update(int yi, int * xi, double * h0_cap,
                         double * b, double * c, double * d,
                         double * U, double * W, int n, int D, int K)
{
    img_tmp_mem0 = (double*)malloc(sizeof(double) * D);
    img_tmp_mem1 = (double*)malloc(sizeof(double) * D);

    // Positive Phase
    int y0 = yi;
    x0 = xi;

    // Here we update h0_cap <- sigmoid( c + Wx0 + Uy0 )

    double* x0d = (double*)malloc(sizeof(double) * n);
    for (int i = 0; i < n; i++)
        x0d[i] = (double)x0[i];
    
    #ifdef PERF_H0_UPDATE
        myInt64 start;
        start = start_tsc();
    #endif
        h_update(h0_cap, y0, x0d);
    #ifdef PERF_H0_UPDATE
        myInt64 cycles = stop_tsc(start);
        count = count + 1;
        delta = cycles - mean;
        mean += delta / (double)count;
        m2 += delta * (cycles - mean);
    #endif



    // Negative Phase
    // Compute Gibbs samplings for h0, y1 and x1
    #ifdef PERF_GIBBS_H
        myInt64 start;
        start = start_tsc();
    #endif
        gibbs_H(y0);
    #ifdef PERF_GIBBS_H
        myInt64 cycles = stop_tsc(start);
        count = count + 1;
        delta = cycles - mean;
        mean += delta / (double)count;
        m2 += delta * (cycles - mean);
    #endif



    #ifdef PERF_GIBBS_Y
        myInt64 start;
        start = start_tsc();
    #endif
        int y1 = gibbs_Y_();
    #ifdef PERF_GIBBS_Y
        myInt64 cycles = stop_tsc(start);
        count = count + 1;
        delta = cycles - mean;
        mean += delta / (double)count;
        m2 += delta * (cycles - mean);
    #endif



    #ifdef PERF_GIBBS_X
        myInt64 start;
        start = start_tsc();
    #endif
        gibbs_X_(x1);
    #ifdef PERF_GIBBS_X
        myInt64 cycles = stop_tsc(start);
        count = count + 1;
        delta = cycles - mean;
        mean += delta / (double)count;
        m2 += delta * (cycles - mean);
    #endif





    // Here we update h1_cap <- sigmoid( c + Wx1 + Uy1 )

    double* x1d = (double*)malloc(sizeof(double) * n);
    for (int i = 0; i < n; i++)
        x1d[i] = (double)x1[i];

    #ifdef PERF_H1_UPDATE
        myInt64 start;
        start = start_tsc();
    #endif
        h_update(h1_cap, y1, x1d);
    #ifdef PERF_H1_UPDATE
        myInt64 cycles = stop_tsc(start);
        count = count + 1;
        delta = cycles - mean;
        mean += delta / (double)count;
        m2 += delta * (cycles - mean);
    #endif


    for (int i = 0; i < D; i++) {
        img_tmp_mem0[i] = (double)x0[i];
        img_tmp_mem1[i] = (double)x1[i];
    }

    //Update Phase
    #ifdef PERF_W_UPDATE
        myInt64 start;
        start = start_tsc();
    #endif
        W_update(img_tmp_mem0, img_tmp_mem1);
    #ifdef PERF_W_UPDATE
        myInt64 cycles = stop_tsc(start);
        count = count + 1;
        delta = cycles - mean;
        mean += delta / (double)count;
        m2 += delta * (cycles - mean);
    #endif



    #ifdef PERF_B_UPDATE
        myInt64 start;
        start = start_tsc();
    #endif
        b_update();
    #ifdef PERF_B_UPDATE
        myInt64 cycles = stop_tsc(start);
        count = count + 1;
        delta = cycles - mean;
        mean += delta / (double)count;
        m2 += delta * (cycles - mean);
    #endif



    #ifdef PERF_C_UPDATE
        myInt64 start;
        start = start_tsc();
    #endif
        c_update();
    #ifdef PERF_C_UPDATE
        myInt64 cycles = stop_tsc(start);
        count = count + 1;
        delta = cycles - mean;
        mean += delta / (double)count;
        m2 += delta * (cycles - mean);
    #endif



    #ifdef PERF_D_UPDATE
        myInt64 start;
        start = start_tsc();
    #endif
        d_update(y0, y1);
    #ifdef PERF_D_UPDATE
        myInt64 cycles = stop_tsc(start);
        count = count + 1;
        delta = cycles - mean;
        mean += delta / (double)count;
        m2 += delta * (cycles - mean);
    #endif



    #ifdef PERF_U_UPDATE
        myInt64 start;
        start = start_tsc();
    #endif
        U_update(y0, y1);
    #ifdef PERF_U_UPDATE
        myInt64 cycles = stop_tsc(start);
        count = count + 1;
        delta = cycles - mean;
        mean += delta / (double)count;
        m2 += delta * (cycles - mean);
    #endif
}


/*
 Runs CODTrainingUpdate over all images and trains the model
 */
void COD_train()
{
    int i;
    for (i = 0; i < num_img_train; i++)
    {
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
            mean += delta / (double)count;
            m2 += delta * (cycles - mean);
        #endif
    }
}


double energy(int y, int *x, double * h, double * W,
              double * b, double * c, double * d, double * U,
              int n, int D, int K)
{
    double sum = 0;
    double tmp;

    // sum += h'Wx
    for (int i = 0; i < n; i++)
    {
        // Matrix line with vector multiplication
        tmp = 0;
        for (int j = 0; j < D; j++)
            tmp += W[i * D + j] * x[j];

        sum += tmp * h[i];
    }

    // sum += b'x
    for (int i = 0; i < D ; i++)
        sum += b[i] * x[i];

    // sum += c'h
    for (int i = 0; i < n; i++)
        sum += c[i] * h[i];

    // sum += d'y
    sum += d[y];

    // sum += h'Uy
    for (int i = 0; i < n; i++)
        sum += U[y * n + i] * h[i];

    return -sum;
}


double energy_for_all()
{
    double sum = 0;
    for (int k = 0; k < num_img_train ; k++)
    {
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
void gibbs_H(int y0)
{
    double sum, s1, s2, s3, s4, s5, s6, s7, s8;
    for (int i = 0; i < n ; i++)
    {
        s1 = 0;
        s2 = 0;
        s3 = 0;
        s4 = 0;
        s5 = 0;
        s6 = 0;
        s7 = 0;
        s8 = 0;
        for (int j = 0; j < D; j += 16)
        {
            s1 = s1 + W[i * D + j] * x0[j];
            s2 = s2 + W[i * D + j + 1] * x0[j + 1];
            s3 = s3 + W[i * D + j + 2] * x0[j + 2];
            s4 = s4 + W[i * D + j + 3] * x0[j + 3];
            s5 = s5 + W[i * D + j + 4] * x0[j + 4];
            s6 = s6 + W[i * D + j + 5] * x0[j + 5];
            s7 = s7 + W[i * D + j + 6] * x0[j + 6];
            s8 = s8 + W[i * D + j + 7] * x0[j + 7];

            s1 = s1 + W[i * D + j + 8] * x0[j + 8];
            s2 = s2 + W[i * D + j + 9] * x0[j + 9];
            s3 = s3 + W[i * D + j + 10] * x0[j + 10];
            s4 = s4 + W[i * D + j + 11] * x0[j + 11];
            s5 = s5 + W[i * D + j + 12] * x0[j + 12];
            s6 = s6 + W[i * D + j + 13] * x0[j + 13];
            s7 = s7 + W[i * D + j + 14] * x0[j + 14];
            s8 = s8 + W[i * D + j + 15] * x0[j + 15];
        }
        sum = c[i] + U[y0 * n + i] + s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8;

        double p = sigmoid(sum);
        double rand = uniform();
        if (rand < p)
        {
            h0[i] = 1;
        }
        else
        {
            h0[i] = 0;
        }
    }
}



/*
 Gibbs samplings for Y (label)
 Returns value for Y
 */
int gibbs_Y_()
{
    double sum = 0;
    double *y_temp = (double *) malloc(sizeof(double) * K);
    for (int i = 0; i < K; i++)
    {
        double val = d[i];
        for (int j = 0; j < n; j++)
        {
            val = val + U[i * n + j] * h0[j];
        }
        y_temp[i] = exp(val);
        sum = sum + y_temp[i];
    }

    for (int i = 0; i < K; i++)
        y_temp[i] = y_temp[i] / sum;

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
void gibbs_X_(int * x)
{
    double p1, p2, p3, p4, p5, p6, p7, p8;
    double sum1, sum2, sum3, sum4, sum5, sum6, sum7, sum8;
    double rand_n1, rand_n2, rand_n3, rand_n4, rand_n5, rand_n6, rand_n7, rand_n8;
    double h0tmp;

    for (int i = 0; i < D; i += 8)
    {
        sum1 = b[i];
        sum2 = b[i + 1];
        sum3 = b[i + 2];
        sum4 = b[i + 3];
        sum5 = b[i + 4];
        sum6 = b[i + 5];
        sum7 = b[i + 6];
        sum8 = b[i + 7];

        for (int j = 0; j < n ; j++)
        {
            h0tmp = h0[j];

            sum1 += W[j * D + i] * h0tmp;
            sum2 += W[j * D + i + 1] * h0tmp;
            sum3 += W[j * D + i + 2] * h0tmp;
            sum4 += W[j * D + i + 3] * h0tmp;
            sum5 += W[j * D + i + 4] * h0tmp;
            sum6 += W[j * D + i + 5] * h0tmp;
            sum7 += W[j * D + i + 6] * h0tmp;
            sum8 += W[j * D + i + 7] * h0tmp;
        }

        p1 = sigmoid(sum1);
        p2 = sigmoid(sum2);
        p3 = sigmoid(sum3);
        p4 = sigmoid(sum4);
        p5 = sigmoid(sum5);
        p6 = sigmoid(sum6);
        p7 = sigmoid(sum7);
        p8 = sigmoid(sum8);

        rand_n1 = uniform();
        rand_n2 = uniform();
        rand_n3 = uniform();
        rand_n4 = uniform();
        rand_n5 = uniform();
        rand_n6 = uniform();
        rand_n7 = uniform();
        rand_n8 = uniform();

        if (rand_n1 < p1) x[i] = 1; else x[i] = 0;
        if (rand_n2 < p2) x[i + 1] = 1; else x[i + 1] = 0;
        if (rand_n3 < p3) x[i + 2] = 1; else x[i + 2] = 0;
        if (rand_n4 < p4) x[i + 3] = 1; else x[i + 3] = 0;
        if (rand_n5 < p5) x[i + 4] = 1; else x[i + 4] = 0;
        if (rand_n6 < p6) x[i + 5] = 1; else x[i + 5] = 0;
        if (rand_n7 < p7) x[i + 6] = 1; else x[i + 6] = 0;
        if (rand_n8 < p8) x[i + 7] = 1; else x[i + 7] = 0;
    }
}

