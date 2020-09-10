#include <iostream>
#include<math.h>
#include<algorithm>
#include <fstream> // output
#include "complex.h"
#include "filter.h"

#define PI 3.1415926535897933

using namespace std;

unsigned char* bmp_read(char*, int&, int&);
complex<double>* fft2(complex<double>*, int, int, int);
complex<double>* ifft2(complex<double>*, int, int, int);
unsigned char* image_back(complex<double> *, int , int );

int sigma = 5; // GaussianFilter

int main(int argc, char** argv){

    char* filename = argv[1];
    if(filename == NULL) throw "Argument Exception";
    int width = 0, height = 0;
    unsigned char *image = bmp_read(filename, width, height);

    // radix-2 fft with zeros padding
    int width_pad =  1 << (int)ceil(log2(width));
    int height_pad =  1 << (int)ceil(log2(height));
    complex<double>* data = new complex<double>[width_pad*height_pad]();
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            complex<double> temp ((int)image[ i*width_pad+j ], 0);
            data[ i*width_pad + j ] = temp;
        }
    }

    // [ image -> fft -> ifft -> image ] about 8sec(normal) / 2sec(O3) 
    data  = fft2(data, width_pad, height_pad, 1); 
    GaussianFilter(data, height_pad, width_pad, sigma);
    data = ifft2(data, width_pad, height_pad, -1); 
    image = image_back(data, width, height);

    ofstream fs("testfile.txt");
    for(int i=0; i<height; i++){
        for(int j=0; j< width; j++){
            if( j != width-1){
                fs << (int)image[i*width + j ] << ","; 
            }else{
                fs << (int)image[i*width + j ] ; 
            }
        }
        fs << endl;
    }
    fs.close();

    return 0;
}

unsigned char* image_back(complex<double> *data, int width, int height){

    int width_pad =  1 << (int)ceil(log2(width));
    unsigned char *image = new unsigned char [width*height];

    for(int i=0; i<height; ++i){
        for(int j=0; j<width; ++j){
            int tmp = data[ i*width_pad + j].abs();

            if(tmp > 255 ) image[ i*width + j ] = 255;
            else image[ i*width + j ] = tmp;
        }
    }
    return image;
}

void fft1(complex<double> *data, int length, int flag){

    // bit-reversal
    for (int i = 1, j = 0; i < length; ++i){
        int k = length >> 1;
        while( !((j ^= k)&k) ){
            k = k >> 1;
        }
        if (i > j) swap(data[i], data[j]);
    }

    // twiddle factor table
    complex<double> table[length/2];
    for(int i=0; i<length/2 ; ++i){
        complex<double> tmp(cos( 2*PI / length * i), -flag * sin( 2*PI / length * i));
        table[i] = tmp;
    }

    // butterfly computation
    for(int steplenght = 2; steplenght <= length ; steplenght*=2){
        for (int step = 0 ; step < length/steplenght ; ++step){
            for (int i = 0 ; i < steplenght/2 ; ++i){
                int Index0 = steplenght*step + i;
                int Index1 = steplenght*step + i + steplenght/2;
                complex<double> temp = data[Index0];
                complex<double> factor = data[Index1] * table[ length / steplenght * i];
            
                data[Index0] = data[Index0] + factor;
                data[Index1] = temp - factor;

            }
        }
    }

    if ( flag == -1 ){
        for ( int i = 0 ; i < length; i++ ) 
            data[i] = data[i] / length;
    }
}

complex<double>* fft2(complex<double>* data, int width, int height, int flag){

    complex<double>*row = new complex<double>[width];
	complex<double>*col = new complex<double>[height];
    
    // each row
    for (int c = 0; c < height; ++c) {
		for (int r = 0; r < width; ++r) row[r] = data[ c*width + r ];
		fft1(row, width, flag);
		for (int r = 0; r < width; ++r) data[ c*width + r ] = row[r];
	}

    //each col
    for (int r = 0; r < width; ++r){
        for(int c = 0; c < height; ++c) col[c] = data[ c*width + r ];
        fft1(col, height, flag);
        for(int c = 0; c < height; ++c) data[ c*width + r ] = col[c];
    }

    // fft shift
    complex<double>* result = new complex<double>[height*width];
    for(int i=0; i<height; ++i){
        rotate( data + i*width, data + i*width + width/2 ,data + (i+1)*width);
        int buff = (i + height/2) % height;
        for(int j=0; j<width; j++){
            result[ buff * width + j ] = data[ i*width + j ];
        }
    }
    return result;
}

complex<double>* ifft2(complex<double>* data, int width, int height, int flag){
    complex<double>*row = new complex<double>[width];
	complex<double>*col = new complex<double>[height];
    complex<double>* result = new complex<double>[height*width];

    // ifft shift
    for(int i=0; i<height; ++i){
        rotate( data + i*width, data + i*width + width/2 ,data + (i+1)*width);
        int buff = (i + height/2) % height;
        for(int j=0; j<width; j++){
            result[ buff * width + j ] = data[ i*width + j ];
        }
    }
    
    // each row
    for (int c = 0; c < height; ++c) {
		for (int r = 0; r < width; ++r) row[r] = result[ c*width + r ];
		fft1(row, width, flag);
		for (int r = 0; r < width; ++r) result[ c*width + r ] = row[r];
	}

    //each col
    for (int r = 0; r < width; ++r){
        for(int c = 0; c < height; ++c) col[c] = result[ c*width + r ];
        fft1(col, height, flag);
        for(int c = 0; c < height; ++c) result[ c*width + r ] = col[c];
    }
    return result;
}

unsigned char* bmp_read(char *filename, int &width, int &height) {

    FILE* fp = fopen(filename, "rb");
    if(fp == NULL) throw "Argument Exception";

    // read the 14-byte fileheader
    unsigned char fh[14];
    fread(fh, sizeof(char), 14, fp);
    fseek(fp, 0, SEEK_SET);

    // read the offset from the beginning of the file to the bitmap data
    int length = *(int*)&fh[10];
    unsigned char info[length];
    fread(info, sizeof(char), length, fp);

    width = *(int*)&info[18];
    height = *(int*)&info[22];
    unsigned char* data = new unsigned char[width * height];

    // read the rest of the data at once
    fread(data, sizeof(char), width*height , fp);
    fclose(fp);

    return data;
}