#include <stdio.h>
#include <math.h>
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


void W_update(double * W, double * h0_cap, double * h1_cap,
              int * x0, int * x1, double lambda, int n, int D){
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < D; j++)
        {
            W[i * D + j] = W[i * D + j] + lambda * (h0_cap[i] * x0[j] - h1_cap[i] * x1[j]);
        }
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
    if(y0!=y1)
    {
        d[y0] = d[y0] + lambda;
        d[y1] = d[y1] - lambda;
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
    
    
    
    //Update Phase
#ifdef PERF_W_UPDATE
    myInt64 start;
    start = start_tsc();
#endif
    W_update(W, h0_cap, h1_cap, x0, x1, lambda, n, D);
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

