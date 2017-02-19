/***************************************************
//author: 			hl
//created time: 	2017.02.07
//last modified:	2017.02.17
//function:			parameters to similarity matrix
****************************************************/
#include <opencv2/opencv.hpp>

cv::Mat parameters_to_similarity_matrix(cv::Mat xi){

    cv::Mat smatrix(2, 3, CV_64FC1);
    double theta = xi.at<double>(1, 0);
    double r[2][2] = {cos(theta), -sin(theta), sin(theta), cos(theta)};

    cv::Mat R(2, 2, CV_64FC1, r);
    smatrix(cv::Range(0,2), cv::Range(0,2)) = xi.at<double>(0,0)*R;
    smatrix.at<double>(0,2) = xi.at<double>(2,0);
    smatrix.at<double>(1,2) = xi.at<double>(3,0);

    return smatrix;
}

