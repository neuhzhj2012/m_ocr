#include "ocr.h"  
#include <cv.h>  
#include <highgui.h>  
#include <iostream>  
using namespace cv;  
using namespace std;  

int main(int argc, char* argv[])  
{  
    char *image_path="/home/cv/work/deep_ocr/test_data_eng.png";  

    tesseract::TessBaseAPI  api;  
    api.Init(NULL,"eng",tesseract::OEM_DEFAULT);  

    api.SetPageSegMode(tesseract::PSM_AUTO);  

    STRING text_out;  
    if (!api.ProcessPages(image_path, NULL, 0, &text_out)) {  
        printf("Error during processing.\n");  
    }  

    cout<<"识别结果为："<<text_out.string();  

    return 0;  
} 
