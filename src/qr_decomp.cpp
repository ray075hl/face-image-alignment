/********************************************************************
author:   		hl
date:     		2017.02.09
last modified:	2017.02.15
function: 		householder QR decomposition(economy qr decomposition likes matlab [q, r] = qr(A, 0))   
*********************************************************************/
#include <opencv2/opencv.hpp>
#include <rasl_type.h>

int min_s(int a, int b){

    return a>b ? b : a;
}


re_rasl_qr QR_decomp(cv::Mat A){

    int m = A.rows;
    int n = A.cols;
    int p = min_s(m, n);
    cv::Mat QR = A.clone();
    cv::Mat Rdaig = cv::Mat::zeros(p, 1, CV_64FC1);

    for(int k=0; k<p; k++){

        double nrm = 0;
        for(int i=k;i<m;++i)
            nrm += QR.at<double>(i,k)*QR.at<double>(i,k);
        nrm = sqrt(nrm);

        if(nrm != 0.0){

            if(QR.at<double>(k,k) < 0)
                nrm = -nrm;

            for(int i=k; i<m; i++)
                QR.at<double>(i,k) /=nrm;

            QR.at<double>(k,k) += 1.0;

            //apply tranformation to remaining columns.
            for(int j=k+1; j<n; j++){
                double s = 0.0;
                for(int i=k; i<m; i++){
                    s += QR.at<double>(i,k)*QR.at<double>(i,j);
                }
                s = -s/QR.at<double>(k,k);
                for(int i=k; i<m; i++){
                    QR.at<double>(i,j) += s*QR.at<double>(i,k);
                }
            }
        }

        Rdaig.at<double>(k,0) = -nrm;
    }

    //---------------------------get Q---------------------------------
    cv::Mat Q = cv::Mat::zeros(m, p, CV_64FC1);

    for(int k=p-1; k>=0; --k){

        for(int i=0; i<m;++i)
             Q.at<double>(i,k) = 0;

        Q.at<double>(k,k) = 1;


        for(int j=k; j<p; ++j){
            if( QR.at<double>(k,k)!=0){
                double s = 0;
                for(int i=k; i<m; ++i)
                    s +=  QR.at<double>(i,k)* Q.at<double>(i,j);

                s = -s/ QR.at<double>(k,k);
                for(int i=k; i<m; ++i)
                     Q.at<double>(i,j) += s*QR.at<double>(i,k);
            }
        }

    }
    //---------------------------get Q---------------------------------
    //---------------------------get R---------------------------------
    cv::Mat R = cv::Mat::zeros(p, p, CV_64FC1);

    for(int i=0;i<p;i++)
        for(int j=0; j<p; ++j){
            if(i<j){
                R.at<double>(i,j) = QR.at<double>(i,j);
            }
            else if(i == j){
                R.at<double>(i,j) = Rdaig.at<double>(i);
            }else{
                R.at<double>(i,j) = 0;
            }
        }
    //---------------------------get R---------------------------------
    re_rasl_qr out;
    out.Q = Q;
    out.R = R;
    return out;
}
