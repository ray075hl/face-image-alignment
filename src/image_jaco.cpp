//author:       hl
//date:         2017.02.08
//function:     obtain image jacobian matrix given by image derivation(dI/dx, dI/dy).

#include <opencv2/opencv.hpp>

//lu: dI/dx
//lv: dI/dy
//height:   standard canvas height
//width:    standard canvas width
//xi:       parameters of similarity transformation
cv::Mat image_jaco(cv::Mat lu, cv::Mat lv, int height, int width, cv::Mat xi){
    cv::Mat J = cv::Mat::zeros(height*width, 4, CV_64F);
    cv::Mat u(height, width, CV_64F);
    cv::Mat v(height, width, CV_64F);
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            u.at<double>(i,j) = j+1;
            v.at<double>(i,j) = i+1;
        }
    }

    u = u.reshape(1, height*width);     //vec(u)
    v = v.reshape(1, height*width);     //vec(v)

    double theta = xi.at<double>(1,0);
    double scale = xi.at<double>(0,0);
    cv::Mat temp;
    temp  = lu.mul( cos(theta)*u - sin(theta)*v ) +
                 lv.mul( sin(theta)*u + cos(theta)*v );
    temp.copyTo(J.col(0));

    temp = lu.mul(-scale*sin(theta)*u - scale*cos(theta)*v ) +
                 lv.mul( scale*cos(theta)*u - scale*sin(theta)*v );

    temp.copyTo(J.col(1));
    lu.copyTo(J.col(2));
    lv.copyTo(J.col(3));
    //cv::hconcat(J, t2, J);
    //cv::hconcat(J, lu, J);
    //cv::hconcat(J, lv, J);

    return J;
}
