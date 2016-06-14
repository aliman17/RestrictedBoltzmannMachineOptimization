
void check_images_labels(int sample1, int sample2){


    printf("%d\n",num_img_train);
    int sample = sample1;
    
    printf("sample label %d\n",labels_train[sample]);   

    
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 28 - 1; j++) {
            //imageout << (double )(test).images[sample*(28*28)+28*i+j] << " ";
            printf("%f ",images_train[sample*(28*28)+28*i+j]);
        }
        //imageout << (double )(test).images[sample*(28*28)+28*i+27] << endl;
        printf("%f \n",images_train[sample*(28*28)+28*i+27]);
    }
    



    printf("%d\n",num_img_test);
    sample = sample2;
    
    printf(" test sample label %d\n",labels_test[sample]);   

    
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 28 - 1; j++) {
            //imageout << (double )(test).images[sample*(28*28)+28*i+j] << " ";
            printf("%f ",images_test[sample*(28*28)+28*i+j]);
        }
        //imageout << (double )(test).images[sample*(28*28)+28*i+27] << endl;
        printf("%f \n",images_test[sample*(28*28)+28*i+27]);
    }




}




void test_get_img_func(int sample){

  
    
    printf("sample label get image label test %d\n",get_label(sample, labels_train));   

    float * cur = get_image(sample, images_train);
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 28 - 1; j++) {
            //imageout << (double )(test).images[sample*(28*28)+28*i+j] << " ";
            printf("%f ",cur[28*i+j]);
        }
        //imageout << (double )(test).images[sample*(28*28)+28*i+27] << endl;
        printf("%f \n",cur[28*i+27]);
    }
    
}
