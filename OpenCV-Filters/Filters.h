//
//  Filters.h
//  OpenCV-Filters
//
//  Created by Justin Haupt on 1/17/17.
//  Copyright © 2017 Justin Haupt. All rights reserved.
//

#include <iostream>
#include <cctype>
#include <cstring>
#include <cmath>
#include <cassert>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Filters {
    
public:
    static void box(const char *, const char *, int, int);
    static void gaussian(const char *, const char *, int);
    static void median(const char *, const char *, int);
    static void bilateral(const char *, const char *, int, int, int);
    static void canny_edge(const char *, const char *, int, int);
    static void non_local_mean(const char *, const char *, int, int);
    static void haar_wavelet_denoise(const char *, const char *, int);
    static void anisotropic_diffusion_denoise(const char *, const char *, int);
};



class HaarWavelet {

public:
    static void HaarDecomposeArray(float *A, int width);
    static void HaarDecomposeImage(float *A, int width, int height);
    static void HaarTransposeArray(float *A, int width, int height);
    static void HaarReconstructArray(float *A, int width);
    static void HaarReconstructImage(float *A, int width, int height);
    static void Denoise(const Mat &input, Mat &output, float threshold);
};
