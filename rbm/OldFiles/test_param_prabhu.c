#define UNITTEST_MODE
#include "fenv.h"
void check_bounds(char []);
#include "rbmV3.c"


void check_bounds(char location[100])
{
    int raised = fetestexcept (FE_OVERFLOW | FE_INVALID | FE_UNDERFLOW | FE_DIVBYZERO| FE_INEXACT);
    if (raised & FE_OVERFLOW) { printf("FE_OVERFLOW - %s\n",location); exit(0); }
    if (raised & FE_INVALID) {printf("FE_INVALID - %s\n",location); exit(0); }
    if (raised & FE_UNDERFLOW) {printf("FE_UNDERFLOW - %s\n",location); exit(0); }
    if (raised & FE_DIVBYZERO) {printf("FE_DIVBYZERO - %s\n",location); exit(0); }
    //if (raised & FE_INEXACT) {printf("FE_INEXACT - %s\n",location); exit(0); }

}

double *W_prev;
double *b_prev;
double *c_prev;
double *d_prev;
double *U_prev;

void init_param_test()
{
    W_prev = (double *) malloc(n * D * sizeof(double));
    b_prev = (double *) malloc(D * sizeof(double));
    c_prev = (double *) malloc(n * sizeof(double));
    d_prev = (double *) malloc(K * sizeof(double));
    U_prev = (double *) malloc(n * K * sizeof(double));
}

void copy_param()
{
    for(int i=0;i<n*D;i++)
    {
        W_prev[i] = W[i];
    }
    
    for(int i=0;i<D;i++)
    {
        b_prev[i] = b[i];
    }
    
    for(int i=0;i<n;i++)
    {
        c_prev[i] = c[i];
    }
    
    for(int i=0;i<K;i++)
    {
        d_prev[i] = d[i];
    }
    
    for(int i=0;i<n*K;i++)
    {
        U_prev[i] = U[i];
    }
}

void test_params()
{
    double sum_W = 0;
    double sum_b = 0;
    double sum_c = 0;
    double sum_d = 0;
    double sum_U = 0;
    
    for(int i=0;i<n*D;i++)
    {
        sum_W = sum_W + fabs(W_prev[i] - W[i]);
    }
    
    for(int i=0;i<D;i++)
    {
        sum_b = sum_b + fabs(b_prev[i] - b[i]);
    }
    
    for(int i=0;i<n;i++)
    {
        sum_c = sum_c + fabs(c_prev[i] - c[i]);
    }
    
    for(int i=0;i<K;i++)
    {
        /*if(fabs(d_prev[i] - d[i])>0.00500)
        {
            
            printf("Prev:%0.16lf Curr:%0.16lf fabs:%lf(ERROR)\n", d_prev[i], d[i], fabs(d_prev[i] - d[i]));
            exit(0);
        }*/
        sum_d = sum_d + fabs(d_prev[i] - d[i]);
    }
    
    printf("Prev D:: ");
    for(int i=0;i<K;i++)
    {
        //printf("%d:%0.4lf ", i+1,d_prev[i]);
    }
    //printf("\n");
    
    printf("Curr D:: ");
    for(int i=0;i<K;i++)
    {
        //printf("%d:%0.4lf ", i+1,d[i]);
    }
    //printf("\n");
    
    for(int i=0;i<n*K;i++)
    {
        sum_U = sum_U + fabs(U_prev[i] - U[i]);
    }
    
    printf("W:%lf b:%lf c:%lf d:%lf U:%lf ", sum_W, sum_b, sum_c, sum_d, sum_U);
}

void COD_test_train()
{
    int i;
    copy_param();
    for (i = 0; i < num_img_train; i++)
    {
        printf("training image %d \n", i + 1);
        COD_training_update(get_label(i, labels_train), get_image(i, images_train));
        
        /*if(i%500==0)
        {
            test_params();
            printf(" Energy:%lf\n", energy_for_all());
            copy_param();
            fflush(stdout);
        }*/
    }
}


int main()
{
    srand(1);
    feclearexcept (FE_ALL_EXCEPT);
    check_bounds("Start");
    //feraiseexcept(FE_INEXACT);
    check_bounds("After Setting Inexact");
    
    get_images(&num_img_train, &images_train_init, &labels_train,
               "../MNISTDataSet/train-images-idx3-ubyte", "../MNISTDataSet/train-labels-idx1-ubyte");
    
    get_images(&num_img_test, &images_test_init, &labels_test,
               "../MNISTDataSet/t10k-images-idx3-ubyte", "../MNISTDataSet/t10k-labels-idx1-ubyte");
    
    image_pp();
    
    check_images_labels(300, 222);
    
    init_param();
    
    
    // Testing
    init_param_test();

    
    printf(" DONE Initializing W b c d and U\n");
    fflush(stdout);
    int T=0;
    while(1){
        //printf("Traing time %d \n",T++);
        COD_test_train();
        check_bounds("After COD Test");
        predict_images ();
        check_bounds("After Image Prediction");
        double score = score_function();
        printf("Errors %d %lf \n", T++, score);
        
        check_bounds("After every iter. End");
        //save_parameters ();
        
    }
    
    return 0;
}



