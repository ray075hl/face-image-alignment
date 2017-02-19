/******************************
author:				hl
created date:		2017.02.13
last modified:		2017.02.17
fuction:			read images
*******************************/
#include <opencv2/opencv.hpp>
#include <vector>
#include <rasl_type.h>

extern cv::Mat two_point_similarity(cv::Mat pts1, cv::Mat pts2);

re_rasl_get_image get_train_image(char* img_path, char* points_path, cv::Mat standard_eye, int length ){

    re_rasl_get_image out;
    out.num_of_image = length;
    char img_path2[256];
    char points_path2[256];
    cv::Mat p1;
    cv::Mat t;
    for (int i=1; i<length+1; i++){

        sprintf(img_path2,"%s%04d.jpg",img_path,i);
        out.filename_list.push_back(img_path2);

        sprintf(points_path2,"%s%d.yml",points_path,i);
        cv::FileStorage fs(points_path2, cv::FileStorage::READ);
        fs["points"] >> p1;

        t = two_point_similarity(standard_eye, p1);
        out.T.push_back(t);
        fs.release();
    }

    return out;
}
