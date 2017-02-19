/**************************
author:					hl
date:					2017.02.08
last modified:			2017.02.08
***************************/
#include <iostream>
#include <opencv2/opencv.hpp>


cv::Mat pos(cv::Mat input){
       
    cv::Mat output = input.clone();
    return  output.setTo(0, output<0);
            
}
