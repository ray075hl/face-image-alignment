/****************************************
author:	hl
date:		2017.02.14
function: generate an 2D sobel kernel
		||  1  2  1  ||
		||  0  0  0  ||
		|| -1 -2 -1  ||
*****************************************/
#include <opencv2/opencv.hpp>

cv::Mat sobel_kernel(){

    double m[9] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
    cv::Mat sobel(3,3,CV_64FC1, m);
    return 1.0*sobel;
}
