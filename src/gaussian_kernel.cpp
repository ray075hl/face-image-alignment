/********************************************
author:				hl
created date:		2017.02.14
last modified:		2017.02.17
function: generate an 2D gaussian kernel given by odd size and same sigma, rho=0.
*********************************************/
#include <opencv2/opencv.hpp>
#include <math.h>
#define PI 3.14159265354

cv::Mat gaussian_kernel(int size,  double sigma){

    //size must be odd

    cv::Mat out(size, size, CV_64FC1);
    int center = ceil(size/2);
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){

           out.at<double>(i,j) = 1/(2*PI*sigma*sigma)*
                    exp(-( (i-center)*(i-center) + (j-center)*(j-center) )/(2*sigma*sigma));
        }
    }

    out = out/cv::sum(out)[0];

    return 1.0*out;
}
