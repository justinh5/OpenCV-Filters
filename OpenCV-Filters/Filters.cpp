//
//  Filters.cpp
//  OpenCV-Filters
//
//  Created by Justin Haupt on 1/17/17.
//  Copyright © 2017 Justin Haupt. All rights reserved.
//

#include "Filters.h"



void Filters::box(const char *source_path, const char *filename, int param1, int param2)
{
    Mat src = imread(source_path, CV_LOAD_IMAGE_COLOR);
    Mat dst = src.clone();
    
    boxFilter(src, dst, -1, Size((param1/2)+1,(param2/2)+1));

    imwrite(filename, dst);
    
    return;
}


void Filters::gaussian(const char *source_path, const char *filename, int param1)
{
    Mat src = imread(source_path, CV_LOAD_IMAGE_COLOR);
    Mat dst = src.clone();
    
    int x = param1*2+1;   //make parameter odd
    
    GaussianBlur(src, dst, Size(x,x), 0, 0);
    
    imwrite(filename, dst);
    
    return;
}


void Filters::median(const char *source_path, const char *filename, int param1)
{
    Mat src = imread(source_path, CV_LOAD_IMAGE_COLOR);
    Mat dst = src.clone();
    
    int temp = param1/2;
    
    int x = ((temp % 2) == 0) ? temp+1 : temp;    // make parameter odd
    
    medianBlur(src, dst, x);

    imwrite(filename, dst);
    
    return;
}


void Filters::bilateral(const char *source_path, const char *filename, int param1, int param2, int param3)
{
    Mat src = imread(source_path, CV_LOAD_IMAGE_COLOR);
    Mat dst = src.clone();
    
    bilateralFilter(src, dst, param1/2, param2*2, param3*2);
    
    imwrite(filename, dst);
    
    return;
}


void Filters::canny_edge(const char *source_path, const char *filename, int param1, int param2)
{
    int lowThreshold = param1*2;
    int ratio = 3;          // recommeded by Canny
    int temp = (100-param2)/25;
    int kernel_size = ((temp % 2) == 0) ? temp+1 : temp;    // make parameter odd
    
    Mat src, src_gray;
    Mat dst, detected_edges;
    
    src = imread(source_path, CV_LOAD_IMAGE_COLOR);
    dst.create(src.size(), src.type());
    
    cvtColor(src, src_gray, CV_BGR2GRAY);
    blur(src_gray, detected_edges, Size(kernel_size,kernel_size));
    Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, 3);
    
    dst = Scalar::all(0);
    src.copyTo(dst, detected_edges);
    
    imwrite(filename, dst);
    
    return;
}


void Filters::non_local_mean(const char *source_path, const char *filename, int param1, int param2)
{
    Mat src = imread(source_path, CV_LOAD_IMAGE_COLOR);
    Mat dst = src.clone();

    fastNlMeansDenoisingColored(src, dst, (param1/3)+1, (param2/3)+1, 7, 21);
    
    imwrite(filename, dst);
    
    return;
}


/* Implementation by Nghia Ho
 URL: http://nghiaho.com/?p=2039
 */
