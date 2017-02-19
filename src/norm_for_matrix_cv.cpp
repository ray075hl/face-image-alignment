/**********************************
author:				hl
created date:		2017.02.18
last modified:		2017.02.18
function:			norm of matrix
***********************************/

#include <opencv2/opencv.hpp>

//function:	return L1 norm of a matrix
double norm_1_m(cv::Mat input){

    return cv::norm(input, cv::NORM_L1);

}

//function:	return L2 norm of a matrix
double norm_2_m(cv::Mat input){

    cv::Mat w, u, vt;
    cv::SVDecomp(input, w, u, vt);

    return  w.at<double>(0, 0);

}

//function:	return frobenius norm of a matrix
double norm_fro_m(cv::Mat input){

    return cv::norm(input, cv::NORM_L2);

}

//function:	return inf norm of the matrix
//inf norm:	max(abs(matrix))
double norm_inf_m(cv::Mat input){

    return cv::norm(input, cv::NORM_INF);

}