
/*
 Load all images into memory
 Returns pointer to the first image
*/
// char * getImages()

/*
 Get one image from the loaded set of images
 Output is a pointer to that image
*/
// char * getImage(int n)

/*
 Gives us lable of the image n
*/ 
// int getLabels(int n)

/*
 Performs next step of training with a new image and it's class
*/
// void CODTrainingUpdate(int yi, char * xi)

/*
 Runs CODTrainingUpdate over all images and trains the model
*/
 // void CODTrain()

/*
 Gibbs samplings for H (hidden nodes)
 Modifies h0
*/
 // void gibbsH(double * h0, int y0, char * x0)

 /*
 Gibbs samplings for Y (label)
 Returns value for Y 
*/
 // int gibbsY(double* h0)

 /*
 Gibbs samplings for X (image)
 Modifies x
*/
 // void gibbsH(char * x, double * h0)

 