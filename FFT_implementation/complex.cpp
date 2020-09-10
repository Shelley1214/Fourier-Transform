#ifndef COMPLEX_CPP
#define COMPLEX_CPP

#include "complex.h"
#include <math.h>

template<typename T>
complex<T>::complex(T re, T im) {
    this-> real = re;
    this-> imag = im;
}

// Destructor
template<typename T>
complex<T>::~complex() {
}


// Sum of two matrices
template<typename T>
complex<T> complex<T>::operator+( complex<T>& other) {
    this->real += other.real;
    this->imag += other.imag;
    return *this;
}

template<typename T>
complex<T> complex<T>::operator-( complex<T>& other) {
    this->real -= other.real;
    this->imag -= other.imag;
    return *this;
}

// Multiplies this matrix with another from the left
template<typename T>
complex<T> complex<T>::operator*( complex<T>& other) {
    complex result(0,0);
    result.real = other.real * this->real - other.imag * this->imag ;
    result.imag = other.real * this->imag + other.imag * this->real;
    return result;
}

template<typename T>
complex<T> complex<T>::operator*(double& other) {
    this->real = this->real * other ;
    this->imag = this->imag * other;
    return *this;
}

template<typename T>
complex<T> complex<T>::operator/(int& other) {
    this->real = this->real / other;
    this->imag = this->imag / other;
    return *this;
}

template<typename T>
T complex<T>::get_real(){
    return this->real;
}

template<typename T>
int complex<T>::abs(){
    double power = this->imag*this->imag + this->real*this->real;
    double output = sqrt(power);
    return (int)output;
}
#endif
