// Haar Wavelet Denoising
// The following code was implemented by Nghia Ho http://nghiaho.com/?p=2039

#include "Filters.h"


void HaarWavelet::HaarDecomposeArray(float *A, int width)
{
    const float inv_sqrt2 = 1/sqrt(2);
    
    float norm = 1.0f/sqrt(width);
    
    for(int i=0; i < width; i++) {
        A[i] *= norm;
    }
    
    float *tmp = new float[width];
    
    while(width > 1) {
        width /= 2;
        
        for(int i=0; i < width; i++) {
            tmp[i] = (A[2*i] + A[2*i+1]) * inv_sqrt2;
            tmp[width + i] = (A[2*i] - A[2*i+1]) * inv_sqrt2;
        }
        
        memcpy(A, tmp, width*2*sizeof(float));
    }
    
    delete [] tmp;
}

void HaarWavelet::HaarTransposeArray(float *A, int width, int height)
{
    float *B = new float[width*height];
    
    for(int y=0; y < height; y++) {
        for(int x=0; x < width; x++) {
            B[x*height + y] = A[y*width + x];
        }
    }
    
    memcpy(A, B, sizeof(float)*width*height);
    
    delete [] B;
}

void HaarWavelet::HaarDecomposeImage(float *A, int width, int height)
{
    for(int i=0; i < height; i++) {
        HaarDecomposeArray(&A[i*width], width);
    }
    
    HaarTransposeArray(A, width, height);
    
    for(int i=0; i < width; i++) {
        HaarDecomposeArray(&A[i*height], height);
    }
    
    HaarTransposeArray(A, height, width);
}

void HaarWavelet::HaarReconstructArray(float *A, int width)
{
    const float inv_sqrt2 = 1/sqrt(2);
    float inv_norm = sqrt(width);
    
    float *tmp = new float[width];
    int k = 1;
    
    while(k < width)  {
        for(int i=0; i < k; i++) {
            tmp[2*i] = (A[i] + A[k+i]) * inv_sqrt2;
            tmp[2*i+1] = (A[i] - A[k+i]) * inv_sqrt2;
        }
        
        memcpy(A, tmp, sizeof(float)*(k*2));
        
        k *= 2;
    }
    
    for(int i=0; i < width; i++) {
        A[i] *= inv_norm;
    }
    
    delete [] tmp;
}

void HaarWavelet::HaarReconstructImage(float *A, int width, int height)
{
    for(int i=0; i < width; i++) {
        HaarReconstructArray(&A[i*height], height);
    }
    
    HaarTransposeArray(A, height, width);
    
    for(int i=0; i < height; i++) {
        HaarReconstructArray(&A[i*width], width);
    }
    
    HaarTransposeArray(A, width, height);
}

void HaarWavelet::Denoise(const Mat &input, Mat &output, float threshold)
{
    assert(input.data != output.data);
    
    input.convertTo(output, CV_32F);
    
    HaarDecomposeImage((float*)output.data, output.cols, output.rows);
    
    for(int y=0; y < output.rows; y++) {
        float *ptr = (float*)output.ptr(y);
        
        for(int x=0; x < output.cols; x++) {
            // signbit returns 1 for negative numbers, and 0 otherwise
            // Haar wavelet shrinkage using soft thresholding
            ptr[x] = (signbit(ptr[x]) == 1 ? -1 : 1) * max(0.f, fabs(ptr[x]) - threshold);
        }
    }
    
    HaarReconstructImage((float*)output.data, output.rows, output.cols);
    
    output.convertTo(output, CV_8U);
}
