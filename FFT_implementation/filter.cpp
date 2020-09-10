#ifndef FILTER_CPP
#define FILTER_CPP

#include "filter.h"
#include <math.h>
#include <algorithm>
#include <iostream>

#define PI 3.1415926535897933

using namespace std;

void GaussianFilter(complex<double>*data,int height, int width, int sigma){
    int y_center = height/2;
    int x_center = width/2;

    // set table to remove duplicate
    // int size = 0;
    // for(int i=1; i <= y_center+1 ; ++i){
    //     size += i + abs(x_center-y_center); 
    // }
    // double table[size];
    // for(int k=0, l=0, i=k ; i< y_center; ++i){
    //     for(int j=k; j<x_center; ++j){
    //         table[i][j] = (i-y_center)*(i-y_center) + (j-x_center)*(j-x_center);
    //     }
    //     k++;
    // }
    
    double *mask = new double [height*width];
    for(int i=0; i<height; ++i){
        for(int j=0; j<width; ++j){
            double tmp = (i-y_center)*(i-y_center) + (j-x_center)*(j-x_center);
            mask[ i*width + j ] = -1*(1/(2*PI*sigma*sigma))* exp(tmp/-(2*sigma*sigma));
        }
    }

    // linearly renormalise into range [0,1]
    double* get_num = std::min_element(mask, mask+height*width);
    double min = *get_num;
    get_num = std::max_element(mask, mask+height*width);
    double interval = *get_num - min;

    for(int i=0; i<height; ++i){
        for(int j=0; j<width; ++j){
            double tmp = (mask[ i*width + j ]- min) / interval;
            data[ i*width + j ] = data[ i*width + j ] * tmp;
        }
    }
}

#endif