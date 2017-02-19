/**************************************************************************************************************
author:				hl
date:				2017.02.09
last modified:		2017.02.17
function:			inexact argumented lagrangian multiplier method to solve deltaTau
cited: 				RASL matlab code by Yigang Peng
ref: 				Yigang Peng, Arvind Ganesh, John Wright, Wenli Xu, and Yi Ma. "RASL: Robust Alignment
					by Sparse and Low-rank Decomposition for Linearly Correlated Images," the 23th IEEE Conference
					on Computer Vision and Pattern Recognition (CVPR 10'), San Francisco, CA, USA, June 2010.
problem:			min ||A||_* + lambda||E||_1 + <Y_k, D+J*deltaTau-A-E> + mu/2||D+J*deltaTau-A-E||_F^2
***************************************************************************************************************/
#include <opencv2/opencv.hpp>
#include <vector>
#include <rasl_type.h>

extern double norm_2_m(cv::Mat input);
extern double norm_fro_m(cv::Mat input);
extern double norm_inf_m(cv::Mat input);
extern cv::Mat pos(cv::Mat input);
extern cv::Mat sign(cv::Mat in);

#define DISPLAY_EVERY 10

re_rasl_ialm ialm_t(cv::Mat D, std::vector<cv::Mat> J, double lambda){

	re_rasl_ialm out;
    double tol = 1e-7;
    int maxIter = 1000;
    double stoppingCriterion;
    int m = D.rows;
    int n = D.cols;
    //initialize
    cv::Mat Y = D.clone();      //deep copy

    double norm_two  = norm_2_m(Y);
    double norm_inf  = norm_inf_m(Y)/lambda;
    double dual_norm = cv::max(norm_two, norm_inf);
    Y = Y/dual_norm;

    cv::Mat obj_v = D.reshape(1, 1)*Y.reshape(1, 1).t();

    cv::Mat A_dual = cv::Mat::zeros(m, n, CV_64F);
    cv::Mat E_dual = cv::Mat::zeros(m, n, CV_64F);
    std::vector<cv::Mat> dt_dual(n, cv::Mat::zeros(1,1,CV_64F));
    cv::Mat dt_dual_matrix = cv::Mat::zeros(m, n, CV_64F);


    double mu  = 1.25/norm_2_m(D);

    double rho = 1.25;

    double d_norm = norm_fro_m(D);

    int iter = 0;
    bool converged = false;
    cv::Mat temp_T;
    cv::Mat U,S,V;
    cv::Mat Z;
    while(!converged){
        iter++;

        temp_T = D + dt_dual_matrix - E_dual + (1.0/mu)*Y;
        cv::SVDecomp(temp_T, S, U, V);
        //std::cout<<U.cols<<" "<<U.rows<<std::endl;
//        temp_M.m_Data = (float*)temp_T.data;
//        cv_re_svd re1 = SVD_decomp(temp_M);
//        U = re1.U;
//        S = re1.S;
//        V = re1.V;
        cv::Mat diag_S = pos(S-1/mu);
        A_dual = U*diag_S.diag(diag_S)*V;

        temp_T = D + dt_dual_matrix - A_dual + (1.0/mu)*Y;
        E_dual = sign(temp_T).mul(pos( cv::abs(temp_T) - lambda/mu ));

        temp_T = D - E_dual - A_dual + (1.0/mu)*Y;
        for(int i=0; i<n; i++){
            dt_dual[i]            =  -1.0*J[i].t()*temp_T.col(i);
            dt_dual_matrix.col(i) =  J[i]*dt_dual[i];
        }

        Z = D + dt_dual_matrix - A_dual - E_dual;
        Y = Y + mu*Z;

        obj_v = D.reshape(1, 1)*Y.reshape(1, 1).t();

        mu = mu*rho;
        stoppingCriterion = norm_fro_m(Z)/d_norm;




        //----------------------------display-------------------------
        //calculate  rank(A_dual)
        cv::Mat UA,SA,VA;
        cv::SVDecomp(A_dual, SA, UA, VA);

        SA.setTo(0, SA<1e-4);

        //calculate ||E||_0
        cv::Mat E_dual2 = E_dual.clone();
        E_dual2.setTo(0, E_dual2<1e-4);

        if (iter % DISPLAY_EVERY == 0){

            std::cout<<"#Iteration: "<<iter<<" "
                     <<"Rank(A): "<<cv::countNonZero(SA)<<" "
                     <<"||E||_0: "<<cv::countNonZero(E_dual2)<<" "
                     <<"objvalue: "<<obj_v<<" "
                     <<"Stopping Criterion: "<<stoppingCriterion<<std::endl;

        }

        if (stoppingCriterion <= tol){

            std::cout<<"IAML loop is converged at:"<<std::endl;
            std::cout<<"#Iteration: "<<iter<<" "
                     <<"Rank(A): "<<cv::countNonZero(SA)<<" "
                     <<"||E||_0: "<<cv::countNonZero(E_dual2)<<" "
                     <<"objvalue: "<<obj_v<<" "
                     <<"Stopping Criterion: "<<stoppingCriterion<<std::endl;

            converged = true;

        }

        if (!converged && iter >= maxIter){

            std::cout<<"Maximum iterations reached"<<std::endl;
            converged = 1;

        }

        //std::cout<<"intner iter: "<<iter<<std::endl;
    }

    out.A_dual  = A_dual;
    out.E_dual  = E_dual;
    out.dt_dual = dt_dual;
    out.iter    = iter;
    out.Y       = Y;

    return out;

}
