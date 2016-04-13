#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <test_param_prabhu.h> 

//RANDOM LINE ADDED

//#define D 784      //Dimension
//#define n 6000        //Number of hidden layers
#define lambda 0.005 //Training Rate
//#define K 10       //Number of classes

const int D = 784;
const int n = 6000;
const int K = 10;


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


//Array used for energy calculation
double * energy_temp ;


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




void gibbs_H(int * h0, int y0, int *x0);
int gibbs_Y(int* h0);
void gibbs_X(int * x, int * h0);



double uniform()
{
    return (double)rand() / (double)(RAND_MAX+1);
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
    {

        images_train[i] =  (int)images_train_init[i] < 127 ? 0 : 1;

    }


    for (int i = 0; i < num_img_test * D; i++)
    {

        images_test[i] =  (int)images_test_init[i] < 127 ? 0 : 1;

    }


}


void init_param()
{
    W = (double *) malloc(n * D * sizeof(double));
    b = (double *) malloc(D * sizeof(double));
    c = (double *) malloc(n * sizeof(double));
    d = (double *) malloc(K * sizeof(double));
    U = (double *) malloc(n * K * sizeof(double));
    h0_cap = (double *) malloc(sizeof(double) * n);
    energy_temp = (double *) malloc(sizeof(double)*n);

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
void get_images(int *_num_img, unsigned char **_images, int **_labels, char *imagesFileName, char *labelsFileName)
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
 * Update h0 in positive phase of COD_training_update
 */
void h0_cap_update(int y0, int * x0, double * c, double * W, double * U, int n, int D, int K, double * h0_cap){

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

        h0_cap[i] = sigmoid(sum); // DO NOT FORGET THE SIGMOID 
    }

}

/*
 Performs next step of training with a new image and it's class
*/
void COD_training_update(int yi, int * xi) //DONE
{
    //Positive Phase
    int y0 = yi;
    int * x0 = xi;

    //double * h0_cap = (double *) malloc(sizeof(double) * n);
    h0_cap_update(y0, x0, c, W, U, n, D, K, h0_cap);

    //Negative Phase
    int * h0 = (int *) malloc(sizeof(int) * n);
    gibbs_H(h0, y0, x0);
    int y1 = gibbs_Y(h0);
    int * x1 = (int *) malloc(sizeof(int) * D);
    gibbs_X(x1, h0);

    double * h1_cap = (double *) malloc(sizeof(double) * n);

    for (int i = 0; i < n ; i++)
    {
        double sum = c[i];
        for (int j = 0; j < D; j++)
        {
            sum = sum + W[i * D + j] *  x1[j];
        }


        //Can be optimized
        for (int j = 0; j < K; j++)
        {
            if (j == y1) sum = sum + U[i * K + j];
        }

        h1_cap[i] = sigmoid(sum);
    }

    //Update Phase 

    //Update W
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < D; j++)
        {
            W[i * D + j] = W[i * D + j] + lambda * (h0_cap[i] * x0[j] - h1_cap[i] * x1[j]);
        }
    }

    //Update b
    for (int i = 0; i < D; i++)
    {
        b[i] = b[i] + lambda * (x0[i] - x1[i]);
    }

    //Update c
    for (int i = 0; i < n; i++)
    {
        c[i] = c[i] + lambda * (h0_cap[i] - h1_cap[i]);
    }

    //Update d
    for (int i = 0; i < K; i++)
    {
        if (i == y0)
            d[i] = d[i] + lambda;

        if (i == y1)
            d[i] = d[i] + lambda * (- 1);
    }

    //Update U
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

    //free(h0_cap);
    free(h1_cap);
    free(x1);
    free(h0);

}

double energy(int y, int *x)
{
    double sum = 0;
    
    //double * energy_temp = (double *) malloc(sizeof(double)*n);
    
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<D;j++)
        {
            energy_temp[i] = W[i*D+j] * x[i];
        }
        sum = sum + energy_temp[i] * h0_cap[i];
    }
    
    for(int i=0; i<D ;i++)
    {
        sum = sum + b[i] * x[i];
    }
    
    for(int i=0; i<n;i++)
    {
        sum = sum + c[i] * h0_cap[i];
    }
    
    sum = sum + d[y];
    
    
    for(int i=0;i<n;i++)
    {
        energy_temp[i] = U[i*K+y];
        sum = sum + energy_temp[i] * h0_cap[i];
    }
    
    return -1*sum;
}

double energy_for_all()
{
    double sum = 0;
    
    for(int k=0; k<num_img_train ; k++)
    {
        //printf("energy image %d \n ", k);
        sum = sum + energy(get_label(k, labels_train), get_image(k, images_train));
    }

    return sum;
}
/*
 Runs CODTrainingUpdate over all images and trains the model
*/
void COD_train()
{
    int i;
    for (i = 0; i < num_img_train; i++)
    {
        printf("training image %d \n ", i + 1);
        COD_training_update(get_label(i, labels_train), get_image(i, images_train));
        // if(i%500==0)printf("training image %d \n ", i + 1);

         if(i==0)
         {
           printf(" Energy:%lf\n", energy_for_all());
           //fflush(stdout);
         }
    }
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
int gibbs_Y(int* h0)
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
void gibbs_X(int * x, int * h0)
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

            prod += log((1 + exp(U[j * K + y] + precomValues[j])));
        }
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
            score++;
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


// NO TESTING HERE
//#include "testfunctions.h"

int main()
{

    get_images(&num_img_train, &images_train_init, &labels_train,
               "../MNISTDataSet/train-images-idx3-ubyte", "../MNISTDataSet/train-labels-idx1-ubyte");

    get_images(&num_img_test, &images_test_init, &labels_test,
             "../MNISTDataSet/t10k-images-idx3-ubyte", "../MNISTDataSet/t10k-labels-idx1-ubyte");

    image_pp();

    check_images_labels(300, 222);

    init_param();

    //exit(0);
    //num_img_train=1000;
    //num_img_test=200;
    //read_parameters();
    printf(" DONE Initializing W b c d and U\n");
    fflush(stdout);
    int T=0;
    while(1){
    //printf("Traing time %d \n",T++);
    COD_train();
    predict_images ();
    double score = score_function();
    printf("Errors %d %lf \n", T++, score);
    //save_parameters ();

    }

    return 0;
}



