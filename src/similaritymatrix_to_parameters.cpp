/***************************************************
author: 			hl
created date: 		2017.02.07
last modified:		2017.02.07
function:			similarity matrix to parameters 
****************************************************/

#include <opencv2/opencv.hpp>

cv::Mat similarityMatrix_to_parameters(cv::Mat smatrix){

    cv::Mat xi(4, 1, CV_64F);

    cv::Mat T = smatrix(cv::Range(0,2), cv::Range(0,2));
    cv::Mat S = T.t()*T;
    xi.at<double>(0,0) = sqrt(S.at<double>(0,0));

    double theta = acos(smatrix.at<double>(0,0)/xi.at<double>(0,0));
    if (smatrix.at<double>(1,0) < 0)     theta = -theta;
    xi.at<double>(1,0) = theta;
    xi.at<double>(2,0) = smatrix.at<double>(0,2);
    xi.at<double>(3,0) = smatrix.at<double>(1,2);

    return xi;
}
