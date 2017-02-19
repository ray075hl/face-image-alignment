/**************************************************************************
author: 					hl
created time: 				2017.02.07
function:					solve similarity matrix by given two matching points  
similarity transformation:
							T  =	 |s  0| |cos(r)  -sin(r)| + |tx|
									 |0  s| |sin(r)   cos(r)|   |ty|
***************************************************************************/
#include "opencv2/opencv.hpp"

cv::Mat two_point_similarity(cv::Mat pts1, cv::Mat pts2){
	
    double m[4] = {0, -1, 1, 0};
    cv::Mat delta(2, 2, CV_64F, m);
    cv::hconcat(pts1, pts1.col(0) + delta*(pts1.col(1) - pts1.col(0)), pts1);
    cv::hconcat(pts2, pts2.col(0) + delta*(pts2.col(1) - pts2.col(0)), pts2);
    cv::vconcat(pts1, cv::Mat::ones(1, 3, CV_64F), pts1);
    cv::Mat C1;
    cv::Mat C2;
    cv::Mat D;
    cv::hconcat(pts1.t(), cv::Mat::zeros(pts1.cols, pts1.rows, CV_64F), C1);
    cv::hconcat(cv::Mat::zeros(pts1.cols, pts1.rows, CV_64F), pts1.t(), C2);
    cv::vconcat(C1, C2, D);
    cv::Mat temp;
    cv::vconcat(pts2.row(0).t(), pts2.row(1).t(), temp);
    cv::Mat A_tr_vec = D.inv()*temp;
    return  A_tr_vec.reshape(0, 2);
	
}
