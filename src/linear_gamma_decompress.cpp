/***********************************
author:				hl
created date:		2017.02.08
last modified:		2017.02.08
function: 			linear gamma decompress
************************************/
#include <opencv2/opencv.hpp>


void linear_gamma_decompress(cv::Mat input){
    input.convertTo(input, CV_64F, 1/255.0);
    //return input;
}