void Filters::haar_wavelet_denoise(const char *source_path, const char *filename, int param1)
{
    const int MAX_IMAGE_SIZE = 1024; // keep the image small for speed
    Mat g_img;
    Mat g_noisy_img;
    Mat g_padded_img;
    Mat g_denoised_img;
    Mat g_noise;
    float g_threshold = 0;
    
    g_img = imread(source_path, CV_LOAD_IMAGE_COLOR);
    assert(g_img.data);
    
    // Haar Wavelet requires image to be a power of two
    // We'll pad the borders and remove them later
    int max_dim = max(g_img.rows, g_img.cols);
    int padded_width = 0;
    int padded_height = 0;
    
    if(max_dim > MAX_IMAGE_SIZE) {
        resize(g_img, g_img, Size(g_img.cols*MAX_IMAGE_SIZE/max_dim, g_img.rows*MAX_IMAGE_SIZE/max_dim));
    }
    
    padded_width = pow(2, ceil(log(g_img.cols)/log(2)));
    padded_height = pow(2, ceil(log(g_img.rows)/log(2)));
    
    g_noisy_img = g_img;
    
    copyMakeBorder(g_noisy_img, g_padded_img, 0, padded_height-g_img.rows, 0, padded_width-g_img.cols, BORDER_CONSTANT);
    
    g_threshold = 0.1 * param1 / 100.0;
    
    if(g_padded_img.channels() == 3) {
        Mat bgr[3];
        split(g_padded_img, bgr);
     
        HaarWavelet::Denoise(bgr[0].clone(), bgr[0], g_threshold);
        HaarWavelet::Denoise(bgr[1].clone(), bgr[1], g_threshold);
        HaarWavelet::Denoise(bgr[2].clone(), bgr[2], g_threshold);
     
        merge(bgr, 3, g_denoised_img);
    }
    else {
        HaarWavelet::Denoise(g_padded_img, g_denoised_img, g_threshold);
    }
    
    g_denoised_img = g_denoised_img(Range(0, g_img.rows), Range(0, g_img.cols));
    
    imwrite(filename, g_denoised_img);
    
    return;
}


/* Implementation by OpenCV developer, Laurent Berger
   URL: http://answers.opencv.org/question/71005/isotropic-non-linear-diffusion-smoothing-perona-malik/
 */
void Filters::anisotropic_diffusion_denoise(const char *source_path, const char *filename, int param1)
{
    Mat src = imread(source_path, CV_LOAD_IMAGE_GRAYSCALE);
    Mat dst;
    src.convertTo(dst, CV_32FC1);
    
    double t=0;
    double lambda=0.25; // Defined in equation (7)
    double K=param1; // defined after equation(13) in text
    
    Mat x1,xc;
    
    while (t<20)
    {
        Mat D; // defined just before equation (5) in text
        Mat gradxX,gradyX; // Image Gradient t time
        Sobel(dst,gradxX,CV_32F,1,0,3);
        Sobel(dst,gradyX,CV_32F,0,1,3);
        D = Mat::zeros(dst.size(),CV_32F);
        for (int i=0;i<dst.rows;i++)
            for (int j = 0; j < dst.cols; j++)
            {
                float gx = gradxX.at<float>(i, j), gy = gradyX.at<float>(i,j);
                float d;
                if (i==0 || i== dst.rows-1 || j==0 || j==dst.cols-1) // conduction coefficient set to 1 p633 after equation 13
                    d=1;
                else
                    d =1.0/(1.0+abs((gx*gx + gy*gy))/(K*K)); // expression of g(gradient(I))
                //d =-exp(-(gx*gx + gy*gy)/(K*K)); // expression of g(gradient(I))
                D.at<float>(i, j) = d;
            }
        x1 = Mat::zeros(dst.size(),CV_32F);
        for (int i = 1; i < dst.rows-1; i++)
        {
            float *u1 = (float*)x1.ptr(i);
            u1++;
            for (int j = 1; j < dst.cols-1; j++,u1++)
            {
                // Value of I at (i+1,j),(i,j+1)...(i,j)
                float ip10=dst.at<float>(i+1, j),i0p1=dst.at<float>(i, j+1);
                float im10=dst.at<float>(i-1, j),i0m1=dst.at<float>(i, j-1),i00=dst.at<float>(i, j);
                // Value of D at at (i+1,j),(i,j+1)...(i,j)
                float cp10=D.at<float>(i+1, j),c0p1=D.at<float>(i, j+1);
                float cm10=D.at<float>(i-1, j),c0m1=D.at<float>(i, j-1),c00=D.at<float>(i, j);
                // Equation (7) p632
                *u1 = i00 + lambda/4*( (cp10+c00)*(ip10-i00) + (c0p1+c00)*(i0p1-i00) + (cm10+c00)*(im10-i00)+ (c0m1+c00)*(i0m1-i00));
                // equation (9)
            }
        }
        x1.copyTo(dst);
        dst.convertTo(xc,CV_8U);
        t=t+lambda;
    }

    imwrite(filename, dst);
    
    return;
}





