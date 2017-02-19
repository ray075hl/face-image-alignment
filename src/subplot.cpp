/*************************
author:		hl
date:		2017.02.17
funciton:	show mulitply pictures in single window like matlab subplot
**************************/

#include <opencv2/opencv.hpp>


void subplot(int h, int w, char* title,cv::Mat D, int p){


    int dstheight = 80*h;
    int dstwidth  = 60*w;

    cv::Mat dst = cv::Mat::zeros(dstheight, dstwidth, CV_64F);
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++){
            cv::Rect roi(cv::Rect(0+j*60,0+i*80, 60, 80)); //(x,y, w,h)
            cv::Mat targetROI = dst(roi);
            cv::Mat rawdata = D.col(i*w+j).clone();
            rawdata = rawdata.reshape(1,80);
            rawdata.copyTo(targetROI );
        }

    cv::namedWindow(title);
    if (p==0){
        cv::imshow(title, 30*dst);}
    else{
        cv::imshow(title, dst);
        cv::waitKey(0);
    }

}
