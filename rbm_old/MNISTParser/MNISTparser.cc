//Parser for the MNIST data set
//Fast Numerical Code 2016
// Sergio Solorzano, Omerzel  Ales ,Santhanam  Prabhakaran
//Code to read and parse the MNIST data set http://yann.lecun.com/exdb/mnist/



#include <math.h>
#include <iostream>
#include <vector>
#include <fstream> 
// #include <stdio.h>
using namespace std;

#define SIZE 5

int ReverseInt (int i)
{
    //taken from http://compvisionlab.wordpress.com/
    unsigned char ch1, ch2, ch3, ch4;
    ch1 = i & 255;
    ch2 = (i >> 8) & 255;
    ch3 = (i >> 16) & 255;
    ch4 = (i >> 24) & 255;
    return((int) ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
}

struct MNISTdata{

int numberOfObjects;
unsigned char * images;
unsigned char * labels; 

};

void InitializeMNISTdata (struct MNISTdata * data, char *imagesFileName, char *labelsFileName){

FILE *images = fopen(imagesFileName,"rb");
FILE *labels = fopen(labelsFileName,"rb");
    if(!images) {
        perror("Image file opening failed");
        exit (1);
    }
    if(!labels){
      perror("Label file opening failed");
        exit (1);
    }

  int magicNumber;
  int numberOfImages;
  int numberOfRows;
  int numberOfColumns;
  int labelNumberOfImages;
  int labelMagicNumber;
  int code;
   
  fseek(images,0,SEEK_SET); 
  fread(&magicNumber,sizeof(int),1,images);
  fread(&numberOfImages,sizeof(int),1,images);
  fread(&numberOfRows,sizeof(int),1,images);
  fread(&numberOfColumns,sizeof(int),1,images);

  fseek(labels,0,SEEK_SET);
  fread(&labelMagicNumber,sizeof(int),1,labels);
  fread(&labelNumberOfImages,sizeof(int),1,labels);



  magicNumber=ReverseInt(magicNumber);
  numberOfImages=ReverseInt(numberOfImages);
  numberOfRows=ReverseInt(numberOfRows);
  numberOfColumns=ReverseInt(numberOfColumns);
  labelNumberOfImages=ReverseInt(labelNumberOfImages);
  labelMagicNumber=ReverseInt(labelMagicNumber);


  (*data).numberOfObjects = numberOfImages;
  (*data).images = (unsigned char *) malloc(sizeof(unsigned char)*numberOfImages* numberOfRows* numberOfColumns);
  (*data).labels = (unsigned char *) malloc(sizeof(int)*numberOfImages);

  code=fread((*data).images,sizeof(unsigned char),numberOfImages* numberOfRows* numberOfColumns,images);

  if(code != numberOfImages* numberOfRows* numberOfColumns){
    perror("Image transfer failed");
        exit (1);
  }

  code = fread((*data).labels,sizeof(unsigned char),labelNumberOfImages,labels);
 if(code != labelNumberOfImages){
    perror("Label transfer failed");
        exit (1);
  }

  fclose(images);
  fclose(labels);

}


int main(){

    struct MNISTdata test;
    InitializeMNISTdata (&test, "../MNISTDataSet/t10k-images-idx3-ubyte", "../MNISTDataSet/t10k-labels-idx1-ubyte");
    

    cout<<(test).numberOfObjects<<endl;

    ofstream imageout ("image2.dat");
    int sample =300;

    cout<<"sample label "<<(int)((test).labels)[sample]<<endl;

    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 28 - 1; j++) {
            imageout << (double )(test).images[sample*(28*28)+28*i+j] << " ";
            printf("%i ",(test).images[sample*(28*28)+28*i+j]);
        }
        imageout << (double )(test).images[sample*(28*28)+28*i+27] << endl;
        printf("%i \n",(test).images[sample*(28*28)+28*i+27]);
    }

    imageout.close();

  return 0;
}
