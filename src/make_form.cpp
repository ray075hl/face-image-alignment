/****************************************
author:   		  hl
created date:     2017.02.14
last modified:	  2017.02.17
function: 		  inverse transformation
*****************************************/
#include <opencv2/opencv.hpp>


cv::Mat makeform(cv::Mat T){
    //T row 2 col 3
    cv::Mat tfm = cv::Mat::eye(3, 3, CV_64F);

    for (int i=0; i<T.rows; i++)
        for (int j=0; j<T.cols; j++)
            tfm.at<double>(i,j) = T.at<double>(i,j);

    tfm = tfm.inv();

    return tfm;
}

