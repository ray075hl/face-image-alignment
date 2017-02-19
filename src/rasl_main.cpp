/****************************************
author:			hl
date:			2017.02.14~2017.02.17
function:		problem solving process
*****************************************/

#include <opencv2/opencv.hpp>
#include <vector>
#include <rasl_type.h>
#include <math.h>

extern void linear_gamma_decompress(cv::Mat input);
extern cv::Mat gaussian_kernel(int size,  double sigma);
extern cv::Mat sobel_kernel();
extern cv::Mat similarityMatrix_to_parameters(cv::Mat smatrix);
extern cv::Mat parameters_to_similarity_matrix(cv::Mat xi);
extern cv::Mat makeform(cv::Mat T);
extern double norm_1_m(cv::Mat input);
extern double norm_2_m(cv::Mat input);
extern re_rasl_ialm ialm_t(cv::Mat D, std::vector<cv::Mat> J, double lambda);
extern cv::Mat image_jaco(cv::Mat lu, cv::Mat lv, int height, int width, cv::Mat xi);
extern re_rasl_qr QR_decomp(cv::Mat input);
extern void subplot(int h, int w, char *title, cv::Mat D, int p);

re_rasl_main rasl_main(std::vector<std::string> filename, std::vector<cv::Mat> transformations, int numimages, rasl_para raslpara){

    double sigma0 = 0.4;

    std::vector<cv::Mat> I0(numimages, cv::Mat::zeros(1,1, CV_64F));
    std::vector<cv::Mat> I0x(numimages, cv::Mat::zeros(1,1, CV_64F));
    std::vector<cv::Mat> I0y(numimages, cv::Mat::zeros(1,1, CV_64F));
    //------------------------------------------------------
    //              read and store full images
    //------------------------------------------------------
    for(int fileindex=0; fileindex<numimages; fileindex++){

        cv::Mat img1 = cv::imread(filename[fileindex]);
        std::vector<cv::Mat>  bgr;

        cv::split(img1, bgr);
        cv::Mat img = bgr[1];			//use green channel
		
        img.convertTo(img, CV_64FC1);
        linear_gamma_decompress(img);

        double  zoom_factor = sqrt(cv::determinant(transformations[fileindex](cv::Range(0,2), cv::Range(0,2))));
        cv::Mat g_kernel = gaussian_kernel(7, sigma0*zoom_factor);
        cv::Mat s_kernel = sobel_kernel();

        cv::filter2D(img, img, -1, g_kernel, cv::Point(-1,-1), 0, cv::BORDER_CONSTANT );
        cv::Mat imgx,imgy;
        cv::filter2D(img, imgx, -1, -s_kernel.t()/8, cv::Point(-1,-1), 0, cv::BORDER_CONSTANT );
        cv::filter2D(img, imgy, -1, -s_kernel/8,     cv::Point(-1,-1), 0, cv::BORDER_CONSTANT );

        /*// debug
        std::cout<< imgx.at<double>(24,24) <<" "<<sigma0*zoom_factor<<" "<<-s_kernel<<std::endl;
        cv::imshow("haha",imgx);
        cv::waitKey(0);
        */
        I0[fileindex] = img;
        I0x[fileindex] = imgx;
        I0y[fileindex] = imgy;
        //}
    }
    //--------------------------------------------------------
    //    get  the initial input images in canonical frame
    //--------------------------------------------------------
    int height = raslpara.canoicalsize.height;
    int width  = raslpara.canoicalsize.width;

    std::vector<cv::Mat> xi_initial;
    for (int i=0; i<numimages; i++){
        cv::Mat xx;
        xx = similarityMatrix_to_parameters(transformations[i]);
        xi_initial.push_back(xx);
    }

    cv::Mat D;
    for (int i=0; i<numimages; i++){
        cv::Mat d;
        cv::Mat tfm = makeform(transformations[i]);
        cv::warpPerspective(I0[i], d, tfm, cv::Size(width, height) ,cv::INTER_CUBIC, cv::BORDER_CONSTANT);


        if (i==0){
            D = d.reshape(1,height*width);
        }else{
            cv::hconcat(D,d.reshape(1,height*width),D);
        }

        /*
        //debug
        if (i==2){
            std::cout<<d.at<double>(1,1)<<std::endl;
            cv::imshow("haha",d);
            cv::waitKey(0);
        }*/
    }
    /*
    //debug
    cv::Mat mm = D.col(34).clone();
    cv::imshow("haha",mm.reshape(1,80));
    cv::waitKey(0);
    */
    //------------------------------------------
    //          start the main loop
    //------------------------------------------
    std::vector<cv::Mat> T_in(numimages, cv::Mat::zeros(1,1,CV_64F));
    for (int i=0; i<numimages; i++){
        cv::Mat temp_t = transformations[i].clone();
        T_in[i] = temp_t;
    }
    int numIterOuter = 0;
    int numIterInner = 0;

    bool converged = false;

    double prevObj = 1e7;
    double curObj;
    std::vector<cv::Mat> xi(numimages, cv::Mat::zeros(1, 1, CV_64FC1));

    cv::Mat DD = cv::Mat::zeros(height*width, numimages, CV_64FC1);
    std::vector<cv::Mat> J(numimages, cv::Mat::zeros(1,1,CV_64FC1));
    re_rasl_ialm re_ialm;
    while(!converged){

        numIterOuter = numIterOuter + 1 ;

        std::cout<<"Scale 1"<<" "<<"OutIter"<<numIterOuter<<std::endl;

        for (int i=0; i<numimages; i++){

            cv::Mat tfm = makeform(T_in[i]);

            cv::Mat I, Iu, Iv, temp, y;
            cv::warpPerspective(I0[i], temp, tfm, cv::Size(width, height) ,cv::INTER_CUBIC, cv::BORDER_CONSTANT);
            I = temp.reshape(1, height*width).clone();
            y = I.clone();
            cv::warpPerspective(I0x[i], temp, tfm, cv::Size(width, height) ,cv::INTER_CUBIC, cv::BORDER_CONSTANT);
            Iu = temp.reshape(1, height*width).clone();
            cv::warpPerspective(I0y[i], temp, tfm, cv::Size(width, height) ,cv::INTER_CUBIC, cv::BORDER_CONSTANT);
            Iv = temp.reshape(1, height*width).clone();

            cv::Mat tempx = (1/norm_2_m(y))*Iu - y.dot(Iu)/pow(norm_2_m(y),3)*y;
            Iu  = tempx.clone();
            cv::Mat tempy = (1/norm_2_m(y))*Iv - y.dot(Iv)/pow(norm_2_m(y),3)*y;
            Iv = tempy.clone();
            y  = y/norm_2_m(y);
			
            //DD(:,i) = y
            y.copyTo(DD.col(i));
			
            //transformation matrix to parameters
            xi[i] = similarityMatrix_to_parameters(T_in[i]);
			
            //compute jacobian matrix
            J[i]  = image_jaco(Iu, Iv, height, width, xi[i]);

        }
        //debug
//        cv::Mat mm = DD.col(33).clone();
//        std::cout<< T_in[33] <<std::endl;
//        cv::imshow("haha",30*mm.reshape(1,80));
//        cv::waitKey(0);
        double lambda = raslpara.lambdac/sqrt(1.0*height*width);

        //-----------------------------------------------------
        //                      rasl inner loop
        //-----------------------------------------------------
		
        //using QR to orthgonalize the Jacobian matrix
        re_rasl_qr qr_m;
        qr_m.Q = cv::Mat::zeros(1, 1, CV_64FC1);
        qr_m.R = cv::Mat::zeros(1, 1, CV_64FC1);
        std::vector<cv::Mat> Q(numimages, cv::Mat::zeros(1, 1, CV_64FC1));
        std::vector<cv::Mat> R(numimages, cv::Mat::zeros(1, 1, CV_64FC1));
        for (int i=0; i<numimages; i++){
            qr_m = QR_decomp(J[i]);
            Q[i] = qr_m.Q;
            R[i] = qr_m.R;
        }

        re_ialm = ialm_t(DD, Q, lambda);

        for (int i=0; i<numimages; i++){
            re_ialm.dt_dual[i] = R[i].inv()*re_ialm.dt_dual[i];
        }
        //-----------------------------------------------------
		//
        //-----------------------------------------------------

        numIterInner += re_ialm.iter;

        cv::Mat S,U,V;
        cv::SVDecomp(re_ialm.A_dual, S, U, V);
        curObj = norm_1_m(S) + lambda*norm_1_m(re_ialm.E_dual);

        std::cout<<"previous objective function: "<< prevObj <<std::endl;
        std::cout<<"current  objective function: "<< curObj <<std::endl;

        for (int i=0; i<numimages; i++){
            xi[i] = xi[i] + re_ialm.dt_dual[i];
            T_in[i] = parameters_to_similarity_matrix(xi[i]);
        }

        //----------
        //
        //----------

        if((prevObj - curObj < raslpara.stoppingDelta) || numIterOuter >= raslpara.maxiter){


            converged = true;
            if(prevObj - curObj >= raslpara.stoppingDelta){
                std::cout<<"Maximum iterations reached!!!"<<std::endl;
            }
        }else{

            prevObj = curObj;

        }


    }
	//-------------------
    //display
	//-------------------
    subplot(5,7, "aligned image", DD, 0);
    subplot(5,7, "low rank representation", re_ialm.A_dual, 0);
    subplot(5,7, "residual", re_ialm.E_dual, 0);
    subplot(5,7, "original image", D, 1);
}
