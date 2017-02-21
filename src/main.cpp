/***********************************************
author:           	hl
created date:     	2017.02.14
last modified:		2017.02.17
function:			program entry
************************************************/
#include <opencv2/opencv.hpp>
#include <rasl_type.h>

extern re_rasl_get_image get_train_image(char* img_path, char* points_path, cv::Mat standard_eye, int length );
extern re_rasl_main rasl_main(std::vector<std::string> filename, std::vector<cv::Mat> transformations, int numimages, rasl_para x);


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int length = 35;
    double m[4] = {5, 55, 32, 32};
    cv::Mat stdeye(2,2, CV_64F, m);

    rasl_para raslpara;
    raslpara.canoicalsize       = cv::Size(60,80);
    raslpara.maxiter            = 25;
    raslpara.stoppingDelta      = 0.01;
    raslpara.inner_tol          = 1e-6;
    raslpara.inner_maxiter      = 1000;
    raslpara.continuationFlag   = 1;
    raslpara.mu                 = 1e-3;
    raslpara.lambdac            = 1.0;

    //std::cout<<raslpara.canoicalsize.height<<std::endl;
    char img_path[] = "./tony_blair_data/image/Tony_Blair_d";
    char point_path[] = "./tony_blair_data/yml/";


    re_rasl_get_image pp = get_train_image(img_path, point_path, stdeye, length);

/*
    //debug
    for (int i=0;i<length;i++){
        std::cout<<pp.filename_list[i]<<std::endl;
        std::cout<<std::endl;
        std::cout<<pp.T[i]<<std::endl;
    }
*/
    re_rasl_main kk = rasl_main(pp.filename_list, pp.T, pp.num_of_image, raslpara);








    return 0;
}
