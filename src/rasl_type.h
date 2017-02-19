/********************************
author:		hl
date:		2017.02.08~2017.02.17
function:	self define type
*********************************/
#ifndef RASL_TYPE_H
#define RASL_TYPE_H

typedef struct{

    cv::Mat Q;
    cv::Mat R;

}re_rasl_qr;

typedef struct{

    cv::Mat A_dual;
    cv::Mat E_dual;
    std::vector<cv::Mat> dt_dual;
    int iter;
    cv::Mat Y;

}re_rasl_ialm;


typedef struct{

    std::vector<std::string> filename_list;
	std::vector<cv::Mat> T;
	int num_of_image;

}re_rasl_get_image;

typedef struct{
	
	cv::Mat D;
	cv::Mat Do;
	cv::Mat A;
	cv::Mat E;
	cv::Mat xi;
	int numIterOuter;
	int numIterInner;

}re_rasl_main;

typedef struct{
    cv::Size    canoicalsize;
    int         maxiter;
    double       stoppingDelta;
    double       inner_tol;
    int         inner_maxiter;
    int         continuationFlag;
    double       mu;
    double       lambdac;
}rasl_para;
#endif // RASL_TYPE_H

