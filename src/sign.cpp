/********************
author:		hl
date:		2017.02.09
*********************/

#include <opencv2/opencv.hpp>

cv::Mat sign(cv::Mat in){

    cv::Mat out = in.clone();

    out.setTo( 1, out>0.0 );
    out.setTo(-1, out<0.0 );
    return out;

}
