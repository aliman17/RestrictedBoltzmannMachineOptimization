#include <stdio.h>
#include <math.h>
#include <stdlib.h>


#define D 784      //Dimension
#define n 2        //Number of hidden layers
#define lambda 0.5 //Training Rate
#define K 10       //Number of classes

//Training Data
unsigned char *images_train;
//Starting Pointer for all Images
int *labels_train; //labels in the dataset
int num_img_train; // Number of Images

//Test Data
unsigned char *images_test;
//Starting Pointer for all Images
int *labels_test; //labels in the dataset
int num_img_test; // Number of Images


// Training parameters
double *W;
double *b;
double *c;
double *d;
double *U;


int ReverseInt (int i)
{   
    unsigned char ch1, ch2, ch3, ch4;
    ch1 = i & 255;
    ch2 = (i >> 8) & 255;
    ch3 = (i >> 16) & 255;
    ch4 = (i >> 24) & 255;
    return((int) ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
}


void init_param()
{
    W = (double *) malloc(n * D * sizeof(double));
    b = (double *) malloc(D * sizeof(double));
    c = (double *) malloc(n * sizeof(double));
    d = (double *) malloc(K * sizeof(double));
    U = (double *) malloc(n * K * sizeof(double));
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

    FILE *image_file = fopen(imagesFileName,"rb");
    FILE *label_file = fopen(labelsFileName,"rb");
    if(!image_file) {
        perror("Image file opening failed");
        exit (1);
    }
    if(!label_file){
      perror("Label file opening failed");
        exit (1);
    }

    int magicNumber;
    int numberOfRows;
    int numberOfColumns;
    int labelNumberOfImages;
    int labelMagicNumber;
    int code;
   
    fseek(image_file,0,SEEK_SET); 
    fread(&magicNumber,sizeof(int),1,image_file);
    fread(&num_img, sizeof(int),1,image_file);
    fread(&numberOfRows,sizeof(int),1,image_file);
    fread(&numberOfColumns,sizeof(int),1,image_file);

    fseek(label_file,0,SEEK_SET);
    fread(&labelMagicNumber,sizeof(int),1,label_file);
    fread(&labelNumberOfImages,sizeof(int),1,label_file);

    magicNumber=ReverseInt(magicNumber);
    num_img=ReverseInt(num_img);
    numberOfRows=ReverseInt(numberOfRows);
    numberOfColumns=ReverseInt(numberOfColumns);
    labelNumberOfImages=ReverseInt(labelNumberOfImages);
    labelMagicNumber=ReverseInt(labelMagicNumber);

    *_num_img = num_img;
    
    images = (unsigned char *) malloc(sizeof(unsigned char)* num_img * numberOfRows * numberOfColumns);
    unsigned char * temp_labels = (unsigned char *) malloc(sizeof(int)* num_img); 
    labels = (int *) malloc(sizeof(int)* num_img); 

    code=fread(images,sizeof(unsigned char),num_img * numberOfRows* numberOfColumns,image_file);

    if(code != num_img * numberOfRows* numberOfColumns){
        perror("Image transfer failed");
        exit (1);
    }

    code = fread(temp_labels,sizeof(unsigned char),num_img,label_file);
    if(code != labelNumberOfImages){
        perror("Label transfer failed");
        exit (1);
    }

    for(int i=0;i<labelNumberOfImages;i++)
    {
        *(labels+i) = *(temp_labels+i);
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
unsigned char * get_image(int num, unsigned char *start)
{
    return start + num * D;
}

/*
 Gives us lable of the image n
*/ 
int get_label(int num, int * labels)
{
    return *(labels+num);
}

/*
 *Sigmoid Function 
 */
double sigmoid(double val)
{
    return 1/(1+exp(-1 * val));
}

/*
 Performs next step of training with a new image and it's class
*/
void COD_training_update(int yi, unsigned char * xi)
{
    //Positive Phase
    int y0 = yi;
    unsigned char * x0 = xi;
    
    double * h0_cap = (double *) malloc(sizeof(double) * n);

    for(int i=0; i<n ;i++)
    {
        double sum = c[i];
        for(j = 0; j< D; j++)
        {
            sum = sum + W[i*n+j] * (int) x0[j]; 
        }

        //Can be optimized
        for(j = 0; j < K; j++)
        {
            if(j==y0) sum = sum + U[i*n+j];
        }

        h0_cap[i] = sigmoid(sum);
    }


    //Negative Phase
    double * h0 = (double *) malloc(sizeof(double) * n);
    gibbs_H(h0, y0, x0);
    
}

/*
 Runs CODTrainingUpdate over all images and trains the model
*/
void COD_train()
{
    int i;
    for(i=0;i<num_img_train;i++)
    {
        COD_training_update(get_label(i, labels_train), get_image(i, images_train));
    }
}

/*
 Gibbs samplings for H (hidden nodes)
 Modifies h0
*/
void gibbs_H(double * h0, int y0, unsigned char *x0)
{
    for(int j=0;j<n;j++)
    {

    }
}

 /*
 Gibbs samplings for Y (label)
 Returns value for Y 
*/
int gibbs_Y(double* h0);

 /*
 Gibbs samplings for X (image)
 Modifies x
*/
void gibbs_X(char * x, double * h0);


int main()
{
    get_images(&num_img_train, &images_train, &labels_train,
        "../MNISTDataSet/t10k-images-idx3-ubyte", "../MNISTDataSet/t10k-labels-idx1-ubyte");
    
    printf("%d\n",num_img_train);
    int sample = 1000;
    
    printf("sample label %d\n",labels_train[sample]);   

    
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 28 - 1; j++) {
            //imageout << (double )(test).images[sample*(28*28)+28*i+j] << " ";
            printf("%i ",images_train[sample*(28*28)+28*i+j]);
        }
        //imageout << (double )(test).images[sample*(28*28)+28*i+27] << endl;
        printf("%i \n",images_train[sample*(28*28)+28*i+27]);
    }

    return 0;
}
