#include <fftw3.h>
#include <iostream>

using namespace std;

int main( int argc, char** argv ){
    
    fftw_complex    *data_in;
    fftw_complex    *fft;        
    fftw_complex    *ifft;    
    fftw_plan       plan_f;
    fftw_plan       plan_b;

    int             width = 3000, height = 4096, step;
    unsigned char *img1_data = new unsigned char[width * height];
    
    // check for supplied argument
    if ( argc < 2 )  throw "Argument Exception";
    
    // load original image
    char* filename = argv[1];
    FILE* fp = fopen(filename, "rb");
    if(fp == NULL) throw "Argument Exception";
    int l;
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            fscanf( fp, "%ld", &l );
            img1_data[ i*width + j] = l;
        }
    }
    fclose(fp);    


    // initialize arrays for fftw operations
    data_in = fftw_alloc_complex(width * height);
    fft     = fftw_alloc_complex(width * height);
    ifft    = fftw_alloc_complex(width * height);

//     // create plans
    plan_f = fftw_plan_dft_1d( width * height, data_in, fft,  FFTW_FORWARD,  FFTW_ESTIMATE );
    plan_b = fftw_plan_dft_1d( width * height, fft,     ifft, FFTW_BACKWARD, FFTW_ESTIMATE );
    
    // load img1's data to fftw input
    for( int i = 0, k = 0 ; i < height ; i++ ) {
        for( int j = 0 ; j < width ; j++ ) {
            data_in[k][0] = ( double )img1_data[i * step + j];
            data_in[k][1] = 0.0;
            k++;
        }
    }
    
    // perform FFT
    fftw_execute( plan_f );
    
    // perform IFFT
    fftw_execute( plan_b );
    
    // normalize IFFT result
    for( i = 0 ; i < ( width * height ) ; i++ ) {
        ifft[i][0] /= ( double )( width * height );
	    cout << ifft[i][0] << endl;
    }

    // free memory
    fftw_destroy_plan( plan_f );
    fftw_destroy_plan( plan_b );
    fftw_free( data_in );
    fftw_free( fft );
    fftw_free( ifft );
    
    return 0;
}
